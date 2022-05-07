/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit:   http://blog.csdn.net/webzhuce

**********************************************************************/
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"

int main(int argc, char **argv)
{
	typedef itk::Image<unsigned short, 2> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;   //ͼ���ȡ��
	typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;  //VTK��ITK������

	ReaderType::Pointer reader = ReaderType::New();
	ConnectorType::Pointer connector = ConnectorType::New();
	reader->SetFileName("../0.jpg");
	connector->SetInput(reader->GetOutput());
	connector->Update();

	vtkImageViewer *viewer = vtkImageViewer::New();
	vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
	viewer->SetInputData(connector->GetOutput());
	viewer->SetupInteractor(interactor);
	viewer->SetColorWindow(255); //���ô���
	viewer->SetColorLevel(128);   //���ô�λ
	viewer->Render();
	interactor->Initialize();
	interactor->Start();
	return EXIT_SUCCESS;
}
