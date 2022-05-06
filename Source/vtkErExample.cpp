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

#include "vtkErInterface.cpp"


void InteractorRender();
void OffScreenRender();

using namespace ExposureRender;

int main(int, char *[])
{
    //InteractorRender();

    //OffScreenRender();

    std::vector<unsigned char> imageData = vtkErInterface::OffScreenRenderDicom();

    std::cout<<"image data size:"<<imageData.size()<<std::endl;

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

    vtkErInterface::ConfigureER(Renderer);

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

    vtkErInterface::ConfigureER(Renderer);

    Renderer->ResetCamera();

    RenderWindow->Render();

    //RenderWindowInteractor->Start();

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(RenderWindow);
    windowToImageFilter->Update();

    //std::string pngFile = "D:\\exposure-render-ercore\\test.png";
    std::string pngFile = "/home/liuxiaobo/ProjectCode/exposure/exposure-render.ercore/test.png";
    
    vtkSmartPointer<vtkPNGWriter> writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName(pngFile.c_str());
    writer->SetInputConnection(windowToImageFilter->GetOutputPort());
    writer->Write();

}


