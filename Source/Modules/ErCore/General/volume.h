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

#include "hostvolume.h"
#include "boundingbox.h"
#include "octree.h"
#include "cudatexture3d.h"
#include "utilities.h"
#include "transform.h"

namespace ExposureRender
{

/*! Volume class */
class Volume : public TimeStamp
{
public:
	/*! Default constructor */
	HOST Volume() :
		TimeStamp(),
		Transform(),
		BoundingBox(),
		Spacing(1.0f),
		InvSpacing(1.0f),
		Size(1.0f),
		InvSize(1.0f),
		MinStep(1.0f),
		Voxels(),
		AcceleratorType(Enums::Octree),
		MaxGradientMagnitude(0.0f)
	{
	}
	
	/*! Copy constructor
		@param[in] Other Volume to copy
	*/
	HOST Volume(const HostVolume& Other) :
		TimeStamp(),
		Transform(),
		BoundingBox(),
		Spacing(1.0f),
		InvSpacing(1.0f),
		Size(1.0f),
		InvSize(1.0f),
		MinStep(1.0f),
		Voxels(),
		AcceleratorType(Enums::Octree),
		MaxGradientMagnitude(0.0f)
	{
		*this = Other;
	}
	
	/*! Assignment operator
		@param[in] Other Volume to copy
		@return Copied volume
	*/
    HOST Volume& operator = (const HostVolume& Other)
	{
		TimeStamp::operator = (Other);

        this->Transform			= Other.Getalignment().GetTransform();
		this->Voxels			= Other.Voxels;
		this->AcceleratorType	= Other.GetAcceleratorType();

		const int NoElements = this->Voxels.GetResolution().CumulativeProduct();

		if (NoElements > 0)
		{
			float Scale = 0.0f;

			if (Other.GetNormalizeSize())
			{
				const Vec3f PhysicalSize = Vec3f((float)this->Voxels.GetResolution()[0], (float)this->Voxels.GetResolution()[1], (float)this->Voxels.GetResolution()[2]) * Other.GetSpacing();
				Scale = 1.0f / max(PhysicalSize[0], max(PhysicalSize[1], PhysicalSize[2]));
			}

			this->Spacing		= Scale * Other.GetSpacing();
			this->InvSpacing	= 1.0f / this->Spacing;
			this->Size			= Vec3f((float)this->Voxels.GetResolution()[0] * this->Spacing[0], (float)this->Voxels.GetResolution()[1] *this->Spacing[1], (float)this->Voxels.GetResolution()[2] * this->Spacing[2]);
			this->InvSize		= 1.0f / this->Size;

			this->BoundingBox.SetMinP(-0.5 * Size);
			this->BoundingBox.SetMaxP(0.5f * Size);

			this->MinStep = min(this->Spacing[0], min(this->Spacing[1], this->Spacing[2]));

			this->MaxGradientMagnitude = Other.GetMaximumGradientMagnitude();
		}

		return *this;
	}
	
	/*! Gets voxel data at \a XYZ
		@param[in] XYZ Position
		@param[in] TextureID CUDA texture ID
		@return Data at \a XYZ volume
	*/
	DEVICE unsigned short operator()(const Vec3f& XYZ, const int& TextureID = 0)
	{
		const Vec3f NormalizedXYZ = (XYZ - this->BoundingBox.GetMinP()) * this->InvSize;
		
		switch (TextureID)
		{
			case 0: return (float)USHRT_MAX * tex3D(TexVolume0, NormalizedXYZ[0], NormalizedXYZ[1], NormalizedXYZ[2]); 
			case 1: return (float)USHRT_MAX * tex3D(TexVolume1, NormalizedXYZ[0], NormalizedXYZ[1], NormalizedXYZ[2]); 
		}

		return 0;
	}
	
	/*! Gets voxel data at (\a X,\a Y,\a Z)
		@param[in] X Position
		@param[in] Y Position
		@param[in] Z Position
		@return Data at \a X,\a Y,\a Z
	*/
	DEVICE unsigned short operator()(const int& X, const int& Y, const int& Z)
	{
		const Vec3f NormalizedXYZ((float)X / (float)Voxels.GetResolution()[0], (float)X / (float)Voxels.GetResolution()[1], (float)X / (float)Voxels.GetResolution()[2]);
		
		return (float)USHRT_MAX * tex3D(TexVolume0, NormalizedXYZ[0], NormalizedXYZ[1], NormalizedXYZ[2]);
	}
	
	/*! Gets the voxel data at \a P
		@param[in] P Position
		@return Data at \a XYZ volume
	*/
	DEVICE unsigned short GetIntensity(const Vec3f& P)
	{
		return (*this)(P);
	}
	
	/*! Computes the gradient at \a P using central differences
		@param[in] P Position at which to compute the gradient
		@return Gradient at \a P
	*/
	DEVICE Vec3f GradientCD(const Vec3f& P)
	{
		const float Intensity[3][2] = 
		{
			{ GetIntensity(P + Vec3f(this->Spacing[0], 0.0f, 0.0f)), GetIntensity(P - Vec3f(this->Spacing[0], 0.0f, 0.0f)) },
			{ GetIntensity(P + Vec3f(0.0f, this->Spacing[1], 0.0f)), GetIntensity(P - Vec3f(0.0f, this->Spacing[1], 0.0f)) },
			{ GetIntensity(P + Vec3f(0.0f, 0.0f, this->Spacing[2])), GetIntensity(P - Vec3f(0.0f, 0.0f, this->Spacing[2])) }
		};

		return Vec3f(Intensity[0][1] - Intensity[0][0], Intensity[1][1] - Intensity[1][0], Intensity[2][1] - Intensity[2][0]);
	}
	
	/*! Computes the gradient at \a P using forward differences
		@param[in] P Position at which to compute the gradient
		@return Gradient at \a P
	*/
	DEVICE Vec3f GradientFD(const Vec3f& P)
	{
		const float Intensity[4] = 
		{
			GetIntensity(P),
			GetIntensity(P + Vec3f(this->Spacing[0], 0.0f, 0.0f)),
			GetIntensity(P + Vec3f(0.0f, this->Spacing[1], 0.0f)),
			GetIntensity(P + Vec3f(0.0f, 0.0f, this->Spacing[2]))
		};

		return Vec3f(Intensity[0] - Intensity[1], Intensity[0] - Intensity[2], Intensity[0] - Intensity[3]);
	}
	
	/*! Computes the filtered gradient at \a P using central differences
		@param[in] P Position at which to compute the gradient
		@return Gradient at \a P
	*/
	DEVICE Vec3f GradientFiltered(const Vec3f& P)
	{
		Vec3f Offset(Vec3f(this->Spacing[0], 0.0f, 0.0f)[0], Vec3f(0.0f, this->Spacing[1], 0.0f)[1], Vec3f(0.0f, 0.0f, this->Spacing[2])[2]);

		const Vec3f G0 = GradientCD(P);
		const Vec3f G1 = GradientCD(P + Vec3f(-Offset[0], -Offset[1], -Offset[2]));
		const Vec3f G2 = GradientCD(P + Vec3f( Offset[0],  Offset[1],  Offset[2]));
		const Vec3f G3 = GradientCD(P + Vec3f(-Offset[0],  Offset[1], -Offset[2]));
		const Vec3f G4 = GradientCD(P + Vec3f( Offset[0], -Offset[1],  Offset[2]));
		const Vec3f G5 = GradientCD(P + Vec3f(-Offset[0], -Offset[1],  Offset[2]));
		const Vec3f G6 = GradientCD(P + Vec3f( Offset[0],  Offset[1], -Offset[2]));
		const Vec3f G7 = GradientCD(P + Vec3f(-Offset[0],  Offset[1],  Offset[2]));
		const Vec3f G8 = GradientCD(P + Vec3f( Offset[0], -Offset[1], -Offset[2]));
	    
		const Vec3f L0 = Lerp(0.5f, Lerp(0.5f, G1, G2), Lerp(0.5f, G3, G4));
		const Vec3f L1 = Lerp(0.5f, Lerp(0.5f, G5, G6), Lerp(0.5f, G7, G8));
	    
		return Lerp(0.75f, G0, Lerp(0.5f, L0, L1));
	}
	
	/*! Computes the gradient at \a P using \a GradientMode
		@param[in] P Position at which to compute the gradient
		@param[in] GradientMode Type of gradient computation
		@return Gradient at \a P
	*/
	DEVICE Vec3f Gradient(const Vec3f& P, const Enums::GradientMode& GradientMode)
	{
		switch (GradientMode)
		{
			case Enums::ForwardDifferences:		return GradientFD(P);
			case Enums::CentralDifferences:		return GradientCD(P);
			case Enums::Filtered:				return GradientFiltered(P);
		}

		return GradientFD(P);
	}
	
	/*! Computes the normalized gradient at \a P using \a GradientMode
		@param[in] P Position at which to compute the gradient
		@param[in] GradientMode Type of gradient computation
		@return Gradient at \a P
	*/
	DEVICE Vec3f NormalizedGradient(const Vec3f& P, const Enums::GradientMode& GradientMode)
	{
		return Normalize(Gradient(P, GradientMode));
	}
	
	/*! Computes the gradient magnitude at \a P
		@param[in] P Position at which to compute the gradient magnitude
		@return Gradient magnitude at \a P
	*/
	DEVICE float GradientMagnitude(const Vec3f& P)
	{
		const Vec3f HalfSpacing = 0.5f / this->Spacing;

		float D = 0.0f, Sum = 0.0f;

		D = (GetIntensity(P + Vec3f(this->Spacing[0], 0.0f, 0.0f)) - GetIntensity(P - Vec3f(this->Spacing[0], 0.0f, 0.0f))) * 0.5f;
		Sum += D * D;

		D = (GetIntensity(P + Vec3f(0.0f, this->Spacing[1], 0.0f)) - GetIntensity(P - Vec3f(0.0f, this->Spacing[1], 0.0f))) * 0.5f;
		Sum += D * D;

		D = (GetIntensity(P + Vec3f(0.0f, 0.0f, this->Spacing[2])) - GetIntensity(P - Vec3f(0.0f, 0.0f, this->Spacing[2]))) * 0.5f;
		Sum += D * D;

		return sqrtf(Sum);
	}
	
	Transform						Transform;					/*! Transform of the volume */
	BoundingBox						BoundingBox;				/*! Encompassing bounding box */
	Vec3f							Spacing;					/*! Voxel spacing */
	Vec3f							InvSpacing;					/*! Inverse voxel spacing */
	Vec3f							Size;						/*! Volume size */
	Vec3f							InvSize;					/*! Inverse volume size */
	float							MinStep;					/*! Minimum step size */
	CudaTexture3D<unsigned short>	Voxels;						/*! Voxel 3D buffer */
	Enums::AcceleratorType			AcceleratorType;			/*! Type of ray traversal accelerator */
	float							MaxGradientMagnitude;		/*! Maximum gradient magnitude */
};

}
