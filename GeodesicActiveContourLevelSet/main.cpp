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
//�����һ�׶���ʹ�� itk::CurvatureAnisotropicDiffusionImageFilter ������ƽ
//������ƽ�����ͼ����Ϊ���봫�ݸ� itk::GradientMagnitudeRecursiveGaussianImageFilter ����
//���ٴ��ݸ� itk::SigmoidImageFilter �Ա����Ǳ��ͼ��ı�Ե��һϵ���û��ṩ�����Ӵ��ݸ�
//FastMarchingImageFilter �Ա�������ӳ�䡣�����ӳ���м�ȥһ�������Ա�õ�һ��ˮƽ
//����������ˮƽ����ʾ��������������ˮƽ����Ϊ���봫�ݸ� GeodesicActiveContourLevel
//SetImageFilter ��
//��󣬽� GeodesicActiveContourLevelSetImageFilter ������ˮƽ�����ݸ�һ�� Binary
//ThresholdImageFilter �Ա㴴��һ����ֵģ�������ָ����

int main(int argc, char *argv[])
{
	//��������ʹ��һ��������������ͺ�һ��ά������ͼ�����͡�������Ҫ�õ�ƽ���˲�
	//    ������������¶�����ʹ�ø�������������
	typedef   float           InternalPixelType;
	const     unsigned int    Dimension = 2;
	typedef itk::Image< InternalPixelType, Dimension >  InternalImageType;
	//���������жԵ����ͽ���ʵ��������ʹ�� New( ) ��ʽ����������͵�һ������
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
	//���ö�ȡ�ļ�·��
	reader->SetFileName("../BrainProtonDensitySlice.png");
	//���ñ����ļ�·��
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
	//������չϵ��
	const double propagationScaling = atof("10.0");
	/*���� GeodesicActiveContourLevelSetImageFilter �����ű������������Դ���(����) ϵ����
	  ����(ƽ��) ϵ����ˮƽ����ϵ��֮����н��滻λ����Щ����ʹ�� SetPropagationScaling() ��
	  SetCurvatureScaling() �� SetAdvectionScaling() ��ʽ�����á�����������У����ǽ���������
	  ��ˮƽ�������ű�����Ϊһ���������Ѵ������ű�������Ϊһ�������б���*/
	  //���� ( ���� ) ϵ��
	geodesicActiveContour->SetPropagationScaling(propagationScaling);
	//����(ƽ��) ϵ��
	geodesicActiveContour->SetCurvatureScaling(1.0);
	//ˮƽ����ϵ��
	geodesicActiveContour->SetAdvectionScaling(1.0);

	geodesicActiveContour->SetMaximumRMSError(0.02);
	geodesicActiveContour->SetNumberOfIterations(800);
	//����ʹ�ý������ļ��д��뽫��Щ�˲������ӵ�ͼ 9-21 ��ʾ��һ�����̣�
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
	//���æ�
	const double sigma = atof("0.5");
	gradientMagnitude->SetSigma(sigma);


	//���æ�
	const double alpha = atof("-0.3");
	//���æ� 
	const double beta = atof("2.0");

	sigmoid->SetAlpha(alpha);
	sigmoid->SetBeta(beta);



	typedef FastMarchingFilterType::NodeContainer  NodeContainer;
	typedef FastMarchingFilterType::NodeType       NodeType;

	NodeContainer::Pointer seeds = NodeContainer::New();

	InternalImageType::IndexType  seedPosition;
	//�������ӵ�
	seedPosition[0] = atoi("40");
	seedPosition[1] = atoi("90");

	//���ü��
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

	//Writer �ϵ� Updata( ) �����������̵����С�ͨ���ڳ��ִ�����׳�����ʱ����һ��
	//try / catch ģ����� updata ��
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