/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit:   http://blog.csdn.net/webzhuce

**********************************************************************/
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"
#include "itkImportImageFilter.h"//���� ImportImageFilter(ͼ���������ݵ��뻺����) ���ͷ�ļ�
#include "itkImageFileWriter.h"

#include "vtkImageViewer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkImageActor.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkSmartPointer.h"


void CreateGrayImage()
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
		for (int j = 10; j < 50; j++) {
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
}

void CreateColorImage()
{
	//new a 3D  color image based on unsigned char pixel
	typedef itk::RGBPixel< unsigned char >    PixelType;
	typedef itk::Image<PixelType, 3> ImageTpye;
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
	ImageTpye::PixelType initialValue ;
	PixelType::ValueType red = initialValue.GetRed();//��ȡ��ɫ����
	PixelType::ValueType green = initialValue.GetGreen();//��ȡ��ɫ����
	PixelType::ValueType blue = initialValue.GetBlue();//��ȡ��ɫ����
	std::cout << "Red = "
		<< itk::NumericTraits<PixelType::ValueType>::PrintType(red)
		<< std::endl;
	std::cout << "Green = "
		<< itk::NumericTraits<PixelType::ValueType>::PrintType(green)
		<< std::endl;
	std::cout << "Blue = "
		<< itk::NumericTraits<PixelType::ValueType>::PrintType(blue)
		<< std::endl << std::endl;

	image->FillBuffer(initialValue);
	initialValue.SetRed(255);
	ImageTpye::IndexType pixelIndex;
	for (int i = 10; i < 50; i++) {
		for (int j = 10; j < 50; j++) {
			pixelIndex[0] = i;
			pixelIndex[1] = j;
			pixelIndex[2] = 0;
			image->SetPixel(pixelIndex, initialValue);
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
}

void CreateImageFromBuffer()
{
	/*ѡ��������������ʾͼ�����ء����Ǽ����ڴ���ⲿ�ڴ��ʹ��ͬ����������
	  ������ʾ����*/
	typedef unsigned char   PixelType;
	const unsigned int Dimension = 3;

	typedef itk::Image< PixelType, Dimension > ImageType;
	//ImportImageFilter ���͵�ʵ����
	typedef itk::ImportImageFilter< PixelType, Dimension >   ImportFilterType;
	//ʹ�� New( ) ��������һ���˾�����importFilterȻ��ָ��һ������ָ��
	ImportFilterType::Pointer importFilter = ImportFilterType::New();
	/*�˾�Ҫ���û�ָ��ͼ��Ĵ�С����Ϊ�����ʹ�� SetRgion() ��������������ͼ���С��
	 ��͵�ǰ���õĻ����������ر�����������ƥ��*/
	ImportFilterType::SizeType  size;
	size[0] = 200;  // size along X
	size[1] = 200;  // size along Y
	size[2] = 200;  // size along Z
	ImportFilterType::IndexType start;
	start.Fill(0);

	ImportFilterType::RegionType region;
	region.SetIndex(start);
	region.SetSize(size);
	importFilter->SetRegion(region);
	//ʹ�� SetOrigin() ������ָ�����ͼ���ԭ��
	const itk::SpacePrecisionType origin[Dimension] = { 0.0, 0.0, 0.0 };
	importFilter->SetOrigin(origin);
	//ʹ�� SetSpacing( ) �������������ͼ��ļ��
	const itk::SpacePrecisionType  spacing[Dimension] = { 1.0, 1.0, 1.0 };
	importFilter->SetSpacing(spacing);
	/*�������Ƿ�������������ݵ��ڴ�鴫����Ϣ�� ImportImageFilter ��ע�⣺����ʹ����
	SetRegion() ����ָ���Ĵ�С��ȫ��ͬ�ĳߴ硣��ʵ��Ӧ���У������ʹ��һ������ͼ���
	��ͬ�����ݽṹ��һЩ����������еõ������������*/
	const unsigned int numberOfPixels = size[0] * size[1] * size[2];
	PixelType *localBuffer = new PixelType[numberOfPixels];

	const double radius = 80.0;
	/*���������һ�� binary sphere �������������������������� C �� FOTTRAN �����
	��һ��ʹ�ü򵥵� for () ѭ����ע�⣺ ITK ����������ص��ڲ������в���ʹ�� for () ѭ����
	ʹ��֧�ִ��� n άͼ��� itk::ImageIterators ������ִ�����Ե����ط�������*/
	const double radius2 = radius * radius * 3;
	PixelType *it = localBuffer + (size[2] - size[2]) * size[0] * size[1];


	for (unsigned int z = 0; z < size[2]; z++)
	{
		const double dz = static_cast<double>(z)
			- static_cast<double>(size[2]) / 2.0;
		for (unsigned int y = 0; y < size[1]; y++)
		{
			const double dy = static_cast<double>(y)
				- static_cast<double>(size[1]) / 2.0;
			for (unsigned int x = 0; x < size[0]; x++)
			{
				const double dx = static_cast<double>(x)
					- static_cast<double>(size[0]) / 2.0;
				const double d2 = dx * dx + dy * dy + dz * dz;
				*it++ = (d2 < radius2) ? 255 : 0;
			}
		}
	}

	/* �������� SetImportPointer() �����´��ݵ� ImportImageFilter ��ע�����ַ��������һ��
	�����ǵ��ڴ治��ʹ��ʱָ��˭���ͷ��ڴ档������ֵΪ��ʱ����ʾ����������ʱ
	ImportImageFilter ��û���ͷŻ���������һ���棬������ֵ����ʱ����ʾ�����ͷ���������
	���˾��ϵ��ڴ�顣
	���� ImportImageFilter �ͷ����ʵ����ڴ�飬 C++ new() �����Ϳ��Ե�����Щ�ڴ档��
	���������ڴ���Ʒ�����ڴ棬���� C �е� malloc �� calloc ���������� ImportImageFilter
	���ͷ��ʵ����ڴ档���仰˵�����Ӧ���߾���Ҫȷ�������� ImportImageFilter �������ͷ�
	C++ �²��������ڴ档*/
	const bool importImageFilterWillOwnTheBuffer = true;
	importFilter->SetImportPointer(localBuffer, numberOfPixels,
		importImageFilterWillOwnTheBuffer);
	importFilter->Update();

	typedef itk::ImageFileWriter< ImageType > WriterType;
	WriterType::Pointer writer = WriterType::New();

	writer->SetFileName("test.png");
	/*������ǽ�����˾����������һ���ܵ��ϡ�Ϊ������������������ֻʹ��һ��
	writer ����Ȼ�����κ��˾������ԣ�*/
	writer->SetInput(importFilter->GetOutput());

	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & exp)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << exp << std::endl;
		
	}

}

int main(int argc, char *argv[])
{
	//CreateGrayImage();
	//CreateColorImage();
	CreateImageFromBuffer();

	return EXIT_SUCCESS;
}