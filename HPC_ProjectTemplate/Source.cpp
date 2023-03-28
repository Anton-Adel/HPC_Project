#include <iostream>
#include <math.h>
#include <stdlib.h>
#include<string.h>
#include<msclr\marshal_cppstd.h>
#include <ctime>// include this header
#pragma once
#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#include <mpi.h>
#include <stdio.h>
#include<math.h>
using namespace System;
using namespace std;
using namespace msclr::interop;



int* inputImage(int* w, int* h, System::String^ imagePath) //put the size of image in w & h
{
	int* input;




	int OriginalImageWidth, OriginalImageHeight;



	//*********************************************************Read Image and save it to local arrayss*************************
	//Read Image and save it to local arrayss

	System::Drawing::Bitmap BM(imagePath);
	OriginalImageWidth = BM.Width;
	OriginalImageHeight = BM.Height;
	*w = BM.Width;
	*h = BM.Height;
	int* Red = new int[BM.Height * BM.Width];
	int* Green = new int[BM.Height * BM.Width];
	int* Blue = new int[BM.Height * BM.Width];
	input = new int[BM.Height * BM.Width];
	for (int i = 0; i < BM.Height; i++)
	{
		for (int j = 0; j < BM.Width; j++)
		{
			System::Drawing::Color c = BM.GetPixel(j, i);
			Red[i * BM.Width + j] = c.R;
			Blue[i * BM.Width + j] = c.B;
			Green[i * BM.Width + j] = c.G;



			input[i * BM.Width + j] = ((c.R + c.B + c.G) / 3); //gray scale value equals the average of RGB values



		}
	}
	return input;
}




void createImage(int* image, int width, int height, int index)
{
	System::Drawing::Bitmap MyNewImage(width, height);




	for (int i = 0; i < MyNewImage.Height; i++)
	{
		for (int j = 0; j < MyNewImage.Width; j++)
		{
			//i * OriginalImageWidth + j
			if (image[i * width + j] < 0)
			{
				image[i * width + j] = 0;
			}
			if (image[i * width + j] > 255)
			{
				image[i * width + j] = 255;
			}
			System::Drawing::Color c = System::Drawing::Color::FromArgb(image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j], image[i * MyNewImage.Width + j]);
			MyNewImage.SetPixel(j, i, c);
		}
	}
	MyNewImage.Save("..//Data//Output//outputRes" + index + ".png");
	cout << "result Image Saved " << index << endl;
}




int main()
{
	int start_s, stop_s, TotalTime = 0;
	int ImageWidth = 4, ImageHeight = 4;
	System::String^ imagePath;
	std::string img;
	img = "..//Data//Input//5N.png";
	imagePath = marshal_as<System::String^>(img);
	int* imageData = inputImage(&ImageWidth, &ImageHeight, imagePath);
	float kernal3[9] =
	{ 1 / 9.0,1 / 9.0,1 / 9.0
	,1 / 9.0,1 / 9.0,1 / 9.0
	,1 / 9.0,1 / 9.0,1 / 9.0 };
	int* res_ = new int[(ImageWidth - 2) * (ImageHeight - 2)];
	int index_ = 0, index2_ = 0, index3_ = 0;
	int check_ = 0;
	int counter_ = 0, counter1_ = 0, counter3_ = 1;
	float result = 0.0;
	int index_1 = 0;
	int arrsize = sizeof(kernal3) / sizeof(kernal3[0]);
	for (int i = 0; i < ImageWidth * ImageHeight; i++)
	{
		check_ = 0;
		for (int j = 0; j < arrsize; j++)
		{
			if (check_ / sqrt(arrsize) == 1)  
			{
				index_ += (ImageWidth - 3);      
				check_ = 0;
			}
			result += imageData[index_++];    
			check_++;   
		}
		int res = ceil(result / 9);
		res_[index_1++] = res;
		result = 0;
		index3_++;   
		index_ = index3_;    
		counter_++;    
		if (counter_ / (ImageWidth - 2) == 1)
		{
			counter1_++;
			index_ = ImageWidth * counter3_;
			index3_ = index_;
			counter3_++;  
			counter_ = 0;
		}
		if (counter1_ / (ImageHeight - 2) == 1)
		{
			break;
		}
	}
	start_s = clock();
	createImage(res_, ImageWidth - 2, ImageHeight - 2, 0);
	stop_s = clock();
	TotalTime += (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
	cout << "time: " << TotalTime << endl;
	free(imageData);
	return 0;

}