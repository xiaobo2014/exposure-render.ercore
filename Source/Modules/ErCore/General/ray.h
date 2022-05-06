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

#include "vector.h"

namespace ExposureRender
{

/*! Ray class */
class Ray
{	
public:
	/*! Constructor
		@param[in] O Origin of the ray
		@param[in] D Ray direction
		@param[in] MinT Minimum ray range
		@param[in] MaxT Maximum ray range
	*/
	HOST_DEVICE Ray(Vec3f O = Vec3f(), Vec3f D = Vec3f(0.0f, 0.0f, 1.0f), float MinT = 0.0f, float MaxT = 1000000.0f) :
		O(O),
		D(D),
		MinT(MinT),
		MaxT(MaxT)
	{
	}

	/*! Copy constructor
		@param[in] Other Ray to copy
	*/
	HOST_DEVICE Ray(const Ray& Other) :
		O(),
		D(),
		MinT(),
		MaxT()
	{ 

		*this = Other;
	}
	
	/*! Assignment operator
		@param[in] Other Ray to copy
		@return Ray
	*/
	HOST_DEVICE Ray& operator = (const Ray& Other)
	{
		this->O			= Other.O;
		this->D			= Other.D;
		this->MinT		= Other.MinT;
		this->MaxT		= Other.MaxT;
		this->ImageUV	= Other.ImageUV;

		return *this;
	}
	
	/*! Advance ray
		@param[in] T Ray distance
		@return Advanced point along ray distance \a T
	*/
	HOST_DEVICE Vec3f operator()(const float& T) const
	{
		return this->O + Normalize(this->D) * T;
	}

	Vec3f 	O;				/*! Origin */
	Vec3f 	D;				/*! Direction */
	float	MinT;			/*! Minimum range */
	float	MaxT;			/*! Maximum range */
	Vec2f	ImageUV;		/*! Image UV coordinates */
};

}
