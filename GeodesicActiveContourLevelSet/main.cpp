/**********************************************************************

Copyright (c) Mr.Bin. All rights reserved.
For more information visit:   http://blog.csdn.net/webzhuce

**********************************************************************/
#include "itkGeodesicActiveContourLevelSetImageFilter.h"

#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkFastMarchingImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
//程序第一阶段是使用 itk::CurvatureAnisotropicDiffusionImageFilter 来进行平
//滑。将平滑后的图像作为输入传递给 itk::GradientMagnitudeRecursiveGaussianImageFilter ，并
//且再传递给 itk::SigmoidImageFilter 以便产生潜在图像的边缘。一系列用户提供的种子传递给
//FastMarchingImageFilter 以便计算距离映射。从这个映射中减去一个常数以便得到一个水平
//集，其中零水平集表示最初的轮廓。这个水平集作为输入传递给 GeodesicActiveContourLevel
//SetImageFilter 。
//最后，将 GeodesicActiveContourLevelSetImageFilter 产生的水平集传递给一个 Binary
//ThresholdImageFilter 以便创建一个二值模板来表达分割对象。

int main(int argc, char *argv[])
{
	//现在我们使用一个特殊的像素类型和一个维来定义图像类型。由于需要用到平滑滤波
	//    器，这种情况下对像素使用浮点型数据类型
	typedef   float           InternalPixelType;
	const     unsigned int    Dimension = 2;
	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
	//接下来几行对的类型进行实例化。并使用 New( ) 方式创建这个类型的一个对象
	typedef unsigned char                            OutputPixelType;
	typedef itk::Image< OutputPixelType, Dimension > OutputImageType;
	typedef itk::BinaryThresholdImageFilter<
		InternalImageType,
		OutputImageType    >       ThresholdingFilterType;

	ThresholdingFilterType::Pointer thresholder = ThresholdingFilterType::New();

	thresholder->SetLowerThreshold(-1000.0);
	thresholder->SetUpperThreshold(0.0);

	thresholder->SetOutsideValue(0);
	thresholder->SetInsideValue(255);


	typedef  itk::ImageFileReader< InternalImageType > ReaderType;
	typedef  itk::ImageFileWriter<  OutputImageType  > WriterType;

	ReaderType::Pointer reader = ReaderType::New();
	WriterType::Pointer writer = WriterType::New();
	//设置读取文件路径
	reader->SetFileName("../BrainProtonDensitySlice.png");
	//设置保存文件路径
	writer->SetFileName("BrainProtonDensitySlice_G_ActiveContour.png");

	typedef itk::RescaleIntensityImageFilter<
		InternalImageType,
		OutputImageType >   CastFilterType;

	typedef   itk::CurvatureAnisotropicDiffusionImageFilter<
		InternalImageType,
		InternalImageType >  SmoothingFilterType;

	SmoothingFilterType::Pointer smoothing = SmoothingFilterType::New();


	typedef   itk::GradientMagnitudeRecursiveGaussianImageFilter<
		InternalImageType,
		InternalImageType >  GradientFilterType;
	typedef   itk::SigmoidImageFilter<
		InternalImageType,
		InternalImageType >  SigmoidFilterType;

	GradientFilterType::Pointer  gradientMagnitude = GradientFilterType::New();

	SigmoidFilterType::Pointer sigmoid = SigmoidFilterType::New();

	sigmoid->SetOutputMinimum(0.0);
	sigmoid->SetOutputMaximum(1.0);


	typedef  itk::FastMarchingImageFilter<
		InternalImageType,
		InternalImageType >    FastMarchingFilterType;


	FastMarchingFilterType::Pointer  fastMarching = FastMarchingFilterType::New();

	typedef  itk::GeodesicActiveContourLevelSetImageFilter< InternalImageType,
		InternalImageType >    GeodesicActiveContourFilterType;
	GeodesicActiveContourFilterType::Pointer geodesicActiveContour =
		GeodesicActiveContourFilterType::New();
	//设置扩展系数
	const double propagationScaling = atof("10.0");
	/*对于 GeodesicActiveContourLevelSetImageFilter ，缩放比例参数用来对传播(膨胀) 系数、
	  曲率(平滑) 系数和水平对流系数之间进行交替换位。这些参数使用 SetPropagationScaling() 、
	  SetCurvatureScaling() 和 SetAdvectionScaling() 方式来设置。在这个例子中，我们将设置曲率
	  和水平对流缩放比例作为一个参数并把传播缩放比例设置为一个命令行变量*/
	  //传播 ( 膨胀 ) 系数
	geodesicActiveContour->SetPropagationScaling(propagationScaling);
	//曲率(平滑) 系数
	geodesicActiveContour->SetCurvatureScaling(1.0);
	//水平对流系数
	geodesicActiveContour->SetAdvectionScaling(1.0);

	geodesicActiveContour->SetMaximumRMSError(0.02);
	geodesicActiveContour->SetNumberOfIterations(800);
	//现在使用接下来的几行代码将这些滤波器连接到图 9-21 所示的一个流程：
	smoothing->SetInput(reader->GetOutput());
	gradientMagnitude->SetInput(smoothing->GetOutput());
	sigmoid->SetInput(gradientMagnitude->GetOutput());

	geodesicActiveContour->SetInput(fastMarching->GetOutput());
	geodesicActiveContour->SetFeatureImage(sigmoid->GetOutput());

	thresholder->SetInput(geodesicActiveContour->GetOutput());
	writer->SetInput(thresholder->GetOutput());

	smoothing->SetTimeStep(0.125);
	smoothing->SetNumberOfIterations(5);
	smoothing->SetConductanceParameter(9.0);
	//设置σ
	const double sigma = atof("0.5");
	gradientMagnitude->SetSigma(sigma);


	//设置α
	const double alpha = atof("-0.3");
	//设置β 
	const double beta = atof("2.0");

	sigmoid->SetAlpha(alpha);
	sigmoid->SetBeta(beta);



	typedef FastMarchingFilterType::NodeContainer  NodeContainer;
	typedef FastMarchingFilterType::NodeType       NodeType;

	NodeContainer::Pointer seeds = NodeContainer::New();

	InternalImageType::IndexType  seedPosition;
	//设置种子点
	seedPosition[0] = atoi("40");
	seedPosition[1] = atoi("90");

	//设置间距
	const double initialDistance = atof("5.0");

	NodeType node;

	const double seedValue = -initialDistance;

	node.SetValue(seedValue);
	node.SetIndex(seedPosition);



	seeds->Initialize();
	seeds->InsertElement(0, node);


	fastMarching->SetTrialPoints(seeds);

	fastMarching->SetSpeedConstant(1.0);


	CastFilterType::Pointer caster1 = CastFilterType::New();
	CastFilterType::Pointer caster2 = CastFilterType::New();
	CastFilterType::Pointer caster3 = CastFilterType::New();
	CastFilterType::Pointer caster4 = CastFilterType::New();

	WriterType::Pointer writer1 = WriterType::New();
	WriterType::Pointer writer2 = WriterType::New();
	WriterType::Pointer writer3 = WriterType::New();
	WriterType::Pointer writer4 = WriterType::New();

	caster1->SetInput(smoothing->GetOutput());
	writer1->SetInput(caster1->GetOutput());
	writer1->SetFileName("GeodesicActiveContourImageFilterOutput1.png");
	caster1->SetOutputMinimum(0);
	caster1->SetOutputMaximum(255);
	writer1->Update();

	caster2->SetInput(gradientMagnitude->GetOutput());
	writer2->SetInput(caster2->GetOutput());
	writer2->SetFileName("GeodesicActiveContourImageFilterOutput2.png");
	caster2->SetOutputMinimum(0);
	caster2->SetOutputMaximum(255);
	writer2->Update();

	caster3->SetInput(sigmoid->GetOutput());
	writer3->SetInput(caster3->GetOutput());
	writer3->SetFileName("GeodesicActiveContourImageFilterOutput3.png");
	caster3->SetOutputMinimum(0);
	caster3->SetOutputMaximum(255);
	writer3->Update();

	caster4->SetInput(fastMarching->GetOutput());
	writer4->SetInput(caster4->GetOutput());
	writer4->SetFileName("GeodesicActiveContourImageFilterOutput4.png");
	caster4->SetOutputMinimum(0);
	caster4->SetOutputMaximum(255);

	fastMarching->SetOutputSize(
		reader->GetOutput()->GetBufferedRegion().GetSize());

	//Writer 上的 Updata( ) 方法触发流程的运行。通常在出现错误和抛出异议时，从一个
	//try / catch 模块调用 updata ：
	try
	{
		writer->Update();
	}
	catch (itk::ExceptionObject & excep)
	{
		std::cerr << "Exception caught !" << std::endl;
		std::cerr << excep << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << std::endl;
	std::cout << "Max. no. iterations: " << geodesicActiveContour->GetNumberOfIterations() << std::endl;
	std::cout << "Max. RMS error: " << geodesicActiveContour->GetMaximumRMSError() << std::endl;
	std::cout << std::endl;
	std::cout << "No. elpased iterations: " << geodesicActiveContour->GetElapsedIterations() << std::endl;
	std::cout << "RMS change: " << geodesicActiveContour->GetRMSChange() << std::endl;

	writer4->Update();


	typedef itk::ImageFileWriter< InternalImageType > InternalWriterType;

	InternalWriterType::Pointer mapWriter = InternalWriterType::New();
	mapWriter->SetInput(fastMarching->GetOutput());
	mapWriter->SetFileName("GeodesicActiveContourImageFilterOutput4.mha");
	mapWriter->Update();

	InternalWriterType::Pointer speedWriter = InternalWriterType::New();
	speedWriter->SetInput(sigmoid->GetOutput());
	speedWriter->SetFileName("GeodesicActiveContourImageFilterOutput3.mha");
	speedWriter->Update();

	InternalWriterType::Pointer gradientWriter = InternalWriterType::New();
	gradientWriter->SetInput(gradientMagnitude->GetOutput());
	gradientWriter->SetFileName("GeodesicActiveContourImageFilterOutput2.mha");
	gradientWriter->Update();

	return EXIT_SUCCESS;
}