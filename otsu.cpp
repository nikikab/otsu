#include <QtGui>
#include <QApplication>
#include <QLabel>
#include <string>
#include <algorithm>
#include <bits/stdc++.h>
#include<cmath>
using namespace std;

const int BLACK = 0;
const int WHITE = 255;

void saveCustomImg(QImage image, char* name){
	QPixmap scaled(
		QPixmap::fromImage(image).scaled(
			image.width(), image.height(),
			Qt::IgnoreAspectRatio,
			Qt::FastTransformation
			)
		);
	image.save(name);
	
	return;
}

void fillinGrayImageimgagearray(int* arr, QImage image)
{

	for (int i = 0; i < image.height(); i ++)
	{
		// QRgb* pnt_row = (QRgb*)image.scanLine(i);
		for (int j = 0; j < image.width(); j++)
		{

			// QRgb pixel = pnt_row[j];
			QRgb pixel = image.pixel(j, i);
			// int red = qRed(pixel);
			// int green = qGreen(pixel);
			// int blue = qBlue(pixel);

			// int gray = 0.2989 * red + 0.5870 * green + 0.1140 * blue;			
			// std::cout << image.width() * i + j << ":\t\t" << qGray(pixel) << "\t" << gray << "\n";
			// arr[ image.width() * i + j ] = qGray(pixel); 
			arr[ image.width() * i + j ] = qGray(pixel); 
		}
	}
}

/* TEST display 1x5 window array */
void  printarray(int* array, int size)
{
	std::cout << "[" << "";
	for (int i = 0; i < size; ++i)
	{
		std::cout << array[i] << " ";
	}
	std::cout << "]" << "\n";
}

void fillinImageHistogram(int* image_histogram, int histo_size,int*  image_imgagearray, int size)	
{
	/* Init image histogram */
	for (int i = 0; i < histo_size; ++i)
	{
		image_histogram[ i ] = 0;
	}

	for (int i = 0; i < size; ++i)
	{
		// std::cout << image_imgagearray[j]<<"\t";
		image_histogram[ image_imgagearray[i] ] ++;
	}

	printarray(image_histogram, 250);
}

int calculateOtsuTreashold(int* image_histogram, int size, int totals)
{
	 int treashold=0; 
	 double interClassVar=0; 
	 double Wb=0.0; //Weight background
	 double mb=0.0; //Mean background
	 double qb=0.0; //variant background
	 double qb2=0.0; //variant background
	 double Wf=0.0; //Weight background
	 double mf=0.0; //Mean foreground
	 double qf=0.0; //Variant forground
	 double qf2=0.0; //Variant forground

	 float background_pixels = 1;
	 float background_pixels_multiplied_by_weight = 0;
	 float forground_pixels = 0;
	 float forground_pixels_multiplied_by_weight = 0;
	 for (int i = 0; i < size; ++i)
	 {

		std::cout << "\n\n" << "i : " << i << "\n";
		std::cout << "interClassVar : " << interClassVar<< "\n";


	 		//background
	 	for (int j = 0; j < i; ++j)
	 	{
	 		background_pixels += image_histogram[j];
	 		background_pixels_multiplied_by_weight += image_histogram[j] * j; 

	 	}
	 	Wb = background_pixels / totals;
	 	mb = background_pixels_multiplied_by_weight / background_pixels;
	 	for (int j = 0; j < i; ++j)
	 	{
	 		qb += pow((j - mb), 2) *  image_histogram[j];
	 	}
	 	qb2 = qb / background_pixels;
		std::cout << "Wb: " << Wb << "\t";
		std::cout << "background_pixels: " << background_pixels << "\t";
		std::cout << "mb: " << mb << "\t";
		std::cout << "qb: " << qb << "\t";
		std::cout << "qb2: " << qb2 << "\n";


	 	
	 		//forground
	 	for (int k = i; k < size; ++k)
	 	{
	 		forground_pixels += image_histogram[k];
	 		forground_pixels_multiplied_by_weight += image_histogram[k] * k; 
	 	}
	 	Wf = forground_pixels / totals;
	 	mf = forground_pixels_multiplied_by_weight / forground_pixels;
	 	for (int k = 0; k < i; ++k)
	 	{
	 		qf += pow((k - mf), 2) *  image_histogram[k];
	 	}
	 	qf2 = qf / forground_pixels;
		std::cout << "Wf: " << Wf << "\t";
		std::cout << "forground_pixels: " << forground_pixels << "\t";
		std::cout << "mf: " << mf << "\t";
		std::cout << "qf: " << qf << "\t";
		std::cout << "qf2: " << qf2 << "\t";

		std::cout << "q2W: " <<  (Wb * qb2) + (Wf * qf2) << "\t";
		 
	 	if (interClassVar < (Wb * qb2) + (Wf * qf2) ) 
	 	{
	 		interClassVar = (Wb * qb2) + (Wf * qf2);
	 		if (interClassVar < 0.5)
	 		{
	 			treashold = i;
	 		}	
	 	}   

	 	// set to 0
	 	qb = 0;
	 	mb = 0;
	 	qf = 0;
	 	mf = 0;

		background_pixels = 1;
		background_pixels_multiplied_by_weight = 0;
		forground_pixels = 0;
		forground_pixels_multiplied_by_weight = 0;
	 }
	 
	 std::cout << "\n" << "totals: " << totals;
	 std::cout << "\n" << "treashold: " << treashold;
}

QImage createOtsuImg(QImage image )
{
	// QImage imageOtsu(image.width(), image.height(), QImage::Format_RGB32);
	QImage imageOtsu(image.width(), image.height(), QImage::Format_Grayscale8);

	/* Prepare image imgagearray */
	int image_imgagearray[image.width() * image.height()];
	fillinGrayImageimgagearray(image_imgagearray, image);
	int image_histogram[255];
	fillinImageHistogram(image_histogram, 255, image_imgagearray, image.width() * image.height());	
	int otsu_threshold = calculateOtsuTreashold(image_histogram, 255, image.width() * image.height()); 


	for (int i = 0; i < image.height(); i ++)
	{
		for (int j = 0; j < image.width(); j++)
		{
			int imgarray_indx = i * image.width() + j; 
			
			/*calculate otsu value at this index*/
			QRgb valueOtsuGrey;
			// int otsuValue = (image_imgagearray[imgarray_indx] > otsu_threshold ) ? BLACK : WHITE;
			int otsuValue = image_imgagearray[imgarray_indx];

			valueOtsuGrey = qRgb(otsuValue, otsuValue, otsuValue);
			imageOtsu.setPixel(j, i, valueOtsuGrey);
		}


	}
	return imageOtsu;

}

/*TEST this method prints some tests. Will be execuetd if the second parameter is "test"*/
int testMe(int argc, char *argv[])
{
	//TODO
	// add tests methods here
}

int main(int argc, char *argv[])
{

	if ( argc <= 1 )
	{
		std::cout << "Provide image name as a parameter!";	
		return 0;
	}

	if ( argc > 2 && (std::string) argv[2] == "test")
	{
		testMe(argc, argv);
		return 0;
	}

	char* imgname = argv[1];

	QApplication app(argc, argv);	

	QImage image;
	// showImage("lenna.png");
	QLabel label;
	if (image.load(imgname))
	{
		label.setPixmap(QPixmap::fromImage(image));
	}
	else
	{
		label.setText("Cannot load image.");
	}
	
	// QImage otsuImg = create1dmedianImg(image);
	QImage otsuImg = createOtsuImg(image);
	label.setPixmap(QPixmap::fromImage(otsuImg));
	label.show();

	saveCustomImg(otsuImg, "out_1dm.png");


	QTextStream(stdout) << "\n";

	QTextStream(stdout) << image.height() << " " << image.width() << endl;
	QTextStream(stdout) << "\n";
	return app.exec();
}//main
