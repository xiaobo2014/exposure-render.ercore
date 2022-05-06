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

#include "shapes.h"
#include "alignment.h"

namespace ExposureRender
{

/*! Shape class */
class EXPOSURE_RENDER_DLL Shape
{
public:
	/*! Default constructor */
	HOST_DEVICE Shape() :
		Type(Enums::Plane),
        plane(),
        disk(),
        ring(),
        sphere(),
        box(),
        alignment(),
        transform(),
		Area(0.0f)
	{
	}
	
	/*! Copy constructor
		@param[in] Other Shape to copy
	*/
	HOST_DEVICE Shape(const Shape& Other)
	{
		*this = Other;
	}
	
	/*! Assignment operator
		@param[in] Other Shape to copy
		@return Copied shape
	*/
	HOST_DEVICE Shape& operator = (const Shape& Other)
	{
		this->Type			= Other.Type;
        this->plane			= Other.plane;
        this->disk			= Other.disk;
        this->ring			= Other.ring;
        this->sphere		= Other.sphere;
        this->box			= Other.box;
        this->alignment		= Other.alignment;
        this->transform		= Other.transform;
		this->Area			= Other.Area;

		this->Update();

		return *this;
	}
	
	/*! Update the shape internally */
	HOST_DEVICE void Update()
	{
		switch (this->Type)
		{
            case Enums::Plane:		this->Area = plane.GetArea();		break;
            case Enums::Disk:		this->Area = disk.GetArea();		break;
            case Enums::Ring:		this->Area = ring.GetArea();		break;
            case Enums::Box:		this->Area = box.GetArea();			break;
            case Enums::Sphere:		this->Area = sphere.GetArea();		break;
//			case Enums::Cylinder:	this->Area = Cylinder.GetArea();	break;
		}

        this->transform = this->alignment.GetTransform();
	}
	
	/*! Test whether ray \a R intersects the shape
		@param[in] R Ray
		@return If \a R intersects the shape
	*/
	HOST_DEVICE bool Intersects(const Ray& R) const
	{
        const Ray LocalShapeR = TransformRay(this->transform.InvTM, R);

		switch (this->Type)
		{
            case Enums::Plane:		return plane.Intersects(LocalShapeR);
            case Enums::Disk:		return disk.Intersects(LocalShapeR);
            case Enums::Ring:		return ring.Intersects(LocalShapeR);
            case Enums::Box:		return box.Intersects(LocalShapeR);
            case Enums::Sphere:		return sphere.Intersects(LocalShapeR);
//			case Enums::Cylinder:	return Plane.Intersects(LocalShapeR);
		}

		return false;
	}
	
	/*! Intersect the shape with ray \a R and store the result in \a Int
		@param[in] R Ray
		@param[out] Int Resulting intersection
		@return If \a R intersects the shape
	*/
	HOST_DEVICE bool Intersect(const Ray& R, Intersection& Int) const
	{
        const Ray LocalShapeR = TransformRay(this->transform.InvTM, R);

		bool Intersects = false;

		switch (this->Type)
		{
            case Enums::Plane:		Intersects = plane.Intersect(LocalShapeR, Int);		break;
            case Enums::Disk:		Intersects = disk.Intersect(LocalShapeR, Int);		break;
            case Enums::Ring:		Intersects = ring.Intersect(LocalShapeR, Int);		break;
            case Enums::Box:		Intersects = box.Intersect(LocalShapeR, Int);		break;
            case Enums::Sphere:		Intersects = sphere.Intersect(LocalShapeR, Int);		break;
//			case Enums::Cylinder:	Intersects = Plane.Intersect(LocalShapeR, Int);		break;
		}

		if (Intersects)
		{
			Int.SetValid(true);
            Int.SetP(TransformPoint(this->transform.TM, Int.GetP()));
            Int.SetN(TransformVector(this->transform.TM, Int.GetN()));
			Int.SetT(Length(Int.GetP(), R.O));
		}
		
		return Intersects;
	}
	
	/*! Samples the shape
		@param[out] SS Resulting surface sample
		@param[in] UVW Random sample
	*/
	HOST_DEVICE void Sample(SurfaceSample& SS, const Vec3f& UVW) const
	{
		switch (this->Type)
		{
            case Enums::Plane:		plane.Sample(SS, UVW);		break;
            case Enums::Disk:		disk.Sample(SS, UVW);		break;
            case Enums::Ring:		ring.Sample(SS, UVW);		break;
            case Enums::Box:		box.Sample(SS, UVW);		break;
            case Enums::Sphere:		sphere.Sample(SS, UVW);		break;
//			case Enums::Cylinder:	Cylinder.Sample(SS, UVW);	break;
		}

        SS.P = TransformPoint(this->transform.TM, SS.P);
        SS.N = TransformVector(this->transform.TM, SS.N);
	}
	
	/*! Returns if the shape is one sided or not
		@return If the shape is one sided
	*/
	HOST_DEVICE bool GetOneSided() const
	{
		switch (this->Type)
		{
            case Enums::Plane:		return this->plane.GetOneSided();
            case Enums::Disk:		return this->disk.GetOneSided();
            case Enums::Ring:		return this->ring.GetOneSided();
            case Enums::Box:		return this->box.GetOneSided();
            case Enums::Sphere:		return this->sphere.GetOneSided();
//			case Enums::Cylinder:	return this->Cylinder.GetOneSided();
		}

		return false;
	}
	
	/*! Test whether point \a P is inside the shape
		@return If \a P is inside the shape
	*/
	HOST_DEVICE bool Inside(const Vec3f& P) const
	{
        const Vec3f LocalP = TransformPoint(this->transform.InvTM, P);

		switch (this->Type)
		{
            case Enums::Plane:		return this->plane.Inside(LocalP);
            case Enums::Disk:		return this->disk.Inside(LocalP);
            case Enums::Ring:		return this->ring.Inside(LocalP);
            case Enums::Box:		return this->box.Inside(LocalP);
            case Enums::Sphere:		return this->sphere.Inside(LocalP);
//			case Enums::Cylinder:	return this->Cylinder.Inside(LocalP);
		}

		return false;
	}
	
	GET_SET_MACRO(HOST_DEVICE, Type, Enums::ShapeType)
    GET_REF_SET_MACRO(HOST_DEVICE, plane, Plane)
    GET_REF_SET_MACRO(HOST_DEVICE, disk, Disk)
    GET_REF_SET_MACRO(HOST_DEVICE, ring, Ring)
    GET_REF_SET_MACRO(HOST_DEVICE, sphere, Sphere)
    GET_REF_SET_MACRO(HOST_DEVICE, box, Box)
    GET_REF_MACRO(HOST_DEVICE, alignment, Alignment)
	GET_MACRO(HOST_DEVICE, Area, float)

protected:
	Enums::ShapeType	Type;			/*! Type of active shape */	
    Plane				plane;			/*! Plane shape */
    Disk				disk;			/*! Disk shape */
    Ring				ring;			/*! Ring shape */
    Sphere				sphere;			/*! Sphere shape */
    Box					box;			/*! Box shape */
    Alignment			alignment;		/*! Shape alignment */
    Transform			transform;		/*! Shape transform */
	float				Area;			/*! Area of the shape */
};

}
