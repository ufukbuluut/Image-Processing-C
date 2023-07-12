#pragma once
#include <atlstr.h>
#include <iostream>
#include "image.h"
#include"Clustering.h"
int* Histogram(image im);


namespace read_image {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ openToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^ clusteringToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ histogramExtractionToolStripMenuItem;

	private:
		/// <summary>
		/// Required designer variable.
		unsigned char* img_data = NULL;
		int img_w, img_h, img_c;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^ chart1;
	private: System::Windows::Forms::ToolStripMenuItem^ oneDimensionalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ levelsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ euclideanToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ mahalonobisToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ morfologyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ dilationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ erosionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ erosionDilationToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ labelingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ ýmageDetectionToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::ToolStripMenuItem^ objectMomentHesaplaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ homework2ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ türevHesaplaToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ nonMaximumSupressionToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ hystericThresholdToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ binaryEdgeImageToolStripMenuItem;






		   /// </summary>
		   System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		   /// <summary>
		   /// Required method for Designer support - do not modify
		   /// the contents of this method with the code editor.
		   /// </summary>
		   void ShowRGBImages(image im) {
			   pictureBox1->Width = im.w;
			   pictureBox1->Height = im.h;
			   pictureBox1->Refresh();
			   Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			   pictureBox1->Image = surface;
			   Color c;
			   int psw, bufpos;
			   psw = im.w * 3;
			   for (int row = 0; row < im.h; row++)
				   for (int col = 0; col < im.w; col++) {
					   bufpos = row * psw + col * im.c;


					   c = Color::FromArgb(im.data[bufpos], im.data[bufpos + 1], im.data[bufpos + 2]); //R-G-B
					   surface->SetPixel(col, row, c);
				   }
		   }//ShowImages
		   void ShowIntensity(image im)
		   {
			   pictureBox1->Width = im.w;
			   pictureBox1->Height = im.h;
			   pictureBox1->Refresh();
			   Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			   pictureBox1->Image = surface;
			   Color c;
			   int psw, bufpos;
			   psw = im.w * im.c;
			   for (int row = 0; row < im.h; row++)
				   for (int col = 0; col < im.w; col++) {
					   bufpos = row * psw + col * im.c;
					   c = Color::FromArgb(im.data[bufpos], im.data[bufpos], im.data[bufpos]);
					   surface->SetPixel(col, row, c);
				   }

		   }

		   void ShowBinary(image im)
		   {
			   pictureBox1->Width = im.w;
			   pictureBox1->Height = im.h;
			   pictureBox1->Refresh();
			   Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			   pictureBox1->Image = surface;
			   Color c;
			   int psw, bufpos;
			   psw = im.w * im.c;
			   for (int row = 0; row < im.h; row++)
				   for (int col = 0; col < im.w; col++) {
					   bufpos = row * psw + col * im.c;
					   c = Color::FromArgb(im.data[bufpos], im.data[bufpos], im.data[bufpos]);
					   surface->SetPixel(col, row, c);
				   }
		   }
		   void ShowLabel(image im)
		   {
			   pictureBox1->Width = im.w;
			   pictureBox1->Height = im.h;
			   pictureBox1->Refresh();
			   Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			   pictureBox1->Image = surface;
			   Color c;
			   for (int row = 0; row < im.h; row++)
				   for (int column = 0; column < im.w; column++)
				   {
					   c = Color::FromArgb(im.data[(row * im.w) + column], im.data[(row * im.w) + column], im.data[(row * im.w) + column]);
					   surface->SetPixel(column, row, c);
				   }

		   }
		   void ShowObject(image im)
		   {
			   pictureBox1->Width = im.w;
			   pictureBox1->Height = im.h;
			   pictureBox1->Refresh();
			   Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			   pictureBox1->Image = surface;
			   Color c;

			   int padding = 0;
			   int scanlinebytes = im.w * 3;
			   while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
				   padding++;
			   // get the padded scanline width
			   int psw = scanlinebytes + padding;

			   long newpos;

			   for (int row = 0; row < im.h; row++)
				   for (int column = 0; column < im.w; column++)
				   {
					   newpos = (im.h - row - 1) * psw + column * 3;
					   c = Color::FromArgb(im.data[newpos], im.data[newpos+1], im.data[newpos+2]);
					   surface->SetPixel(column, row, c);
				   }

		   }

		   /*void ShowBinaryDilation(image im)
		   {
			   pictureBox3->Width = im.w;
			   pictureBox3->Height = im.h;
			   pictureBox3->Refresh();
			   Bitmap^ surface = gcnew Bitmap(im.w, im.h);
			   pictureBox3->Image = surface;
			   Color c;
			   int psw, bufpos;
			   psw = im.w * im.c;
			   for (int row = 0; row < im.h; row++)
				   for (int col = 0; col < im.w; col++) {
					   bufpos = row * psw + col * im.c;
					   c = Color::FromArgb(im.data[bufpos], im.data[bufpos], im.data[bufpos]);
					   surface->SetPixel(col, row, c);
				   }
		   }*/

		   void InitializeComponent(void)
		   {
			   System::Windows::Forms::DataVisualization::Charting::ChartArea^ chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			   System::Windows::Forms::DataVisualization::Charting::Legend^ legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			   System::Windows::Forms::DataVisualization::Charting::Series^ series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			   System::Windows::Forms::DataVisualization::Charting::Series^ series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			   this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			   this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->clusteringToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->histogramExtractionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->oneDimensionalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->levelsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->euclideanToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->mahalonobisToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->morfologyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->dilationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->erosionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->erosionDilationToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->labelingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->ýmageDetectionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->objectMomentHesaplaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->homework2ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->türevHesaplaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->nonMaximumSupressionToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->hystericThresholdToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			   this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			   this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			   this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			   this->binaryEdgeImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			   this->menuStrip1->SuspendLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			   this->SuspendLayout();
			   // 
			   // menuStrip1
			   // 
			   this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			   this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				   this->fileToolStripMenuItem,
					   this->clusteringToolStripMenuItem, this->objectMomentHesaplaToolStripMenuItem, this->homework2ToolStripMenuItem
			   });
			   this->menuStrip1->Location = System::Drawing::Point(0, 0);
			   this->menuStrip1->Name = L"menuStrip1";
			   this->menuStrip1->Size = System::Drawing::Size(1425, 28);
			   this->menuStrip1->TabIndex = 0;
			   this->menuStrip1->Text = L"menuStrip1";
			   // 
			   // fileToolStripMenuItem
			   // 
			   this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			   this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			   this->fileToolStripMenuItem->Size = System::Drawing::Size(46, 24);
			   this->fileToolStripMenuItem->Text = L"File";
			   // 
			   // openToolStripMenuItem
			   // 
			   this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			   this->openToolStripMenuItem->Size = System::Drawing::Size(128, 26);
			   this->openToolStripMenuItem->Text = L"Open";
			   this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
			   // 
			   // clusteringToolStripMenuItem
			   // 
			   this->clusteringToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				   this->histogramExtractionToolStripMenuItem,
					   this->oneDimensionalToolStripMenuItem, this->morfologyToolStripMenuItem, this->labelingToolStripMenuItem, this->ýmageDetectionToolStripMenuItem
			   });
			   this->clusteringToolStripMenuItem->Name = L"clusteringToolStripMenuItem";
			   this->clusteringToolStripMenuItem->Size = System::Drawing::Size(89, 24);
			   this->clusteringToolStripMenuItem->Text = L"Clustering";
			   // 
			   // histogramExtractionToolStripMenuItem
			   // 
			   this->histogramExtractionToolStripMenuItem->Name = L"histogramExtractionToolStripMenuItem";
			   this->histogramExtractionToolStripMenuItem->Size = System::Drawing::Size(234, 26);
			   this->histogramExtractionToolStripMenuItem->Text = L"Histogram_Extraction";
			   this->histogramExtractionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::histogramExtractionToolStripMenuItem_Click);
			   // 
			   // oneDimensionalToolStripMenuItem
			   // 
			   this->oneDimensionalToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->levelsToolStripMenuItem });
			   this->oneDimensionalToolStripMenuItem->Name = L"oneDimensionalToolStripMenuItem";
			   this->oneDimensionalToolStripMenuItem->Size = System::Drawing::Size(234, 26);
			   this->oneDimensionalToolStripMenuItem->Text = L"One-Dimensional";
			   // 
			   // levelsToolStripMenuItem
			   // 
			   this->levelsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				   this->euclideanToolStripMenuItem,
					   this->mahalonobisToolStripMenuItem
			   });
			   this->levelsToolStripMenuItem->Name = L"levelsToolStripMenuItem";
			   this->levelsToolStripMenuItem->Size = System::Drawing::Size(146, 26);
			   this->levelsToolStripMenuItem->Text = L"2-Levels";
			   // 
			   // euclideanToolStripMenuItem
			   // 
			   this->euclideanToolStripMenuItem->Name = L"euclideanToolStripMenuItem";
			   this->euclideanToolStripMenuItem->Size = System::Drawing::Size(178, 26);
			   this->euclideanToolStripMenuItem->Text = L"Euclidean";
			   this->euclideanToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::euclideanToolStripMenuItem_Click);
			   // 
			   // mahalonobisToolStripMenuItem
			   // 
			   this->mahalonobisToolStripMenuItem->Name = L"mahalonobisToolStripMenuItem";
			   this->mahalonobisToolStripMenuItem->Size = System::Drawing::Size(178, 26);
			   this->mahalonobisToolStripMenuItem->Text = L"Mahalonobis";
			   // 
			   // morfologyToolStripMenuItem
			   // 
			   this->morfologyToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				   this->dilationToolStripMenuItem,
					   this->erosionToolStripMenuItem, this->erosionDilationToolStripMenuItem
			   });
			   this->morfologyToolStripMenuItem->Name = L"morfologyToolStripMenuItem";
			   this->morfologyToolStripMenuItem->Size = System::Drawing::Size(234, 26);
			   this->morfologyToolStripMenuItem->Text = L"Morfology";
			   // 
			   // dilationToolStripMenuItem
			   // 
			   this->dilationToolStripMenuItem->Name = L"dilationToolStripMenuItem";
			   this->dilationToolStripMenuItem->Size = System::Drawing::Size(204, 26);
			   this->dilationToolStripMenuItem->Text = L"Dilation";
			   this->dilationToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::dilationToolStripMenuItem_Click);
			   // 
			   // erosionToolStripMenuItem
			   // 
			   this->erosionToolStripMenuItem->Name = L"erosionToolStripMenuItem";
			   this->erosionToolStripMenuItem->Size = System::Drawing::Size(204, 26);
			   this->erosionToolStripMenuItem->Text = L"Erosion";
			   this->erosionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::erosionToolStripMenuItem_Click);
			   // 
			   // erosionDilationToolStripMenuItem
			   // 
			   this->erosionDilationToolStripMenuItem->Name = L"erosionDilationToolStripMenuItem";
			   this->erosionDilationToolStripMenuItem->Size = System::Drawing::Size(204, 26);
			   this->erosionDilationToolStripMenuItem->Text = L"Erosion+Dilation";
			   this->erosionDilationToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::erosionDilationToolStripMenuItem_Click);
			   // 
			   // labelingToolStripMenuItem
			   // 
			   this->labelingToolStripMenuItem->Name = L"labelingToolStripMenuItem";
			   this->labelingToolStripMenuItem->Size = System::Drawing::Size(234, 26);
			   this->labelingToolStripMenuItem->Text = L"Labeling";
			   this->labelingToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::labelingToolStripMenuItem_Click);
			   // 
			   // ýmageDetectionToolStripMenuItem
			   // 
			   this->ýmageDetectionToolStripMenuItem->Name = L"ýmageDetectionToolStripMenuItem";
			   this->ýmageDetectionToolStripMenuItem->Size = System::Drawing::Size(234, 26);
			   this->ýmageDetectionToolStripMenuItem->Text = L"Image Detection";
			   this->ýmageDetectionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::ýmageDetectionToolStripMenuItem_Click);
			   // 
			   // objectMomentHesaplaToolStripMenuItem
			   // 
			   this->objectMomentHesaplaToolStripMenuItem->Name = L"objectMomentHesaplaToolStripMenuItem";
			   this->objectMomentHesaplaToolStripMenuItem->Size = System::Drawing::Size(185, 24);
			   this->objectMomentHesaplaToolStripMenuItem->Text = L"Object Moment Hesapla";
			   this->objectMomentHesaplaToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::objectMomentHesaplaToolStripMenuItem_Click);
			   // 
			   // homework2ToolStripMenuItem
			   // 
			   this->homework2ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				   this->türevHesaplaToolStripMenuItem,
					   this->nonMaximumSupressionToolStripMenuItem, this->hystericThresholdToolStripMenuItem, this->binaryEdgeImageToolStripMenuItem
			   });
			   this->homework2ToolStripMenuItem->Name = L"homework2ToolStripMenuItem";
			   this->homework2ToolStripMenuItem->Size = System::Drawing::Size(104, 24);
			   this->homework2ToolStripMenuItem->Text = L"Homework2";
			   // 
			   // türevHesaplaToolStripMenuItem
			   // 
			   this->türevHesaplaToolStripMenuItem->Name = L"türevHesaplaToolStripMenuItem";
			   this->türevHesaplaToolStripMenuItem->Size = System::Drawing::Size(257, 26);
			   this->türevHesaplaToolStripMenuItem->Text = L"Edge Görüntü Oluþtur";
			   this->türevHesaplaToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::türevHesaplaToolStripMenuItem_Click);
			   // 
			   // nonMaximumSupressionToolStripMenuItem
			   // 
			   this->nonMaximumSupressionToolStripMenuItem->Name = L"nonMaximumSupressionToolStripMenuItem";
			   this->nonMaximumSupressionToolStripMenuItem->Size = System::Drawing::Size(257, 26);
			   this->nonMaximumSupressionToolStripMenuItem->Text = L"NonMaximumSupression";
			   this->nonMaximumSupressionToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::nonMaximumSupressionToolStripMenuItem_Click);
			   // 
			   // hystericThresholdToolStripMenuItem
			   // 
			   this->hystericThresholdToolStripMenuItem->Name = L"hystericThresholdToolStripMenuItem";
			   this->hystericThresholdToolStripMenuItem->Size = System::Drawing::Size(257, 26);
			   this->hystericThresholdToolStripMenuItem->Text = L"HystericThreshold";
			   this->hystericThresholdToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::hystericThresholdToolStripMenuItem_Click);
			   // 
			   // pictureBox1
			   // 
			   this->pictureBox1->Location = System::Drawing::Point(13, 39);
			   this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			   this->pictureBox1->Name = L"pictureBox1";
			   this->pictureBox1->Size = System::Drawing::Size(484, 566);
			   this->pictureBox1->TabIndex = 1;
			   this->pictureBox1->TabStop = false;
			   // 
			   // openFileDialog1
			   // 
			   this->openFileDialog1->FileName = L"openFileDialog1";
			   // 
			   // chart1
			   // 
			   chartArea1->Name = L"ChartArea1";
			   this->chart1->ChartAreas->Add(chartArea1);
			   legend1->Name = L"Legend1";
			   this->chart1->Legends->Add(legend1);
			   this->chart1->Location = System::Drawing::Point(955, 57);
			   this->chart1->Name = L"chart1";
			   series1->ChartArea = L"ChartArea1";
			   series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			   series1->Legend = L"Legend1";
			   series1->Name = L"Histogram";
			   series2->ChartArea = L"ChartArea1";
			   series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Point;
			   series2->Legend = L"Legend1";
			   series2->MarkerColor = System::Drawing::Color::Red;
			   series2->MarkerSize = 10;
			   series2->Name = L"KMeans";
			   series2->YValuesPerPoint = 2;
			   this->chart1->Series->Add(series1);
			   this->chart1->Series->Add(series2);
			   this->chart1->Size = System::Drawing::Size(470, 302);
			   this->chart1->TabIndex = 2;
			   this->chart1->Text = L"chart1";
			   this->chart1->Visible = false;
			   // 
			   // pictureBox2
			   // 
			   this->pictureBox2->Location = System::Drawing::Point(552, 39);
			   this->pictureBox2->Name = L"pictureBox2";
			   this->pictureBox2->Size = System::Drawing::Size(372, 336);
			   this->pictureBox2->TabIndex = 3;
			   this->pictureBox2->TabStop = false;
			   this->pictureBox2->Visible = false;
			   // 
			   // binaryEdgeImageToolStripMenuItem
			   // 
			   this->binaryEdgeImageToolStripMenuItem->Name = L"binaryEdgeImageToolStripMenuItem";
			   this->binaryEdgeImageToolStripMenuItem->Size = System::Drawing::Size(257, 26);
			   this->binaryEdgeImageToolStripMenuItem->Text = L"Binary Edge Image";
			   this->binaryEdgeImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::binaryEdgeImageToolStripMenuItem_Click);
			   // 
			   // Form1
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(1425, 837);
			   this->Controls->Add(this->pictureBox2);
			   this->Controls->Add(this->chart1);
			   this->Controls->Add(this->pictureBox1);
			   this->Controls->Add(this->menuStrip1);
			   this->MainMenuStrip = this->menuStrip1;
			   this->Margin = System::Windows::Forms::Padding(4);
			   this->Name = L"Form1";
			   this->Text = L"Form1";
			   this->menuStrip1->ResumeLayout(false);
			   this->menuStrip1->PerformLayout();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			   (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			   this->ResumeLayout(false);
			   this->PerformLayout();

		   }
#pragma endregion
	private: System::Void openToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		CString str;

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			//pictureBox1->ImageLocation = openFileDialog1->FileName;
			str = openFileDialog1->FileName;
			CStringA s2(str);
			const char* input = s2;
			image im = load_image(input);
			img_data = im.data;
			img_w = im.w;
			img_h = im.h;
			img_c = im.c;
			ShowRGBImages(im);
			std::cout << "h: " << im.h << "\n";
			std::cout << "w: " << im.w << "\n";
			std::cout << "c: " << im.c << "\n";
			std::cout << "data " << (int)im.data[10]<< "\n";
			std::cout << "..................................";
		}//

	}//openTool
	private: System::Void histogramExtractionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		//RGB->intensity

		if (img_data == NULL)
			MessageBox::Show("Okunacak imge öncelikle seçilmeli");
		else
		{
			image im;
			im.w = img_w;
			im.h = img_h;
			im.c = img_c;
			im.data = img_data;
			image im2 = RGBtoIntensity(im);


			ShowIntensity(im2);


			int* hist = Histogram(im2);
			//raw_data = im2.data;
			pictureBox2->Visible = false;
			chart1->Visible = true;
			chart1->Series["Histogram"]->Points->Clear();
			chart1->Series["KMeans"]->Points->Clear();

			chart1->Location = System::Drawing::Point(pictureBox1->Width + 20, 54);
			for (int i = 0; i < 256; i++)
				chart1->Series["Histogram"]->Points->AddXY(i, hist[i]);

		}

	}//histogram
	private: System::Void euclideanToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (img_data == NULL)
			MessageBox::Show("Okunacak imge öncelikle seçilmeli");
		else
		{
			image im;
			im.w = img_w;
			im.h = img_h;
			im.c = img_c;
			im.data = img_data;

			image intensity = RGBtoIntensity(im);
			image binaryImage = IntensityToBinary(intensity);

			this->chart1->Location = System::Drawing::Point(pictureBox1->Width + 20, 34);
			ShowBinary(binaryImage);

			chart1->Series["KMeans"]->Points->AddXY(binaryImage.k,0);
			chart1->Series["KMeans"]->Points->AddXY(binaryImage.k2, 0);


		}//k-means with euclidean
	}
	private: System::Void dilationToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (img_data == NULL)
			MessageBox::Show("Okunacak imge öncelikle seçilmeli");
		else
		{
			image im;
			im.w = img_w;
			im.h = img_h;
			im.c = img_c;
			im.data = img_data;

			image intensity = RGBtoIntensity(im);
			image binaryImage = IntensityToBinary(intensity);
			image dilationImage = Dilation(binaryImage);
			ShowBinary(dilationImage);

		}

	}
private: System::Void erosionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image binaryImage = IntensityToBinary(intensity);
		image erosionImage = Erosion(binaryImage);
		ShowBinary(erosionImage);

	}
}
private: System::Void erosionDilationToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {


	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image binaryImage = IntensityToBinary(intensity);
		image erosionImage = Dilation(binaryImage);
		image finalImage = Erosion(erosionImage);
		ShowBinary(finalImage);

	}
}
private: System::Void labelingToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image binaryImage = IntensityToBinary(intensity);
		image erosionImage = Dilation(binaryImage);
		image dilationImage = Erosion(erosionImage);
		image labeling = Labeling(binaryImage);
		ShowLabel(labeling);

	}


}
private: System::Void ýmageDetectionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image binaryImage = IntensityToBinary(intensity);
		image erosionImage = Dilation(binaryImage);
		image dilationImage = Erosion(erosionImage);
		image labeling = Labeling(binaryImage);
		image object = ObjectDetection(labeling);
		ShowObject(object);

		pictureBox2->Visible = true;
		chart1->Visible = false;
		pictureBox2->Width = im.w;
		pictureBox2->Height = im.h;
		pictureBox2->Location = System::Drawing::Point(pictureBox1->Width + 20, 54);
		//pictureBox2->Refresh();
		Bitmap^ surface = gcnew Bitmap(im.w, im.h);
		pictureBox2->Image = surface;
		Color c;
		int psw, bufpos;
		psw = im.w * 3;
		for (int row = 0; row < im.h; row++)
			for (int col = 0; col < im.w; col++) {
				bufpos = row * psw + col * im.c;
				c = Color::FromArgb(im.data[bufpos], im.data[bufpos + 1], im.data[bufpos + 2]); //R-G-B
				surface->SetPixel(col, row, c);
			}

	}
}
private: System::Void objectMomentHesaplaToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image binaryImage = IntensityToBinary(intensity);
		image erosionImage = Dilation(binaryImage);
		image dilationImage = Erosion(erosionImage);
		image labeling = Labeling(binaryImage);
		image object = ObjectDetection(labeling);

		ObjectMomentHesapla(object);
	}
}
private: System::Void gxHesaplaToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image edge = edgeImage(intensity);
		//ShowRGBImages(edge);

		pictureBox2->Visible = true;
		chart1->Visible = false;
		pictureBox2->Width = edge.w;
		pictureBox2->Height = edge.h;
		pictureBox2->Location = System::Drawing::Point(pictureBox1->Width + 20, 54);
		pictureBox2->Refresh();
		Bitmap^ surface = gcnew Bitmap(edge.w, edge.h);
		pictureBox2->Image = surface;
		Color c;
		int psw, bufpos;
		psw = edge.w * 1;
		for (int row = 0; row < edge.h; row++)
			for (int col = 0; col < edge.w; col++) {
				bufpos = row * psw + col ;
				c = Color::FromArgb(edge.data[bufpos], edge.data[bufpos + 1], edge.data[bufpos + 2]); //R-G-B
				surface->SetPixel(col, row, c);
			}

	}
}
private: System::Void türevHesaplaToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image edge = edgeImage(intensity);
		//ShowRGBImages(edge);

		pictureBox2->Visible = true;
		chart1->Visible = false;
		pictureBox2->Width = edge.w;
		pictureBox2->Height = edge.h;
		pictureBox2->Location = System::Drawing::Point(pictureBox1->Width + 20, 54);
		pictureBox2->Refresh();
		Bitmap^ surface = gcnew Bitmap(edge.w, edge.h);
		pictureBox2->Image = surface;
		Color c;
		int psw, bufpos;
		psw = edge.w * 1;
		for (int row = 0; row < edge.h; row++)
			for (int col = 0; col < edge.w; col++) {
				bufpos = row * psw + col;
				c = Color::FromArgb(edge.data[bufpos], edge.data[bufpos + 1], edge.data[bufpos + 2]); //R-G-B
				surface->SetPixel(col, row, c);
			}

	}
}
private: System::Void nonMaximumSupressionToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image nonMaxSupressionImage = nonMaxSuppression(intensity);
		//ShowRGBImages(edge);

		pictureBox2->Visible = true;
		chart1->Visible = false;
		pictureBox2->Width = nonMaxSupressionImage.w;
		pictureBox2->Height = nonMaxSupressionImage.h;
		pictureBox2->Location = System::Drawing::Point(pictureBox1->Width + 20, 54);
		pictureBox2->Refresh();
		Bitmap^ surface = gcnew Bitmap(nonMaxSupressionImage.w, nonMaxSupressionImage.h);
		pictureBox2->Image = surface;
		Color c;
		int psw, bufpos;
		psw = nonMaxSupressionImage.w * 1;
		for (int row = 0; row < nonMaxSupressionImage.h; row++)
			for (int col = 0; col < nonMaxSupressionImage.w; col++) {
				bufpos = row * psw + col;
				c = Color::FromArgb(nonMaxSupressionImage.data[bufpos], nonMaxSupressionImage.data[bufpos + 1], nonMaxSupressionImage.data[bufpos + 2]); //R-G-B
				surface->SetPixel(col, row, c);
			}

	}
}
private: System::Void hystericThresholdToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image hystericThresholdImage = hystericThreshold(intensity);
		//ShowRGBImages(edge);

		pictureBox2->Visible = true;
		chart1->Visible = false;
		pictureBox2->Width = hystericThresholdImage.w;
		pictureBox2->Height = hystericThresholdImage.h;
		pictureBox2->Location = System::Drawing::Point(pictureBox1->Width + 20, 54);
		pictureBox2->Refresh();
		Bitmap^ surface = gcnew Bitmap(hystericThresholdImage.w, hystericThresholdImage.h);
		pictureBox2->Image = surface;
		Color c;
		int psw, bufpos;
		psw = hystericThresholdImage.w * 1;
		for (int row = 0; row < hystericThresholdImage.h; row++)
			for (int col = 0; col < hystericThresholdImage.w; col++) {
				bufpos = row * psw + col;
				c = Color::FromArgb(hystericThresholdImage.data[bufpos], hystericThresholdImage.data[bufpos + 1], hystericThresholdImage.data[bufpos + 2]); //R-G-B
				surface->SetPixel(col, row, c);
			}

	}
}
private: System::Void binaryEdgeImageToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
	if (img_data == NULL)
		MessageBox::Show("Okunacak imge öncelikle seçilmeli");
	else
	{
		image im;
		im.w = img_w;
		im.h = img_h;
		im.c = img_c;
		im.data = img_data;

		image intensity = RGBtoIntensity(im);
		image binaryEdgeImg = binaryEdgeImage(intensity);
		//ShowBinary(binaryEdgeImg);

		pictureBox2->Visible = true;
		chart1->Visible = false;
		pictureBox2->Width = binaryEdgeImg.w;
		pictureBox2->Height = binaryEdgeImg.h;
		pictureBox2->Location = System::Drawing::Point(pictureBox1->Width + 20, 54);
		pictureBox2->Refresh();
		Bitmap^ surface = gcnew Bitmap(binaryEdgeImg.w, binaryEdgeImg.h);
		pictureBox2->Image = surface;
		Color c;
		int psw, bufpos;
		psw = binaryEdgeImg.w * 1;
		for (int row = 0; row < binaryEdgeImg.h; row++)
			for (int col = 0; col < binaryEdgeImg.w; col++) {
				bufpos = row * psw + col;
				c = Color::FromArgb(binaryEdgeImg.data[bufpos], binaryEdgeImg.data[bufpos + 1], binaryEdgeImg.data[bufpos + 2]); //R-G-B
				surface->SetPixel(col, row, c);
			}

	}
}
};

}