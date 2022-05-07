/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit:   http://blog.csdn.net/webzhuce

**********************************************************************/
#include "itkImage.h"
#include "itkImageToVTKImageFilter.h"
#include "itkImportImageFilter.h"//包含 ImportImageFilter(图像像素数据导入缓冲器) 类的头文件
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
	PixelType::ValueType red = initialValue.GetRed();//提取红色部分
	PixelType::ValueType green = initialValue.GetGreen();//提取绿色部分
	PixelType::ValueType blue = initialValue.GetBlue();//提取蓝色部分
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
	/*选择数据类型来表示图像像素。我们假设内存的外部内存块使用同样的数据类
	  型来表示像素*/
	typedef unsigned char   PixelType;
	const unsigned int Dimension = 3;

	typedef itk::Image< PixelType, Dimension > ImageType;
	//ImportImageFilter 类型的实例化
	typedef itk::ImportImageFilter< PixelType, Dimension >   ImportFilterType;
	//使用 New( ) 方法创建一个滤镜对象importFilter然后指向一个智能指针
	ImportFilterType::Pointer importFilter = ImportFilterType::New();
	/*滤镜要求用户指定图像的大小来作为输出，使用 SetRgion() 方法即可做到。图像大小必
	 须和当前调用的缓冲器的像素变量的数字相匹配*/
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
	//使用 SetOrigin() 方法来指定输出图像的原点
	const itk::SpacePrecisionType origin[Dimension] = { 0.0, 0.0, 0.0 };
	importFilter->SetOrigin(origin);
	//使用 SetSpacing( ) 方法来传递输出图像的间距
	const itk::SpacePrecisionType  spacing[Dimension] = { 1.0, 1.0, 1.0 };
	importFilter->SetSpacing(spacing);
	/*现在我们分配包含像素数据的内存块传递信息到 ImportImageFilter 。注意：我们使用与
	SetRegion() 方法指定的大小完全相同的尺寸。在实际应用中，你可以使用一个代表图像的
	不同的数据结构从一些其他的类库中得到这个缓冲器。*/
	const unsigned int numberOfPixels = size[0] * size[1] * size[2];
	PixelType *localBuffer = new PixelType[numberOfPixels];

	const double radius = 80.0;
	/*这里可以用一个 binary sphere 来填充这个缓冲器。这里我们像 C 或 FOTTRAN 编程语
	言一样使用简单的 for () 循环。注意： ITK 在其访问像素的内部编码中不能使用 for () 循环。
	使用支持处理 n 维图像的 itk::ImageIterators 来代替执行所以的像素访问任务。*/
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

	/* 缓冲器在 SetImportPointer() 作用下传递到 ImportImageFilter 。注意这种方法的最后一个
	问题是当内存不再使用时指定谁来释放内存。当返回值为假时，表示当调用析构时
	ImportImageFilter 并没有释放缓冲器；另一方面，当返回值是真时，表示允许释放析构的输
	入滤镜上的内存块。
	由于 ImportImageFilter 释放了适当的内存块， C++ new() 操作就可以调用这些内存。用
	其他分配内存机制分配的内存，比如 C 中的 malloc 和 calloc ，将不会由 ImportImageFilter
	来释放适当的内存。换句话说，编程应用者就需要确保仅仅给 ImportImageFilter 命令来释放
	C++ 新操作分配内存。*/
	const bool importImageFilterWillOwnTheBuffer = true;
	importFilter->SetImportPointer(localBuffer, numberOfPixels,
		importImageFilterWillOwnTheBuffer);
	importFilter->Update();

	typedef itk::ImageFileWriter< ImageType > WriterType;
	WriterType::Pointer writer = WriterType::New();

	writer->SetFileName("test.png");
	/*最后，我们将这个滤镜的输出连到一个管道上。为简便起见，我们在这里只使用一个
	writer ，当然其他任何滤镜都可以：*/
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