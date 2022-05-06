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

#include "filter.h"

namespace ExposureRender
{

/*! Mitchell filter */
class MitchellFilter : public Filter
{
public:
	/*! Constructor
		@param B Filter setting
		@param C Filter setting
		@param Size Size of the filter
	*/
	HOST_DEVICE MitchellFilter(const float& B = 1.0f / 3.0f, const float& C = 1.0f / 3.0f, const Vec2f& Size = Vec2f(1.0f)) :
		Filter(Size),
		B(B),
		C(C)
	{
	}
	
	/*! Evaluates the Mitchell filter
		@param[in] X X position
		@param[in] Y Y position
		@return Mitchell filter weight
	*/
	HOST_DEVICE float MitchellFilter::Evaluate(const float& X, const float& Y) const
	{
		return Mitchell1D(X * this->InvSize[0]) * Mitchell1D(Y * this->InvSize[1]);
	}
	
protected:
	/*! Evaluates the one-dimensional Mitchell filter
		@param[in] V V
		@return Mitchell filter weight
	*/
	HOST_DEVICE float Mitchell1D(const float& V) const
	{
		float av = ::fabsf(V);
        if (av > 1.0f)
        {
            return ((-this->B - this->C * 6.0f) * av * av * av +
                    (this->B * 6.0f + this->C * 30.0f) * av * av +
                    (this->B * -12.0f - this->C * 48.0f) * av +
                    this->B * 8.0f + this->C * 24.0f) / 6.0f;
        }
        else
        {
            return ((12.0f - this->B * 9.0f - this->C * 6.0f) * av * av * av +
                    (this->B * 12.0f + this->C * 6.0f - 18.0f) * av * av +
                    6.0f - this->B * 2.0f) / 6.0f;
        }
	}
	
	const float B;		/*! Internal precomputed quantity */
	const float C;		/*! Internal precomputed quantity */
};

}
