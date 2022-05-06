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

/*! Range class */
class Range
{
public:
	/*! Constructor
		@param[in] MinMax Range
	*/
	HOST_DEVICE Range(const Vec2f& MinMax = Vec2f(0.0f)) :
		Min(MinMax[0]),
		Max(MinMax[1]),
		Length(MinMax[1] - MinMax[0]),
		InvLength(Length > 0.0f ? 1.0f / Length : 0.0f)
	{
	}
	
	/*! Assignment operator
		@param[in] Other Range to copy
		@return Range
	*/
	HOST Range& Range::operator = (const Vec2f& Other)
	{
		this->Min		= Other[0];
		this->Max		= Other[1];
		this->Length	= this->Max - this->Min;
		this->InvLength	= this->Length > 0.0f ? 1.0f / Length : 0.0f;

		return *this;
	}

	GET_SET_MACRO(HOST_DEVICE, Min, float)
	GET_SET_MACRO(HOST_DEVICE, Max, float)
	GET_MACRO(HOST_DEVICE, Length, float)
	GET_MACRO(HOST_DEVICE, InvLength, float)

protected:
	float	Min;				/*! Minimum range */
	float	Max;				/*! Maximum range */
	float	Length;				/*! Length of the interval */
	float	InvLength;			/*! Reciprocal of the interval length */
};

}
