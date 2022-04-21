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

#include "geometry.h"
#include "utilities.h"

namespace ExposureRender
{

KERNEL void KrnlComputeEstimate(Tracer* pTracer)
{
	KERNEL_2D(pTracer->FrameBuffer.Resolution[0], pTracer->FrameBuffer.Resolution[1])

	pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[0] = CumulativeMovingAverage(pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[0], pTracer->FrameBuffer.FrameEstimate(IDx, IDy)[0], pTracer->NoEstimates + 1);
	pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[1] = CumulativeMovingAverage(pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[1], pTracer->FrameBuffer.FrameEstimate(IDx, IDy)[1], pTracer->NoEstimates + 1);
	pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[2] = CumulativeMovingAverage(pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[2], pTracer->FrameBuffer.FrameEstimate(IDx, IDy)[2], pTracer->NoEstimates + 1);
	pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[3] = CumulativeMovingAverage(pTracer->FrameBuffer.RunningEstimateXYZ(IDx, IDy)[3], pTracer->FrameBuffer.FrameEstimate(IDx, IDy)[3], pTracer->NoEstimates + 1);
}

void ComputeEstimate(Tracer& Tracer, Statistics& Statistics)
{
	LAUNCH_DIMENSIONS(Tracer.FrameBuffer.Resolution[0], Tracer.FrameBuffer.Resolution[1], 1, BLOCK_W, BLOCK_H, 1)

    ExposureRender::Tracer* pTracer = NULL;
    Cuda::Allocate(pTracer);
    Cuda::MemCopyHostToDevice(&Tracer, pTracer);

	LAUNCH_CUDA_KERNEL_TIMED((KrnlComputeEstimate<<<GridDim, BlockDim>>>(pTracer)), "Compute estimate");

    Cuda::MemCopyDeviceToHost(pTracer, &Tracer);
    Cuda::Free(pTracer);
}

}
