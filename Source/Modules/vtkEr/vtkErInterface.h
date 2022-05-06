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



#include "vtkErVolume.h"
#include "vtkErTexture.h"
#include "vtkErTracer.h"
#include "vtkErCamera.h"
#include "vtkErObject.h"
#include "vtkErBitmap.h"
#include "vtkErTimerCallback.h"
#include "vtkErVolumeProperty.h"
#include "vtkErInteractorStyleTrackballCamera.h"

class vtkRenderer;

// Specify volume file her (*.mhd)
//char gVolumeFile[] = "C:\\Dropbox\\Work\\Data\\Volumes\\engine.mhd";
//char gVolumeFile[] = "D:\\exposure-download\\volumes\\engine.mhd";

//windows
//char gVolumeFile[] = "D:\\DataTransfer\\aorta_coronary_chamber_image\\CHEN_MEIRONG.mhd";


char gVolumeFile[] = "/home/liuxiaobo/DataTransfer/aorta_coronary_chamber_image/CHEN_MEIRONG.mhd";

//#define BACK_PLANE_ON
#define KEY_LIGHT_ON
//#define RIM_LIGHT_ON
//#define ENVIRONMENT_ON

#ifdef BACK_PLANE_ON
	char gBackPlaneBitmap[] = "C:\\Dropbox\\Work\\Data\\Bitmaps\\back_plane.png";
#endif

#ifdef ENVIRONMENT_ON
	char gEnvironmentBitmap[] = "C:\\Dropbox\\Work\\Data\\Bitmaps\\environment.png";
#endif

using namespace ExposureRender;

class vtkErInterface
{
public:

    static void ConfigureER(vtkRenderer* Renderer);
    static void LoadVolume(vtkErTracer* Tracer);
    static void CreateCamera(vtkRenderer* Renderer);
    static void CreateVolumeProperty(vtkErTracer* Tracer);
    static void CreateLighting(vtkErTracer* Tracer);
    static void CreateObjects(vtkErTracer* Tracer);
    static std::vector<unsigned char> OffScreenRenderDicom();
};


