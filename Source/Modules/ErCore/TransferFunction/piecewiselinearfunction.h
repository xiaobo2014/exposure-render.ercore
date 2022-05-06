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

#include "piecewisefunction.h"
#include "buffer1d.h"

namespace ExposureRender
{

/*! Piecewise linear function base template class */
template<class T, int Size = 64>
class EXPOSURE_RENDER_DLL PiecewiseLinearFunction : public PiecewiseFunction<T, Size>
{
public:
	/*! Constructor
		@param[in] Name Name
	*/
	HOST_DEVICE PiecewiseLinearFunction(const char* Name = "Untitled") :
		PiecewiseFunction<T, Size>(Name)
	{
	}
	
	/*! Destructor */
	HOST_DEVICE virtual ~PiecewiseLinearFunction()
	{
	}
	
	/*! Copy constructor
		@param[in] Other Piecewise linear function to copy
	*/
	HOST_DEVICE PiecewiseLinearFunction(const PiecewiseLinearFunction& Other)
	{
		*this = Other;
	}
	
	/*! Assignment operator
		@param[in] Other Piecewise linear function to copy
		@return Reference to piecewise linear function
	*/
	HOST_DEVICE PiecewiseLinearFunction& operator = (const PiecewiseLinearFunction& Other)
	{
		TimeStamp::operator = (Other);

		PiecewiseFunction<T, Size>::operator = (Other);

		return *this;
	}
	
	/*! Adds a node with \a Position and \a Value
		@param[in] Node Piecewise linear functio node
	*/
	HOST_DEVICE void AddNode(const PiecewiseFunctionNode<T>& Node)
	{
		if (this->Count + 1 >= MAX_NO_TF_NODES)
			return;

		this->Nodes[this->Count] = Node;

		if (Node.GetPosition() < this->NodeRange[0])
			this->NodeRange[0] = Node.GetPosition();

		if (Node.GetPosition() > this->NodeRange[1])
			this->NodeRange[1] = Node.GetPosition();

		this->Count++;

		TimeStamp::Modified();
	}

	/*! Adds a node with \a Position and \a Value
		@param[in] Position Position of the node
		@param[in] Value Value of the node
	*/
	HOST_DEVICE void AddNode(const float& Position, const T& Value)
	{
		this->AddNode(PiecewiseFunctionNode<T>(Position, Value));
	}
	
	/*! Resets the content of the piecewise linear function */
	HOST_DEVICE void Reset()
	{
		PiecewiseFunction<T, Size>::Reset();

		TimeStamp::Modified();
	}
	
	/*! Evaluate the piecewise linear function at \a Position
		@param[in] Position Position at which to evaluate the piecewise linear function
		@return Linearly interpolated value at \a Position
	*/
	HOST_DEVICE T Evaluate(const float& Position) const
	{
		if (this->Count <= 0)
			return T();

		if (Position < this->NodeRange[0])
			return this->Nodes[0].GetValue();

		if (Position > this->NodeRange[1])
			return this->Nodes[this->Count - 1].GetValue();

		for (int i = 1; i < this->Count; i++)
		{
			const float P1 = this->Nodes[i - 1].GetPosition();
			const float P2 = this->Nodes[i].GetPosition();
			const float DeltaP = P2 - P1;
			const float LerpT = DeltaP <= 0.0f ? 0.5f : (Position - P1) / DeltaP;

			if (Position >= P1 && Position < P2)
				return this->Nodes[i - 1].GetValue() + LerpT * (this->Nodes[i].GetValue() - this->Nodes[i - 1].GetValue());
		}

		return T();
	}
};

}
