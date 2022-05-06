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

namespace ExposureRender
{

namespace Cuda
{

template<typename D, typename H, int MaxSize = 32>
class List
{
public:
	HOST List() :
		Map(),
		MapIt(),
		HashMap(),
		HashMapIt(),
		DeviceList(NULL),
		Counter(0)
	{
        //sprintf_s(DeviceSymbol, MAX_CHAR_SIZE, "%s", pDeviceSymbol);

        //case 1:
        //DeviceSymbol = pDeviceSymbol;

        //allocate
        //int VolumeNum = 1;        
        //Cuda::Allocate(DeviceSymbol, VolumeNum);

        //case 2:
        //pDeviceSymbol = &DeviceSymbol;
        //m_pDeviceSymbol = pDeviceSymbol;

        //case 3:
        //m_pDeviceSymbol = pDeviceSymbol;
	}

    HOST ~List()
    {
        //Cuda::Free(this->DeviceSymbol);
    }

	HOST bool Exists(const int& ID)
	{
		if (ID < 0)
			return false;

		this->MapIt = this->Map.find(ID);

		return this->MapIt != this->Map.end();
	}

	HOST D* Bind(const H& Item)
	{
		if (this->Map.size() + 1 >= MaxSize)
		{
			DebugLog("%s failed, max. no. items reached", __FUNCTION__);
			return NULL;
		}
		
		const bool Exists = this->Exists(Item.ID);
		
		if (!Exists)
		{
			Item.ID = this->Counter;
			this->Map[Item.ID] = new D(Item);
			this->Counter++;
		}
		else
		{
			*(this->Map[Item.ID]) = Item;
		}

		this->Synchronize();

		return this->Map[Item.ID];
	}

	HOST void Unbind(const H& Item)
	{
		if (!this->Exists(Item.ID))
		{
			DebugLog("%s failed, resource item with ID:%d does not exist", __FUNCTION__, Item.ID);
			return;
		}
		
		delete this->Map[Item.ID];
				
		this->MapIt = this->Map.find(Item.ID);
		this->Map.erase(this->MapIt);

		this->HashMapIt = this->HashMap.find(Item.ID);

		if (this->HashMapIt != this->HashMap.end())
			this->HashMap.erase(this->HashMapIt);

		this->Synchronize();
	}

	HOST void Synchronize(const int& ID = 0)
	{
		if (this->Map.size() <= 0)
			return;

		if (ID == 0)
		{
			D* pHostList = (D*)malloc(this->Map.size() * sizeof(D));
		
			int Size = 0;

			for (this->MapIt = this->Map.begin(); this->MapIt != this->Map.end(); this->MapIt++)
			{
				memcpy((void*)&pHostList[Size], (void*)this->MapIt->second, sizeof(D));
				HashMap[this->MapIt->first] = Size;
				Size++;
			}
			
            /*
			Cuda::Free(this->DeviceList);
			Cuda::Allocate(this->DeviceList, (int)this->Map.size());
			Cuda::MemCopyHostToDevice(pHostList, this->DeviceList, Size);
            */
            //m_pDeviceSymbol = this->DeviceList;

			//Cuda::MemCopyHostToDeviceSymbol(&this->DeviceList, this->DeviceSymbol);//origin , error
            
            // test on when DeviceSymbol is a char string
            //Cuda::MemCopyHostToDeviceSymbol(this->DeviceList, this->DeviceSymbol);// error

            //Cuda::MemCopyHostToDeviceSymbol(pHostList, this->DeviceSymbol);//error

            //Cuda::MemCopyDeviceToDeviceSymbol(this->DeviceList, this->DeviceSymbol);

            //Cuda::MemCopyDeviceToDeviceSymbol(&this->DeviceList, this->DeviceSymbol);


            // test on when DeviceSymbol is a D pointer
            //Cuda::Free(this->DeviceSymbol);
            //int VolumeNum = 1;
            //Cuda::Allocate(this->DeviceSymbol, VolumeNum);

            //case 1:
            //Cuda::MemCopyDeviceToDevice(this->DeviceList, this->DeviceSymbol);

            //case 2:
            //Cuda::MemCopyDeviceToDevice(this->DeviceList, &this->DeviceSymbol);//error

            //Cuda::MemCopyDeviceToDevice(this->DeviceList, this->m_pDeviceSymbol);
		
			free(pHostList);
		}
		else
		{
			if (!this->Exists(ID))
				return;

			Cuda::Free(this->DeviceList);
			Cuda::Allocate(this->DeviceList);
			
			this->MapIt = this->Map.find(ID);

			Cuda::MemCopyHostToDevice(this->MapIt->second, this->DeviceList);
            //Cuda::MemCopyHostToDeviceSymbol(&this->DeviceList, this->DeviceSymbol);
            
            // test on when DeviceSymbol is a D pointer
            //Cuda::MemCopyDeviceToDevice(this->DeviceList, this->DeviceSymbol);

		}
	}

	HOST D& operator[](const int& i)
	{
		if (!this->Exists(i))
		{
			char Message[MAX_CHAR_SIZE];

                        //sprintf_s(Message, MAX_CHAR_SIZE, "%s failed, resource item with ID:%d does not exist", __FUNCTION__, i);

			throw(Exception(Enums::Warning, Message));
		}

		return *this->Map[i];
	}

	map<int, D*>						Map;
	typename map<int, D*>::iterator		MapIt;
	map<int, int>						HashMap;
	typename map<int, int>::iterator	HashMapIt;
	D*									DeviceList;
	int									Counter;
	D*								    DeviceSymbol;
    //D*								    m_pDeviceSymbol;
};

}

}
