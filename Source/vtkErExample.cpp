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

#include <vtkConeSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMetaImageReader.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkPNGReader.h>
#include <vtkImageCast.h>
#include <vtkCommand.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#include "vtkErVolume.h"
#include "vtkErTexture.h"
#include "vtkErTracer.h"
#include "vtkErCamera.h"
#include "vtkErObject.h"
#include "vtkErBitmap.h"
#include "vtkErTimerCallback.h"
#include "vtkErVolumeProperty.h"
#include "vtkErInteractorStyleTrackballCamera.h"

// Specify volume file her (*.mhd)
//char gVolumeFile[] = "C:\\Dropbox\\Work\\Data\\Volumes\\engine.mhd";
//char gVolumeFile[] = "D:\\exposure-download\\volumes\\engine.mhd";
char gVolumeFile[] = "D:\\DataTransfer\\aorta_coronary_chamber_image\\CHEN_MEIRONG.mhd";

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

void InteractorRender();
void OffScreenRender();
void ConfigureER(vtkRenderer* Renderer);
void LoadVolume(vtkErTracer* Tracer);
void CreateCamera(vtkRenderer* Renderer);
void CreateVolumeProperty(vtkErTracer* Tracer);
void CreateLighting(vtkErTracer* Tracer);
void CreateObjects(vtkErTracer* Tracer);

using namespace ExposureRender;

int main(int, char *[])
{
    //InteractorRender();

    OffScreenRender();

	return EXIT_SUCCESS;
}

void InteractorRender()
{
    vtkSmartPointer<vtkRenderWindow> RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();

    vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindowInteractor> RenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    RenderWindowInteractor->SetRenderWindow(RenderWindow);

    RenderWindow->AddRenderer(Renderer);

    vtkSmartPointer<vtkErTimerCallback> TimerCallback = vtkSmartPointer<vtkErTimerCallback>::New();
    TimerCallback->SetRenderWindowInteractor(RenderWindowInteractor);

    vtkSmartPointer<vtkErInteractorStyleTrackballCamera> InteractorStyle = vtkSmartPointer<vtkErInteractorStyleTrackballCamera>::New();
    InteractorStyle->SetMotionFactor(10);

    RenderWindowInteractor->Initialize();
    RenderWindowInteractor->CreateRepeatingTimer(1);
    RenderWindowInteractor->AddObserver(vtkCommand::TimerEvent, TimerCallback);
    RenderWindowInteractor->SetInteractorStyle(InteractorStyle);

    RenderWindow->Render();
    RenderWindow->SetSize(512, 512);
    RenderWindow->SetWindowName("Exposure Render - VTK wrapping example");

    ConfigureER(Renderer);

    Renderer->ResetCamera();

    RenderWindowInteractor->Start();
}

void OffScreenRender()
{
    vtkSmartPointer<vtkRenderWindow> RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();

    vtkSmartPointer<vtkRenderer> Renderer = vtkSmartPointer<vtkRenderer>::New();

    //vtkSmartPointer<vtkRenderWindowInteractor> RenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

    //RenderWindowInteractor->SetRenderWindow(RenderWindow);

    RenderWindow->AddRenderer(Renderer);

    //vtkSmartPointer<vtkErTimerCallback> TimerCallback = vtkSmartPointer<vtkErTimerCallback>::New();
    //TimerCallback->SetRenderWindowInteractor(RenderWindowInteractor);

    //vtkSmartPointer<vtkErInteractorStyleTrackballCamera> InteractorStyle = vtkSmartPointer<vtkErInteractorStyleTrackballCamera>::New();
    //InteractorStyle->SetMotionFactor(10);

    //RenderWindowInteractor->Initialize();
    //RenderWindowInteractor->CreateRepeatingTimer(1);
    //RenderWindowInteractor->AddObserver(vtkCommand::TimerEvent, TimerCallback);
    //RenderWindowInteractor->SetInteractorStyle(InteractorStyle);

    RenderWindow->SetSize(512, 512);
    RenderWindow->SetWindowName("Exposure Render - VTK wrapping example");

    ConfigureER(Renderer);

    Renderer->ResetCamera();

    RenderWindow->Render();

    //RenderWindowInteractor->Start();

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(RenderWindow);
    windowToImageFilter->Update();

    std::string pngFile = "D:\\exposure-render-ercore\\test.png";
    
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(pngFile.c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

}

void ConfigureER(vtkRenderer* Renderer)
{
	vtkSmartPointer<vtkErTracer> Tracer = vtkSmartPointer<vtkErTracer>::New();
	
	LoadVolume(Tracer);
	CreateVolumeProperty(Tracer);
	CreateLighting(Tracer);
	CreateObjects(Tracer);
	CreateCamera(Renderer);

	Tracer->SetNoiseReduction(false);
	Tracer->Update();

	vtkSmartPointer<vtkVolume> Volume = vtkSmartPointer<vtkVolume>::New();
	
	Volume->Update();
	Volume->SetMapper(Tracer);

	Renderer->AddVolume(Volume);
}

void CreateVolumeProperty(vtkErTracer* Tracer)
{
	vtkSmartPointer<vtkErVolumeProperty> VolumeProperty = vtkSmartPointer<vtkErVolumeProperty>::New();
	
	const float StepSize = 10.0f;

	VolumeProperty->SetShadows(true);
	VolumeProperty->SetStepFactorPrimary(3.0f);
	VolumeProperty->SetStepFactorShadow(3.0f);
	VolumeProperty->SetShadingMode(Enums::Hybrid);
	VolumeProperty->SetDensityScale(100);
	VolumeProperty->SetGradientFactor(3.0f);

	vtkSmartPointer<vtkPiecewiseFunction> Opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();	
	//Opacity->AddPoint(-217, 0);
	//Opacity->AddPoint(-108.75, 0.00185);
	Opacity->AddPoint(0, 0);
    Opacity->AddPoint(108, 0.1);
    Opacity->AddPoint(324, 0.467);
    Opacity->AddPoint(432, 0.712);
    Opacity->AddPoint(540, 0.916);
    Opacity->AddPoint(649, 1);	
	VolumeProperty->SetOpacity(Opacity);

	vtkSmartPointer<vtkColorTransferFunction> Diffuse = vtkSmartPointer<vtkColorTransferFunction>::New();	
	Diffuse->AddRGBPoint(177, 0.651, 0, 0);
	Diffuse->AddRGBPoint(239, 0.933, 0, 0);
    Diffuse->AddRGBPoint(302, 1, 0.8, 0.0627);
    Diffuse->AddRGBPoint(496, 1, 0.906, 0.667);
    Diffuse->AddRGBPoint(677, 1, 0.882, 0.216);
    Diffuse->AddRGBPoint(809, 1, 1, 1);
	VolumeProperty->SetDiffuse(Diffuse);

	vtkSmartPointer<vtkColorTransferFunction> Specular = vtkSmartPointer<vtkColorTransferFunction>::New();	
	const float SpecularLevel = 0.1f;
	Specular->AddRGBPoint(0, SpecularLevel, SpecularLevel, SpecularLevel);
	Specular->AddRGBPoint(2048, SpecularLevel, SpecularLevel, SpecularLevel);
	VolumeProperty->SetSpecular(Specular);
	
	vtkSmartPointer<vtkPiecewiseFunction> Glossiness = vtkSmartPointer<vtkPiecewiseFunction>::New();	
	const float GlossinessLevel = 100.0f;
	Glossiness->AddPoint(0, GlossinessLevel);
	Glossiness->AddPoint(2048, GlossinessLevel);	
	VolumeProperty->SetGlossiness(Glossiness);

	vtkSmartPointer<vtkColorTransferFunction> Emission = vtkSmartPointer<vtkColorTransferFunction>::New();
	Emission->AddRGBPoint(0, 0, 0, 0);
	Emission->AddRGBPoint(100, 0, 0, 0);
	VolumeProperty->SetEmission(Emission);

	Tracer->SetVolumeProperty(VolumeProperty);
}

void LoadVolume(vtkErTracer* Tracer)
{
	vtkSmartPointer<vtkMetaImageReader> Reader	= vtkSmartPointer<vtkMetaImageReader>::New();
	
	Reader->SetFileName(gVolumeFile);
	
	if (Reader->CanReadFile(gVolumeFile) == 0)
	{
		printf("Can't read %s, aborting!\n", gVolumeFile);
		exit(EXIT_FAILURE);
	}

	Reader->Update();

	vtkSmartPointer<vtkImageCast> ImageCast = vtkSmartPointer<vtkImageCast>::New();
	
	ImageCast->SetOutputScalarTypeToUnsignedShort();
	ImageCast->SetClampOverflow(1);
	ImageCast->SetInputConnection(0, Reader->GetOutputPort());
	ImageCast->Update();

	vtkSmartPointer<vtkErVolume> Volume	= vtkSmartPointer<vtkErVolume>::New();

	Volume->SetInputConnection(vtkErVolume::ImageDataPort, ImageCast->GetOutputPort());
	Volume->SetFilterMode(Enums::Linear);
	Volume->SetAcceleratorType(Enums::NoAcceleration);

	Tracer->AddInputConnection(vtkErTracer::VolumesPort, Volume->GetOutputPort());
}

void CreateCamera(vtkRenderer* Renderer)
{
	vtkSmartPointer<vtkErCamera> Camera = vtkSmartPointer<vtkErCamera>::New();

	Camera->SetExposure(0.75);

	// Aperture
	Camera->SetApertureShape(Enums::Polygon);
	Camera->SetApertureSize(0.0f);
	Camera->SetNoApertureBlades(3);
	Camera->SetApertureAngle(0.0f);
	Camera->SetClippingRange(0, 1000000);

    //Focus
    Camera->SetFocalDistance(0.75);
	
	// Apply camera to renderer
	Renderer->SetActiveCamera(Camera);
}

void CreateLighting(vtkErTracer* Tracer)
{
#ifdef KEY_LIGHT_ON
	vtkSmartPointer<vtkErObject> KeyLight = vtkSmartPointer<vtkErObject>::New();

	const float KeyLightSize = 4.07f;

	KeyLight->SetEmitter(true);
	KeyLight->SetAlignmentType(Enums::Spherical);
	KeyLight->SetShapeType(Enums::Plane);
	KeyLight->SetOneSided(false);
	KeyLight->SetVisible(true);
	KeyLight->SetElevation(-26.54f);
	KeyLight->SetAzimuth(-33.85f);
	KeyLight->SetOffset(2.906f);

	KeyLight->SetMultiplier(200.0f);
	KeyLight->SetSize(KeyLightSize, KeyLightSize, KeyLightSize);
	KeyLight->SetEmissionUnit(Enums::Power);
	KeyLight->SetRelativeToCamera(true);
	KeyLight->SetUseCameraFocalPoint(true);
	KeyLight->SetEnabled(true);

	Tracer->AddInputConnection(vtkErTracer::ObjectsPort, KeyLight->GetOutputPort());

	vtkSmartPointer<vtkErTexture> KeyLightTexture = vtkSmartPointer<vtkErTexture>::New();

	KeyLightTexture->SetTextureType(Enums::Procedural);
	KeyLightTexture->SetProceduralType(Enums::Uniform);
	KeyLightTexture->SetUniformColor(15000);
	
	KeyLight->SetInputConnection(vtkErObject::EmissionTexturePort, KeyLightTexture->GetOutputPort());
#endif

#ifdef RIM_LIGHT_ON
	vtkSmartPointer<vtkErObject> RimLight = vtkSmartPointer<vtkErObject>::New();

	const float RimLightSize = 0.5f;

	RimLight->SetEmitter(true);
	RimLight->SetAlignmentType(Enums::Spherical);
	RimLight->SetShapeType(Enums::Plane);
	RimLight->SetOneSided(true);
	RimLight->SetVisible(true);
	RimLight->SetElevation(15.0f);
	RimLight->SetAzimuth(-20.0f);
	RimLight->SetOffset(1.5f);
	RimLight->SetMultiplier(5.0f);
	RimLight->SetSize(RimLightSize, RimLightSize, RimLightSize);
	RimLight->SetEmissionUnit(Enums::Power);
	RimLight->SetRelativeToCamera(true);
	RimLight->SetUseCameraFocalPoint(true);
	RimLight->SetEnabled(true);

	Tracer->AddInputConnection(vtkErTracer::ObjectsPort, RimLight->GetOutputPort());

	vtkSmartPointer<vtkErTexture> RimLightTexture = vtkSmartPointer<vtkErTexture>::New();

	RimLightTexture->SetTextureType(Enums::Procedural);
	RimLightTexture->SetProceduralType(Enums::Uniform);
	RimLightTexture->SetUniformColor(15000);
	
	RimLight->SetInputConnection(vtkErObject::EmissionTexturePort, RimLightTexture->GetOutputPort());
#endif

#ifdef ENVIRONMENT_ON
	vtkSmartPointer<vtkErObject> EnvironmentLight = vtkSmartPointer<vtkErObject>::New();
	
	EnvironmentLight->SetEmitter(true);
	EnvironmentLight->SetAlignmentType(Enums::AxisAlign);
	EnvironmentLight->SetAxis(ExposureRender::Enums::Y);
	EnvironmentLight->SetPosition(0, 0, 0);
	EnvironmentLight->SetShapeType(Enums::Sphere);
	EnvironmentLight->SetOneSided(true);
	EnvironmentLight->SetRadius(5.0f);
	EnvironmentLight->SetMultiplier(5000.0f);
	EnvironmentLight->SetEmissionUnit(Enums::Lux);
	EnvironmentLight->SetEnabled(true);

	Tracer->AddInputConnection(vtkErTracer::ObjectsPort, EnvironmentLight->GetOutputPort());

	vtkSmartPointer<vtkErTexture> EnvironmentLightTexture = vtkSmartPointer<vtkErTexture>::New();

	vtkSmartPointer<vtkPNGReader> ImageReader = vtkSmartPointer<vtkPNGReader>::New();

	if (ImageReader->CanReadFile(gEnvironmentBitmap))
	{
		EnvironmentLightTexture->SetTextureType(Enums::Bitmap);

		ImageReader->SetFileName(gEnvironmentBitmap);
		ImageReader->Update();

		vtkSmartPointer<vtkImageGaussianSmooth> ImageGaussianSmooth = vtkSmartPointer<vtkImageGaussianSmooth>::New();
		
		ImageGaussianSmooth->SetInputConnection(ImageReader->GetOutputPort());
		ImageGaussianSmooth->SetRadiusFactor(2);
		ImageGaussianSmooth->SetStandardDeviation(2);
		ImageGaussianSmooth->Update();

		vtkSmartPointer<vtkErBitmap> Bitmap = vtkSmartPointer<vtkErBitmap>::New();
	
		Bitmap->SetFilterMode(Enums::Linear);
		Bitmap->SetInputConnection(vtkErBitmap::ImageDataPort, ImageGaussianSmooth->GetOutputPort());

		EnvironmentLightTexture->SetInputConnection(vtkErTexture::ImageDataPort, Bitmap->GetOutputPort());
	}
	else
	{
		printf("%s cannot be loaded, reverting to gradient background.", gEnvironmentBitmap);

		EnvironmentLightTexture->SetTextureType(Enums::Procedural);
		EnvironmentLightTexture->SetProceduralType(Enums::Uniform);

		vtkSmartPointer<vtkColorTransferFunction> Gradient = vtkSmartPointer<vtkColorTransferFunction>::New();
		
		Gradient->AddRGBPoint(0.0f, 0.0025f, 0.0025f, 0.0025f);
		Gradient->AddRGBPoint(0.5f, 0.01f, 0.01f, 0.01f);
		Gradient->AddRGBPoint(0.50001f, 0.9f, 0.8f, 0.5f);
		Gradient->AddRGBPoint(0.51f, 0.9f, 0.6f, 0.2f);
		Gradient->AddHSVPoint(1.0f, 0.6f, 0.9f, 0.8f);

		EnvironmentLightTexture->SetGradient(Gradient);
	}

	EnvironmentLight->SetInputConnection(vtkErObject::EmissionTexturePort, EnvironmentLightTexture->GetOutputPort());
#endif
}

void CreateObjects(vtkErTracer* Tracer)
{
#ifdef BACK_PLANE_ON
	vtkSmartPointer<vtkErObject> Object = vtkSmartPointer<vtkErObject>::New();

	const float BackPlaneSize = 2;

	Object->SetAlignmentType(Enums::AxisAlign);
	Object->SetAxis(Enums::Z);
	Object->SetPosition(0.0f, 0.0f, -0.6f);
	Object->SetShapeType(Enums::Plane);
	Object->SetSize(BackPlaneSize, BackPlaneSize, BackPlaneSize);
//	Object->SetRelativeToCamera(true);
	Object->SetUseCameraFocalPoint(true);
	Object->SetElevation(0.0f);
	Object->SetAzimuth(0.0f);
	Object->SetOffset(0.0f);
	Object->SetOneSided(true);
	Object->SetEnabled(true);

	vtkSmartPointer<vtkErTexture> DiffuseTexture = vtkSmartPointer<vtkErTexture>::New();
	vtkSmartPointer<vtkErTexture> SpecularTexture = vtkSmartPointer<vtkErTexture>::New();
	vtkSmartPointer<vtkErTexture> GlossinessTexture = vtkSmartPointer<vtkErTexture>::New();

	DiffuseTexture->SetOutputLevel(1.0f);
	DiffuseTexture->SetRepeat(3, 3);

	vtkSmartPointer<vtkPNGReader> ImageReader = vtkSmartPointer<vtkPNGReader>::New();

	if (ImageReader->CanReadFile(gBackPlaneBitmap))
	{
		DiffuseTexture->SetTextureType(Enums::Bitmap);

		ImageReader->SetFileName(gBackPlaneBitmap);
		ImageReader->Update();

		vtkSmartPointer<vtkErBitmap> Bitmap = vtkSmartPointer<vtkErBitmap>::New();
	
		Bitmap->SetFilterMode(Enums::Linear);
		Bitmap->SetInputConnection(vtkErBitmap::ImageDataPort, ImageReader->GetOutputPort());

		DiffuseTexture->SetInputConnection(vtkErObject::DiffuseTexturePort, Bitmap->GetOutputPort());
	}
	else
	{
		printf("%s cannot be loaded, reverting to unform texture\n", gBackPlaneBitmap);

		DiffuseTexture->SetTextureType(Enums::Procedural);
//		DiffuseTexture->SetProceduralType(Enums::Checker);
		DiffuseTexture->SetUniformColor(1.0, 1.0, 1.0);
		DiffuseTexture->SetCheckerColor1(1.0f, 0.2f, 0.2f);
		DiffuseTexture->SetCheckerColor2(0.2f, 1.0f, 0.2f);
		DiffuseTexture->SetRepeat(3, 3);
		DiffuseTexture->SetOutputLevel(1.00f);
	}

	Object->SetInputConnection(vtkErObject::DiffuseTexturePort, DiffuseTexture->GetOutputPort());
	Object->SetInputConnection(vtkErObject::SpecularTexturePort, SpecularTexture->GetOutputPort());
	Object->SetInputConnection(vtkErObject::GlossinessTexturePort, GlossinessTexture->GetOutputPort());

	Tracer->AddInputConnection(vtkErTracer::ObjectsPort, Object->GetOutputPort());
#endif
}
