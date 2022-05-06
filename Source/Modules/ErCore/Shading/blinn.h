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

#include "montecarlo.h"

namespace ExposureRender
{

/*! Blinn microfacet distribution class */
class Blinn
{
public:
	/*! Default constructor */
	HOST_DEVICE Blinn(void)
	{
	}
	
	/*! Constructor
		@param[in] Exponent Blinn exponent
	*/
	HOST_DEVICE Blinn(const float& Exponent) :
		Exponent(Exponent)
	{
	}
	
	/*! Samples a random direction with importance sampling
		@param[in] Wo Outgoing direction
		@param[out] Wi Incoming direction
		@param[out] Pdf Probability of sampling \a Wi
		@param[in] U Random sample
	*/
	HOST_DEVICE void SampleF(const Vec3f& Wo, Vec3f& Wi, float& Pdf, const Vec2f& U)
	{
		const float CosTheta	= powf(U[0], 1.0f / (this->Exponent + 1));
		const float SinTheta	= sqrtf(max(0.0f, 1.0f - CosTheta * CosTheta));
		const float Phi			= U[1] * 2.0f * PI_F;

		Vec3f Wh = SphericalDirection(SinTheta, CosTheta, Phi);

		if (!SameHemisphere(Wo, Wh))
			Wh *= -1.0f;

		Wi = -Wo + 2.0f * Dot(Wo, Wh) * Wh;

		float BlinnPdf = ((Exponent + 1.0f) * powf(CosTheta, this->Exponent)) / (2.0f * PI_F * 4.0f * Dot(Wo, Wh));

		if (Dot(Wo, Wh) <= 0.0f)
			BlinnPdf = 0.0f;

		Pdf = BlinnPdf;
	}
	
	/*! Determines the probability of sampling \a Wi with \a Wo
		@param[in] Wo Outgoing direction
		@param[out] Wi Incoming direction
		@return Probability
	*/
	HOST_DEVICE float Pdf(const Vec3f& Wo, const Vec3f& Wi)
	{
		const Vec3f Wh = Normalize(Wo + Wi);

		const float CosTheta = AbsCosTheta(Wh);

		float Pdf = ((this->Exponent + 1.0f) * powf(CosTheta, this->Exponent)) / (2.0f * PI_F * 4.0f * Dot(Wo, Wh));

		if (Dot(Wo, Wh) <= 0.0f)
			Pdf = 0.0f;

		return Pdf;
	}
	
	/*! Distribution
		@param[in] Wh Half angle vector
		@return Distribution
	*/
	HOST_DEVICE float D(const Vec3f& Wh) const
	{
		float CosThetaH = AbsCosTheta(Wh);
		return (this->Exponent + 2) * INV_TWO_PI_F * powf(max(0.0f, CosThetaH), this->Exponent);
	}
	
	/*! Assignment operator
		@param[in] Other Blinn to copy
		@return Blinn
	*/
	HOST_DEVICE Blinn& operator = (const Blinn& Other)
	{
		this->Exponent = Other.Exponent;

		return *this;
	}

	float	Exponent;		/*! Blinn exponent */
};


}
