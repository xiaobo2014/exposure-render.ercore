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

#include "mutate.h"

namespace ExposureRender
{

/*! Surface sample class */
class SurfaceSample
{
public:
	/*! Default constructor */
	HOST_DEVICE SurfaceSample() :
		P(0.0f),
		N(0.0f, 0.0f, 1.0f),
		UV(0.0f)
	{
	}

	/*! Assignment operator
		@param[in] Other Surface sample to copy
		@return Surface sample
	*/
    HOST_DEVICE SurfaceSample& operator = (const SurfaceSample& Other)
	{
		this->P		= Other.P;
		this->N		= Other.N;
		this->UV	= Other.UV;

		return *this;
	}

	/*
	GET_REF_SET_MACRO(HOST_DEVICE, P, Vec3f)
	GET_REF_SET_MACRO(HOST_DEVICE, N, Vec3f)
	GET_REF_SET_MACRO(HOST_DEVICE, UV, Vec2f)

protected:*/

	// FIXME: For some reason, using get/set function on this class has a significant impact on performance so for now we let them be public

	Vec3f	P;		/*! Position */
	Vec3f	N;		/*! Normal */
	Vec2f	UV;		/*! UV coordinates */
};

}
