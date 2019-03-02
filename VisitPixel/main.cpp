/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit:   http://blog.csdn.net/webzhuce

**********************************************************************/
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkSmartPointer.h"

//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	//new a 3D image based on unsigned short pixel
	typedef itk::Image<unsigned short, 3> ImageTpye;
	ImageTpye::Pointer image = ImageTpye::New();

	//define image type index
	ImageTpye::IndexType start;
	start[0] = 0; //index in x axis
	start[1] = 0; //index in y axis
	start[2] = 0; //index in z axis
	//define image size
	ImageTpye::SizeType size;
	size[0] = 200;
	size[1] = 200;
	size[2] = 1;
	//define image region
	ImageTpye::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);
	//initialize image
	image->SetRegions(region);
	image->Allocate();

	//define pixel variable
	ImageTpye::PixelType initialValue = 0;
	image->FillBuffer(initialValue);
	ImageTpye::IndexType pixelIndex;
	for (int i = 10; i < 50; i++) {
		for (int j = 10; j < 50; j++){
			pixelIndex[0] = i;
			pixelIndex[1] = j;
			pixelIndex[2] = 0;
			image->SetPixel(pixelIndex, 255);
		}
	}
	
	//show image by using vtk
	typedef itk::ImageToVTKImageFilter<ImageTpye> ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(image);
	connector->Update();
	vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkImageViewer> viewer = vtkSmartPointer<vtkImageViewer>::New();
	vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(renderWindow);
	viewer->SetInputData(connector->GetOutput());
	viewer->SetupInteractor(interactor);
	viewer->SetColorWindow(255);
	viewer->SetColorLevel(128);
	viewer->Render();
	interactor->Initialize();
	interactor->Start();

	return EXIT_SUCCESS;
}