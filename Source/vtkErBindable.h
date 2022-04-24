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

#include "vtkErDll.h"
#include "vtkErUtilities.h"

#include "exposurerender.h"

using namespace ExposureRender;
/*
#define ER_BINDABLE(type, name)												\
																			\
class VTK_ER_EXPORT vtkHostBindable##type##									\
{																			\
public:																		\
	vtkHostBindable##type##()												\
	{																		\
		ER_CALL(this->Bind());												\
	}																		\
																			\
	virtual ~vtkHostBindable##type##()										\
	{																		\
		ER_CALL(this->Unbind());											\
	}																		\
																			\
	void Bind()																\
	{																		\
		ExposureRender::Bind##type##(this->Bindable, true);					\
	}																		\
																			\
	void Unbind()															\
	{																		\
		ExposureRender::Bind##type##(this->Bindable, false);				\
	}																		\
																			\
	ExposureRender::Host##type	Bindable;									\
																			\
};
*/
//ER_BINDABLE(Tracer)
//ER_BINDABLE(Volume)
//ER_BINDABLE(Object)
//ER_BINDABLE(Texture)
//ER_BINDABLE(Bitmap)


//ER_BINDABLE(Tracer, Tracer)
//ER_BINDABLE(Volume, Volume)
//ER_BINDABLE(Object, Object)
//ER_BINDABLE(Texture, Texture)
//ER_BINDABLE(Bitmap, Bitmap)

class VTK_ER_EXPORT vtkHostBindableTracer
{
public:
    vtkHostBindableTracer()
    {
        ER_CALL(this->Bind());
    }

    virtual ~vtkHostBindableTracer()
    {
        ER_CALL(this->Unbind());
    }

    void Bind()
    {
        ExposureRender::BindTracer(this->Bindable, true);
    }

    void Unbind()
    {
        ExposureRender::BindTracer(this->Bindable, false);
    }

    ExposureRender::HostTracer	Bindable;

};

class VTK_ER_EXPORT vtkHostBindableVolume
{
public:
    vtkHostBindableVolume()
    {
        ER_CALL(this->Bind());
    }

    virtual ~vtkHostBindableVolume()
    {
        ER_CALL(this->Unbind());
    }

    void Bind()
    {
        ExposureRender::BindVolume(this->Bindable, true);
    }

    void Unbind()
    {
        ExposureRender::BindVolume(this->Bindable, false);
    }

    ExposureRender::HostVolume	Bindable;

};

class VTK_ER_EXPORT vtkHostBindableObject
{
public:
    vtkHostBindableObject()
    {
        ER_CALL(this->Bind());
    }

    virtual ~vtkHostBindableObject()
    {
        ER_CALL(this->Unbind());
    }

    void Bind()
    {
        ExposureRender::BindObject(this->Bindable, true);
    }

    void Unbind()
    {
        ExposureRender::BindObject(this->Bindable, false);
    }

    ExposureRender::HostObject	Bindable;

};

class VTK_ER_EXPORT vtkHostBindableTexture
{
public:
    vtkHostBindableTexture()
    {
        ER_CALL(this->Bind());
    }

    virtual ~vtkHostBindableTexture()
    {
        ER_CALL(this->Unbind());
    }

    void Bind()
    {
        ExposureRender::BindTexture(this->Bindable, true);
    }

    void Unbind()
    {
        ExposureRender::BindTexture(this->Bindable, false);
    }

    ExposureRender::HostTexture	Bindable;

};

class VTK_ER_EXPORT vtkHostBindableBitmap
{
public:
    vtkHostBindableBitmap()
    {
        ER_CALL(this->Bind());
    }

    virtual ~vtkHostBindableBitmap()
    {
        ER_CALL(this->Unbind());
    }

    void Bind()
    {
        ExposureRender::BindBitmap(this->Bindable, true);
    }

    void Unbind()
    {
        ExposureRender::BindBitmap(this->Bindable, false);
    }

    ExposureRender::HostBitmap	Bindable;

};
