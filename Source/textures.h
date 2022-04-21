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

#include "texture.h"

namespace ExposureRender
{

/*! Evaluates texture with \a ID
	@param[in] ID ID of the texture to evaluate
	@param[in] UV UV coordinates
	@return Evaluated color
*/
HOST_DEVICE ColorXYZf EvaluateTexture(Texture* pTexture, const int& ID, const Vec2f& UV)
{
	if (ID < 0)
		return ColorXYZf::Black();

	//const Texture& T = gpTextures[ID];

	ColorXYZf L;

	Vec2f TextureUV = UV;

	TextureUV[0] *= pTexture->Repeat[0];
	TextureUV[1] *= pTexture->Repeat[1];
	
	TextureUV[0] += pTexture->Offset[0];
	TextureUV[1] += 1.0f - pTexture->Offset[1];
	
	TextureUV[0] = TextureUV[0] - floorf(TextureUV[0]);
	TextureUV[1] = TextureUV[1] - floorf(TextureUV[1]);

	TextureUV[0] = Clamp(TextureUV[0], 0.0f, 1.0f);
	TextureUV[1] = Clamp(TextureUV[1], 0.0f, 1.0f);

	if (pTexture->Flip[0])
		TextureUV[0] = 1.0f - TextureUV[0];

	if (pTexture->Flip[1])
		TextureUV[1] = 1.0f - TextureUV[1];

	switch (pTexture->Type)
	{
		case Enums::Procedural:
		{
			L = pTexture->Procedural.Evaluate(TextureUV);
			break;
		}

		case Enums::Bitmap:
		{
			if (pTexture->BitmapID >= 0)
				L = ColorXYZf::FromRGBAuc((gpBitmaps[pTexture->BitmapID].GetPixels())(TextureUV, true).D);

			break;
		}
	}

	return pTexture->OutputLevel * L;
}

}
