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

#include "defines.h"
#include "enums.h"

namespace ExposureRender
{

/*! Exception class */
class EXPOSURE_RENDER_DLL Exception
{
public:
	/*! Constructor
		@param[in] Level Level of the exception
		@param[in] Message Informative message to display to user
	*/
	HOST Exception(const Enums::ExceptionLevel& Level, const char* Message = "")
	{
		this->Level = Level;
        //sprintf_s(this->Message, MAX_CHAR_SIZE, "%s", Message);
	}
	
	/*! Destructor */
	HOST virtual ~Exception()
	{
	}
	
	/*! Copy constructor
		@param[in] Other Exception to copy
	*/
	HOST Exception(const Exception& Other)
	{
		*this = Other;
	}
	
	/*! Assignment operator
		@param[in] Other Exception to copy
		@return Exception
	*/
	HOST Exception& operator = (const Exception& Other)
	{
		this->Level = Other.Level;
        //sprintf(this->Message, MAX_CHAR_SIZE, "%s", Other.Message);

		return *this;
	}
	
	/*! Gets the exception message */
	HOST const char* GetMessage() const
	{
		return &(this->Message[0]);
	}
	
	GET_SET_MACRO(HOST, Level, Enums::ExceptionLevel)
	
protected:
	Enums::ExceptionLevel	Level;						/*! Level of the exception */
	char					Message[MAX_CHAR_SIZE];		/*! Exception message */
};

}
