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

#include "hosttracer.h"
#include "hostvolume.h"
#include "hostobject.h"
#include "hosttexture.h"
#include "hostbitmap.h"
#include "statistics.h"

namespace ExposureRender
{

/*! Bind/unbind a tracer
	@param[in] Tracer Tracer to bind/unbind
	@param[in] Bind whether to bind/unbind
*/
EXPOSURE_RENDER_DLL void BindTracer(const HostTracer& Tracer, const bool& Bind = true);

/*! Bind/unbind a volume
	@param[in] Volume Volume to bind/unbind
	@param[in] Bind whether to bind/unbind
*/
EXPOSURE_RENDER_DLL void BindVolume(const HostVolume& Volume, const bool& Bind = true);

/*! Bind/unbind an object
	@param[in] Object Object to bind/unbind
	@param[in] Bind whether to bind/unbind
*/
EXPOSURE_RENDER_DLL void BindObject(const HostObject& Object, const bool& Bind = true);

/*! Bind/unbind a texture
	@param[in] Texture Texture to bind/unbind
	@param[in] Bind whether to bind/unbind
*/
EXPOSURE_RENDER_DLL void BindTexture(const HostTexture& Texture, const bool& Bind = true);

/*! Bind/unbind a bitmap
	@param[in] Bitmap Bitmap to bind/unbind
	@param[in] Bind whether to bind/unbind
*/
EXPOSURE_RENDER_DLL void BindBitmap(const HostBitmap& Bitmap, const bool& Bind = true);

/*! Render tracer with \a TracerID
	@param[in] TracerID ID of the tracer to render
	@param[in,out] Statistics Output statistics
*/
EXPOSURE_RENDER_DLL void Render(int TracerID, Statistics& Statistics);

/*! Gets the running estimate from tracer with \a TracerID
	@param[in] TracerID ID of the tracer to render
	@param[out] pData Output buffer
*/
EXPOSURE_RENDER_DLL void GetDisplayEstimate(int TracerID, ColorRGBAuc* pData);

}
