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
#include "volume.h"
#include "transferfunction.h"

namespace ExposureRender
{

/*! Intersects the volume with a ray
	@param[in] R Ray
	@param[in] RNG Random number generator
	@param[out] Int Intersection result
	@param[in] VolumeID ID of the volume
*/
DEVICE void IntersectVolume(Tracer* pTracer, Volume* pVolume, Object* pLight, Ray R, RNG& RNG, Intersection& Int, const int& VolumeID = 0)
{
	//Volume& Volume = gpVolumes[gpTracer->VolumeIDs[VolumeID]];

	if (!pVolume->BoundingBox.Intersect(R, R.MinT, R.MaxT))
		return;

	const float S	= -log(RNG.Get1()) / gDensityScale;
	float Sum		= 0.0f;

	R.MinT += RNG.Get1() * gStepFactorPrimary;

	while (Sum < S)
	{
		if (R.MinT + gStepFactorPrimary >= R.MaxT)
			return;
		
		Int.SetP(R(R.MinT));
		Int.SetIntensity((*pVolume)(Int.GetP(), VolumeID));

		Sum				+= gDensityScale * pTracer->VolumeProperty.GetOpacity(Int.GetIntensity()) * gStepFactorPrimary;
		R.MinT			+= gStepFactorPrimary;
	}

	Int.SetValid(true);
	Int.SetWo(-R.D);
	Int.SetN(pVolume->NormalizedGradient(Int.GetP(), Enums::CentralDifferences));
	Int.SetT(R.MinT);
	Int.SetScatterType(Enums::Volume);
}

/*! Whether a scattering event happens in the volume along the ray
	@param[in] R Ray
	@param[in] RNG Random number generator
	@param[in] VolumeID ID of the volume
	@return Whether an scattering event occurs in the ray's parametric range
*/
DEVICE bool IntersectsVolume(Tracer* pTracer, Volume* pVolume, Object* pLight, Ray R, RNG& RNG, const int& VolumeID = 0)
{
	if (!pTracer->VolumeProperty.GetShadows())
		return false;

	//Volume& Volume = gpVolumes[gpTracer->VolumeIDs[VolumeID]];

	float MaxT = 0.0f;

	if (!pVolume->BoundingBox.Intersect(R, R.MinT, MaxT))
		return false;

	R.MaxT = min(R.MaxT, MaxT);

	const float S	= -log(RNG.Get1()) / gDensityScale;
	float Sum		= 0.0f;
	
	R.MinT += RNG.Get1() * gStepFactorShadow;

	while (Sum < S)
	{
		if (R.MinT > R.MaxT)
			return false;

		Sum		+= gDensityScale * pTracer->VolumeProperty.GetOpacity((*pVolume)(R(R.MinT), VolumeID)) * gStepFactorShadow;
		R.MinT	+= gStepFactorShadow;
	}

	return true;
}

}
