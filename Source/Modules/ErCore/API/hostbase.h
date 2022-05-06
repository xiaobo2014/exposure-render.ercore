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
#include "exception.h"
#include "timestamp.h"

namespace ExposureRender
{

/*! Exposure Render base class */
class EXPOSURE_RENDER_DLL HostBase : public TimeStamp
{
public:
	/*! Default constructor */
	HOST HostBase() :
		TimeStamp(),
		ID(-1),
		Enabled(true)
	{
	}
	
	/*! Destructor */
	HOST virtual ~HostBase()
	{
	}
	
	/*! Copy constructor
		@param[in] Other Bindable to copy
	*/
	HOST HostBase(const HostBase& Other) :
		TimeStamp(),
		ID(-1),
		Enabled(true)
	{
		*this = Other;
	}
	
	/*! Assignment operator
		@param[in] Other Bindable to copy
		@return Copied bindable
	*/
	HOST HostBase& operator = (const HostBase& Other)
	{
		TimeStamp::operator = (Other);

		this->ID		= Other.ID;
		this->Enabled	= Other.Enabled;

		return *this;
	}

	GET_SET_MACRO(HOST_DEVICE, Enabled, bool)

	mutable int		ID;				/*! ID of the bindable */
	
protected:
	bool			Enabled;		/*! Whether enabled or not */
};

}
