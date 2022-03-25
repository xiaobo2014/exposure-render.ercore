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

#include "hysteresis.h"

namespace ExposureRender
{

/*! Timing class */
class EXPOSURE_RENDER_DLL Statistic
{
public:
	/*! Default constructor */
	HOST Statistic()
	{
		this->Reset();
	}

	/*! Copy constructor
		@param[in] Other Timing to copy
	*/
	HOST Statistic(const Statistic& Other)
	{
		*this = Other;
	}
	
	/*! Assignment operator
		@param[in] Other Statistic to copy
		@return Copied statistic
	*/
	HOST Statistic& operator = (const Statistic& Other)
	{
		strcpy_s(this->Name, MAX_CHAR_SIZE, Other.Name);
		strcpy_s(this->ValueFormat, MAX_CHAR_SIZE, Other.ValueFormat);
		strcpy_s(this->Unit, MAX_CHAR_SIZE, Other.Unit);
		
		this->Hysteresis = Other.Hysteresis;

		return *this;
	}
	
	/*! Adds a value 
		@param[in] Value Value
		@param[in] Name Name
		@param[in] ValueFormat ValueFormat
		@param[in] Unit Unit
	*/
	HOST void AddValue(const float& Value, const char* Name, const char* ValueFormat, const char* Unit)
	{
		this->Hysteresis.AddValue(Value);

		strcpy_s(this->Name, MAX_CHAR_SIZE, Name);
		strcpy_s(this->ValueFormat, MAX_CHAR_SIZE, ValueFormat);
		strcpy_s(this->Unit, MAX_CHAR_SIZE, Unit);
	}

	/*! Adds a value 
		@param[in] Value Value
	*/
	HOST void AddValue(const float& Value)
	{
		this->Hysteresis.AddValue(Value);
	}

	/*! Gets the name string
		@return Name string
	*/
	HOST const char* GetName() const
	{
		return this->Name;
	}
	
	/*! Sets the name string
		@param[in] Name string
	*/
	HOST void SetName(const char* Name)
	{
		sprintf_s(this->Name, MAX_CHAR_SIZE, Name);
	}
	
	/*! Gets the value format string
		@return Value format string
	*/
	HOST const char* GetValueFormat() const
	{
		return this->ValueFormat;
	}
	
	/*! Sets the value format string
		@param[in] Value format string
	*/
	HOST void SetValueFormat(const char* ValueFormat)
	{
		sprintf_s(this->ValueFormat, MAX_CHAR_SIZE, ValueFormat);
	}
	
	/*! Gets the unit string
		@return Unit string
	*/
	HOST const char* GetUnit() const
	{
		return this->Unit;
	}
	
	/*! Sets the unit string
		@param[in] Unit string
	*/
	HOST void SetUnit(const char* Unit)
	{
		sprintf_s(this->Unit, MAX_CHAR_SIZE, Unit);
	}

	/*! Resets the statistic */
	HOST void Reset()
	{
		std::string tempName = "Untitled";
		this->SetName(tempName.c_str());

		std::string tempValueFormat = "%.2f";
		this->SetValueFormat(tempValueFormat.c_str());

		std::string tempUnit = "no unit";
		this->SetUnit(tempUnit.c_str());

		this->Hysteresis.Reset();
	}

	/*! Gets the filtered value
		@return Filtered value
	*/
	HOST float GetValue() const
	{
		return this->Hysteresis.GetFilteredValue();
	}

protected:
	char						Name[MAX_CHAR_SIZE];				/*! Name string */
	char						ValueFormat[MAX_CHAR_SIZE];			/*! Value format */
	char						Unit[MAX_CHAR_SIZE];				/*! Unit */
	Hysteresis<float, 64>		Hysteresis;							/*! Hysteresis */
};

}
