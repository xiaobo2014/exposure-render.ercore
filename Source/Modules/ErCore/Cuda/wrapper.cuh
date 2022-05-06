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

#include "exception.h"
#include "log.h"

#ifdef __CUDACC__SELF

#include <cuda.h>
#include <cuda_runtime_api.h>
#include <map>

using namespace std;

namespace ExposureRender
{

namespace Cuda
{

static inline void HandleCudaError(const cudaError_t& CudaError, const char* pTitle = "")
{
	char Message[256];

        //sprintf_s(Message, 256, "%s (%s)", cudaGetErrorString(CudaError), pTitle);

	if (CudaError != cudaSuccess)
		throw(Exception(Enums::Error, Message));
}

static inline void ThreadSynchronize()
{
	Cuda::HandleCudaError(cudaThreadSynchronize(), __FUNCTION__);
}

template<class T> static inline void Allocate(T*& pDevicePointer, int Num = 1)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMalloc((void**)&pDevicePointer, Num * sizeof(T)), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

static inline void MallocArray(struct cudaArray** CudaArray, const cudaChannelFormatDesc& ChannelFormatDescription, const Vec2i& Resolution, unsigned int Flags = 0)
{
	Cuda::ThreadSynchronize();

	HandleCudaError(cudaMallocArray(CudaArray, &ChannelFormatDescription, Resolution[0], Resolution[1]), __FUNCTION__);

	Cuda::ThreadSynchronize();
}

static inline void MemcpyToArray(struct cudaArray* Destination, const int& WidthOffset, const int& HeightOffset, const void* Source, const int& Count, enum cudaMemcpyKind Kind)
{
	Cuda::ThreadSynchronize();
	
	HandleCudaError(cudaMemcpyToArray(Destination, WidthOffset, HeightOffset, Source, Count, Kind), __FUNCTION__);

	Cuda::ThreadSynchronize();
}

static inline void Malloc3DArray(struct cudaArray** CudaArray, const cudaChannelFormatDesc& ChannelDescription, const Vec3i& Resolution, unsigned int Flags = 0)
{
	Cuda::ThreadSynchronize();

	cudaExtent CudaExtent;

	CudaExtent.width	= Resolution[0];
	CudaExtent.height	= Resolution[1];
	CudaExtent.depth	= Resolution[2];
	
	HandleCudaError(cudaMalloc3DArray(CudaArray, &ChannelDescription, CudaExtent, Flags), __FUNCTION__);

	Cuda::ThreadSynchronize();
}

template<class T> static inline void AllocatePiched(T*& pDevicePointer, const int Pitch, const int Width, const int Height)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMallocPitch((void**)&pDevicePointer, (size_t*)&Pitch, Width * sizeof(T), Height), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

template<class T> static inline void MemSet(T*& pDevicePointer, const int Value, int Num = 1)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemset((void*)pDevicePointer, Value, (size_t)(Num * sizeof(T))), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

template<class T> static inline void MemcpyArrayToArray(struct cudaArray* Dest, const int& DestWidthOffset, const int& DestHeightOffset, const struct cudaArray* Src, const int& SrcWidthOffset, const int& SrcHeightOffset, const int& Count, const enum cudaMemcpyKind Kind = cudaMemcpyDeviceToDevice)
{
	// FIXME: Thomas
	/*
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpyArrayToArray((void*)pDevicePointer, Value, (size_t)(Num * sizeof(T))), __FUNCTION__);
	Cuda::ThreadSynchronize();
	*/
}


template<class T> static inline void HostToConstantDevice(T* pHost, char* pDeviceSymbol, int Num = 1)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpyToSymbol(pDeviceSymbol, pHost, Num * sizeof(T)), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

template<class T> static inline void MemCopyHostToDeviceSymbol(T* pHost, const char* pDeviceSymbol, const int& Num = 1, const int& Offset = 0)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpyToSymbol(pDeviceSymbol, pHost, Num * sizeof(T), Offset, cudaMemcpyHostToDevice), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

template<class T> static inline void MemCopyDeviceToDeviceSymbol(T* pDevice, const char* pDeviceSymbol, const int& Num = 1, const int& Offset = 0)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpyToSymbol(pDeviceSymbol, pDevice, Num * sizeof(T), 0, cudaMemcpyDeviceToDevice), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

template<class T> static inline void MemCopyHostToDevice(T* pHost, T* pDevice, int Num = 1)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpy(pDevice, pHost, Num * sizeof(T), cudaMemcpyHostToDevice), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

template<class T> static inline void MemCopyDeviceToHost(T* pDevice, T* pHost, int Num = 1)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpy(pHost, pDevice, Num * sizeof(T), cudaMemcpyDeviceToHost), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

template<class T> static inline void MemCopyDeviceToDevice(T* pDeviceSource, T* pDeviceDestination, int Num = 1)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpy(pDeviceDestination, pDeviceSource, Num * sizeof(T), cudaMemcpyDeviceToDevice), __FUNCTION__);
	Cuda::ThreadSynchronize();
}

static inline void FreeArray(cudaArray*& pCudaArray)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaFreeArray(pCudaArray), __FUNCTION__);
	pCudaArray = NULL;
	Cuda::ThreadSynchronize();
}

template<class T> static inline void Free(T*& pBuffer)
{
	if (pBuffer == NULL)
		return;

	Cuda::ThreadSynchronize();
	
	HandleCudaError(cudaFree(pBuffer), __FUNCTION__);
	pBuffer = NULL;

	Cuda::ThreadSynchronize();
}

static inline void GetSymbolAddress(void** pDevicePointer, char* pSymbol)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaGetSymbolAddress(pDevicePointer, pSymbol), __FUNCTION__);
}

static inline void Memcpy3D(const struct cudaMemcpy3DParms* Memcpy3DParams)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaMemcpy3D(Memcpy3DParams), __FUNCTION__);
}

static inline void BindTextureToArray(const struct textureReference* TextureReference, const struct cudaArray* Array, const struct cudaChannelFormatDesc* ChannelFormatDescription)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaBindTextureToArray(TextureReference, Array, ChannelFormatDescription), __FUNCTION__);
}


static inline void GetTextureReference(const struct textureReference** TextureReference, const char* Symbol)
{
	Cuda::ThreadSynchronize();
	HandleCudaError(cudaGetTextureReference(TextureReference, Symbol), __FUNCTION__);
}

}

}

#endif
