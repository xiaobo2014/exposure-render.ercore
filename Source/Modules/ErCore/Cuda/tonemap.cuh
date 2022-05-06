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

namespace ExposureRender
{

KERNEL void KrnlToneMap(Tracer* pTracer)
{
	KERNEL_2D(pTracer->FrameBuffer.Resolution[0], pTracer->FrameBuffer.Resolution[1])

	ColorXYZAf RunningEstimateXYZ = pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy);

	RunningEstimateXYZ.ToneMap(pTracer->Camera.GetExposure());
	
	ColorRGBAuc ToneMapped = ColorRGBAuc::FromXYZAf(RunningEstimateXYZ.D);

// 	ToneMapped.GammaCorrect(pTracer->Camera.Gamma);

//	pTracer->FrameBuffer.PixelHysteresis(IDx, IDy).AddPixel(ToneMapped);

	pTracer->FrameBuffer.RunningEstimateRGB(IDx, IDy) = ToneMapped;//pTracer->FrameBuffer.PixelHysteresis(IDx, IDy).PixelAverage;
}

void ToneMap(Tracer& Tracer, Statistics& Statistics)
{
	LAUNCH_DIMENSIONS(Tracer.FrameBuffer.Resolution[0], Tracer.FrameBuffer.Resolution[1], 1, BLOCK_W, BLOCK_H, 1)
        
    ExposureRender::Tracer* pTracer = NULL;
    Cuda::Allocate(pTracer);
    Cuda::MemCopyHostToDevice(&Tracer, pTracer);

	LAUNCH_CUDA_KERNEL_TIMED((KrnlToneMap<<<GridDim, BlockDim>>>(pTracer)), "Tone map");

    Cuda::MemCopyDeviceToHost(pTracer, &Tracer);
    Cuda::Free(pTracer);
}

}
