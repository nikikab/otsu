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

void fillinGrayImageimgagearray(int* histo, QImage image)
{

	for (int indx_row = 0; indx_row < image.height(); indx_row ++)
	{
		QRgb* pnt_row = (QRgb*)image.scanLine(indx_row);
		for (int indx_col = 0; indx_col < image.width(); indx_col++)
		{

			QRgb pixel = pnt_row[indx_col];
			histo[ image.width() * indx_row + indx_col ] = qGray(pixel); 
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
	 double Wb=0; //Weight background
	 double mb=0; //Mean background
	 double qb=0; //variant background
	 double qb2=0; //variant background
	 double Wf=0; //Weight background
	 double mf=0; //Mean foreground
	 double qf=0; //Variant forground
	 double qf2=0; //Variant forground

	 int background_pixels = 0;
	 int background_pixels_multiplied_by_weight = 0;
	 int forground_pixels = 0;
	 int forground_pixels_multiplied_by_weight = 0;
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
		std::cout << "Wb: " << qb << "\t";
		std::cout << "qb: " << qb << "\t";
		std::cout << "qb2: " << qb2 << "\t";


	 	
	 		//forground
	 	for (int k = i; k < size; ++k)
	 	{
	 		forground_pixels += image_histogram[k];
	 		forground_pixels_multiplied_by_weight += image_histogram[k] * k; 
	 	}
	 	Wf = forground_pixels / totals;
	 	mf = forground_pixels_multiplied_by_weight += forground_pixels;
	 	for (int k = 0; k < i; ++k)
	 	{
	 		qf += pow((k - mf), 2) *  image_histogram[k];
	 	}
	 	qf2 = qf / forground_pixels;
		std::cout << "Wf: " << qf << "\t";
		std::cout << "qf: " << qf << "\t";
		std::cout << "qf2: " << qf2 << "\t";

		 
	 	if (interClassVar < (Wb * qb2) + (Wf * qf2) ) 
	 	{
	 		interClassVar = (Wb * qb2) + (Wf * qf2);
	 		if (interClassVar < 0.5)
	 		{
	 			treashold = i;
	 		}	
	 	}   

	 	Wb = 0;
	 	qb2 = 0;
	 	Wf = 0;
	 	qf2 = 0;
	 }
	 
	 std::cout << "\n" << "treashold: " << treashold;
}

QImage createOtsuImg(QImage image )
{
	QImage imageOtsu(image.width(), image.height(), QImage::Format_RGB32);

	/* Prepare image imgagearray */
	int image_imgagearray[image.width() * image.height()];
	fillinGrayImageimgagearray(image_imgagearray, image);
	int image_histogram[255];
	fillinImageHistogram(image_histogram, 255, image_imgagearray, image.width() * image.height());	
	int otsu_threshold = calculateOtsuTreashold(image_histogram, 255, image.width() * image.height()); 


	for (int indx_row = 0; indx_row < image.height(); indx_row ++)
	{
		QRgb* pnt_row = (QRgb*)image.scanLine(indx_row);
		for (int indx_col = 0; indx_col < image.width(); indx_col++)
		{
			int imgarray_indx = indx_row * image.width() + indx_col; 
			
			/*calculate otsu value at this index*/
			QRgb valueOtsuGrey;
			int otsuValue = (image_imgagearray[imgarray_indx] > otsu_threshold) ? BLACK : WHITE;

			/*print diffs after filtering*/
			valueOtsuGrey = qRgb(otsuValue, otsuValue, otsuValue);
			imageOtsu.setPixel(indx_col, indx_row, valueOtsuGrey);
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
	
	// QImage oneDMedianImg = create1dmedianImg(image);
	QImage oneDMedianImg = createOtsuImg(image);
	label.setPixmap(QPixmap::fromImage(oneDMedianImg));
	// label.show();

	saveCustomImg(oneDMedianImg, "out_1dm.png");


	QTextStream(stdout) << "\n";

	QTextStream(stdout) << image.height() << " " << image.width() << endl;
	QTextStream(stdout) << "\n";
	return app.exec();
}//main
