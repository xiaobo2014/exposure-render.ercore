/*
*	@file
*	@author  Thomas Kroes <t.kroes at tudelft.nl>
*	@version 1.0
*	
*	@section LICENSE
*	
*	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
*	
*	Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*	Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
*	Neither the name of the TU Delft nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
*
*	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "macros.cuh"
#include "intersect.cuh"
#include "filtering.cuh"
#include "wrapper.cuh"
#include "textures.h"

namespace ExposureRender
{

inline DEVICE ColorXYZf Phong(Vec3f sample, ColorXYZf color, Vec3f gradient, Vec3f lightPos, Vec3f cameraPos)
{
    Vec3f N = Normalize(gradient);
    Vec3f L = Normalize(lightPos - sample);
    Vec3f V = Normalize(cameraPos - sample);

//         float3 shadedColor = getAmbientTerm(color);
//         shadedColor += getDiffuseTerm(color, N, L);
//        shadedColor += getSpecularTerm(color, N, L, V);

    float shade = 0.3f;
    float NdotL = max(Dot(N, L), 0.0f);
    shade += NdotL * 0.4f;

    Vec3f H = Normalize(V + L);                           //opt: nearly 50%!
    float NdotH = __powf(max(Dot(N, H), 0.0f), 60.f);
    shade += NdotH * 0.5f;

    return color * shade;
}

KERNEL void KrnlDvr(Tracer* pTracer, Volume* pVolume)
{
    //case 1:
    //printf("resolution: (%d, %d)\n", gpTracer->FrameBuffer.Resolution[0], gpTracer->FrameBuffer.Resolution[1]);

    //printf("gpTracer address: %d \n", gpTracer);
    //printf("gpVolumes address: %d \n", gpVolumes);

    //case 2:
    //printf("resolution: (%d, %d)\n", pTracer->FrameBuffer.Resolution[0], pTracer->FrameBuffer.Resolution[1]);

    //KERNEL_2D(gpTracer->FrameBuffer.Resolution[0], gpTracer->FrameBuffer.Resolution[1])

    KERNEL_2D(pTracer->FrameBuffer.Resolution[0], pTracer->FrameBuffer.Resolution[1])
        
    
	// Initialize the random number generator
	RNG RNG(&pTracer->FrameBuffer.RandomSeeds1(IDx, IDy), &pTracer->FrameBuffer.RandomSeeds2(IDx, IDy));
	
	ColorRGBAuc& DVR = pTracer->FrameBuffer.DVR(IDx, IDy);

	DVR = ColorRGBAuc::Black();

	// Camera ray
    
	Ray R;

	// Generate
	pTracer->Camera.Sample(R, Vec2i(IDx, IDy), RNG);
	
	//Volume& Volume = gpVolumes[pTracer->VolumeIDs[0]];

	if (!pVolume->BoundingBox.Intersect(R, R.MinT, R.MaxT))
		return;
	
	ColorXYZAf result = ColorXYZAf::Black();

	R.MinT += RNG.Get1() * gStepFactorPrimary;

	int NoSamples = 0;

    while (R.MinT <= R.MaxT && NoSamples < 300)
	{
		// Get sample point
        const Vec3f P = R(R.MinT);

		// Obtain gradient with central differences
//		const Vec3f N = Volume.GradientCD(P);
		
		// Obtain intensity
        const float Intensity = (*pVolume)(P);

		// Move along ray
		R.MinT += gStepFactorPrimary;
		
		ColorXYZf Diffuse = pTracer->VolumeProperty.GetDiffuse(Intensity);

		const float Opacity = pTracer->VolumeProperty.GetOpacity(Intensity)  * (gStepFactorPrimary * 200.0f);

		if (RNG.Get1() < Opacity)
		{
			float Sum = 0.0f;

			for (int i = 0; i < 5; i++)
			{
				Ray Rao;

				Rao.O		= P;
				Rao.D		= UniformSampleSphere(RNG.Get2());
				Rao.MinT	= 0.0f;
				
				float Alpha = 1;

				for (int s = 0; s < 5; s++)
				{
					Alpha *= pTracer->VolumeProperty.GetOpacity((*pVolume)(Rao((float)s * gStepFactorShadow)));
//					Alpha = Alpha + (1.0f - Alpha) * pTracer->GetOpacity(Volume(Rao(s * gStepFactorShadow)));
				}

				Sum += Alpha;
			}

			Diffuse = ColorXYZf(0.1f * Sum);
		}

		// Compositing
        result[0] = result[0] + (1.0f - result[3]) * Opacity * Diffuse[0];
        result[1] = result[1] + (1.0f - result[3]) * Opacity * Diffuse[1];
        result[2] = result[2] + (1.0f - result[3]) * Opacity * Diffuse[2];
        result[3] = result[3] + (1.0f - result[3]) * Opacity;

		NoSamples++;

		// Early ray termination
        if (result[3] >= 1.0f)
		{
            result[3]	= 1.0f;
            break;
        }
    }

	DVR[0] = Clamp((int)(result[0] * 255.0f), 0, 255);
	DVR[1] = Clamp((int)(result[1] * 255.0f), 0, 255);
	DVR[2] = Clamp((int)(result[2] * 255.0f), 0, 255);
	DVR[3] = Clamp((int)(result[3] * 255.0f), 0, 255);
    
}

void Dvr(Tracer& Tracer, Volume& Volume, Statistics& Statistics)
{
    LAUNCH_DIMENSIONS(Tracer.FrameBuffer.Resolution[0], Tracer.FrameBuffer.Resolution[1], 1, BLOCK_W, BLOCK_H, 1)

    ExposureRender::Tracer* pTracer = NULL;
    Cuda::Allocate(pTracer);    
    Cuda::MemCopyHostToDevice(&Tracer, pTracer);

    ExposureRender::Volume* pVolume = NULL;
    Cuda::Allocate(pVolume);
    Cuda::MemCopyHostToDevice(&Volume, pVolume);
    
    LAUNCH_CUDA_KERNEL_TIMED((KrnlDvr<<<GridDim, BlockDim >>>(pTracer, pVolume)), "DVR");

    Cuda::MemCopyDeviceToHost(pTracer, &Tracer);
    Cuda::Free(pTracer);

    Cuda::Free(pVolume);
}

}
