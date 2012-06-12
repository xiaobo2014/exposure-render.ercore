/*
	Copyright (c) 2011, T. Kroes <t.kroes@tudelft.nl>
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	- Neither the name of the TU Delft nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "Intersection.h"
#include "sample.h"

namespace ExposureRender
{

class EXPOSURE_RENDER_DLL Plane
{	
public:
	HOST_DEVICE Plane() :
		Size(1.0f),
		OneSided(false)
	{
	}

	HOST_DEVICE Plane(const Vec2f& Size, const bool& OneSided) :
		Size(Size),
		OneSided(OneSided)
	{
	}

	HOST_DEVICE Plane& operator = (const Plane& Other)
	{
		this->Size		= Other.Size;
		this->OneSided	= Other.OneSided;

		return *this;
	}

	HOST_DEVICE bool Intersects(const Ray& R) const
	{	
		Intersection Int;

		if (fabs(R.O[2] - R.D[2]) < RAY_EPS)
			return false;

		Int.T = (0.0f - R.O[2]) / R.D[2];
		
		if (Int.T < R.MinT || Int.T > R.MaxT)
			return false;

		Int.UV = Vec2f(Int.P[0], Int.P[1]);

		if (Int.UV[0] < -0.5f * this->Size[0] || Int.UV[0] > 0.5f * this->Size[0] || Int.UV[1] < -0.5f * this->Size[1] || Int.UV[1] > 0.5f * this->Size[1])
			return false;
		
		return true;
	}

	HOST_DEVICE bool Intersect(const Ray& R, Intersection& Int) const
	{
		if (fabs(R.O[2] - R.D[2]) < RAY_EPS)
			return false;

		Int.T = (0.0f - R.O[2]) / R.D[2];
		
		if (Int.T < R.MinT || Int.T > R.MaxT)
			return false;

		Int.P 		= R(Int.T);
		Int.UV		= Vec2f(Int.P[0], Int.P[1]);
		Int.N		= this->OneSided && R.D[2] >= 0.0f ? Vec3f(0.0f, 0.0f, -1.0f) : Vec3f(0.0f, 0.0f, 1.0f);
		Int.Front	= this->OneSided && R.D[2] >= 0.0f ? false : true;

		if (Int.UV[0] < -0.5f * this->Size[0] || Int.UV[0] > 0.5f * this->Size[0] || Int.UV[1] < -0.5f * this->Size[1] || Int.UV[1] > 0.5f * this->Size[1])
			return false;

		Int.UV[0] /= this->Size[0];
		Int.UV[1] /= this->Size[1];

		Int.UV += Vec2f(0.5f);
		Int.UV[0] = 1.0f - Int.UV[0];

		return true;
	}

	HOST_DEVICE void Sample(SurfaceSample& SS, const Vec3f& UVW) const
	{
		SS.P 	= Vec3f(-0.5f + UVW[0], -0.5f + UVW[1], 0.0f);
		SS.N 	= Vec3f(0.0f, 0.0f, 1.0f);
		SS.UV	= Vec2f(UVW[0], UVW[1]);

		SS.P *= Vec3f(this->Size[0], this->Size[1], 0.0f);
	}

	HOST_DEVICE float GetArea() const
	{
		return this->Size[0] * this->Size[1];
	}

	HOST_DEVICE bool GetOneSided() const
	{
		return this->OneSided;
	}

	HOST_DEVICE bool Inside(const Vec3f& P) const
	{
		return P[2] < 0.0f;
	}

	HOST_DEVICE void ClipRange(const Ray& R, Vec2f& Range) const
	{
		Intersection Int;

		
	}

protected:
	Vec2f	Size;
	bool	OneSided;
};

}
