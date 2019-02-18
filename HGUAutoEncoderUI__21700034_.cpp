/***
	Copyright 2017 Injung Kim

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
***/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WINDOWS
	#include <conio.h>
#endif // _WINDOWS


#include "Console.h"

#include "HGULayer.h"
#include "HGUNeuralNetwork.h"

#include "HGUDigitImage.h"
#include "HGUAutoEncoderUI.h"

int noLayer = 2;
int aNetStruct[] = { DigitSize, 16, DigitSize };

HGUAutoEncoderUI::~HGUAutoEncoderUI()
{
	if(m_pNN != NULL){
		delete m_pNN;
		m_pNN = NULL;
	}
}

int HGUAutoEncoderUI::Run()
{
	m_windowWidth = getWindowWidth();
	m_windowHeight = getWindowHeight();

	m_sx = (m_windowWidth - DigitWidth * m_hScale) / 2;
	m_sy = (m_windowHeight - DigitHeight * m_vScale) / 2;

	// draw boundary
	DrawScreen();

	int x = 0, y = 0;
	int key = 0;

	do {
		gotoxy(m_sx + x * m_hScale, m_sy + y * m_vScale);
		key = _getch();

		if(key >= '0' && key <= '9'){
			m_digit.SetDigit(key - '0');
			DisplayDigit(m_sx, m_sy, &m_digit, m_hScale, m_vScale);
		} else if(key != 27){
			switch(key){
				case 75:		// left
				case 'h':		// left
					if(x > 0)
						x--;
					break;

				case 77:		// right
				case 'l':		// right
					if(x < DigitWidth - 1)
						x++;
					break;

				case 72:		// up
				case 'k':		// up
					if(y > 0)
						y--;
					break;

				case 80:		// down
				case 'j':		// down
					if(y < DigitHeight - 1)
						y++;
					break;

				case 83:		// del		clear
				case 'c':		// del		clear
					m_digit.Clear(0.F);
					DisplayDigit(m_sx, m_sy, &m_digit, m_hScale, m_vScale);
					break;

				case 'd':					// draw
					m_digit.Pixel(x, y) = 1.F;
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, '=');
					break;

				case 'n':
					m_digit.AddNoise(0.05);
					DisplayDigit(m_sx, m_sy, &m_digit, m_hScale, m_vScale);
					break;

				case 'e':					// erase
					m_digit.Pixel(x, y) = 0.F;
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, ' ');
					break;

				case ' ':					// toggle
					m_digit.Pixel(x, y) = 1.F - m_digit.Pixel(x, y);
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, (m_digit.Pixel(x, y) < 0.5 ? ' ' : '='));
					break;

				case 'f':
					gotoxy(1, 1);
					m_digit.DisplayAsArray();
					break;

				case 'r':
                    RestoreDigit();
					break;

				case 'i':				// reset model
					m_pNN->InitializeWeight();
					gotoxy(1, 4);
					printf("Model was reinitialized!");
					break;

				case 't':
					TrainAutoEncoder(100000, 0.F);
					DrawScreen();
					break;

				case 'T':
					TrainAutoEncoder(100000, 0.05F);
					DrawScreen();
					break;
			}
		}
	} while(key != 27 && key != 'q');

	clrscr();
	printf("Bye!\n");

	return 0;
}

void HGUAutoEncoderUI::DrawScreen()
{
    clrscr();

   	m_sx = (m_windowWidth - DigitWidth * m_hScale) / 2;
	m_sy = (m_windowHeight - DigitHeight * m_vScale) / 2;

 	if (m_sx > 0 && m_sy > 0)
		DrawBox(m_sx - 1, m_sy - 1, m_sx + DigitWidth * m_hScale, m_sy + DigitHeight * m_vScale, '#');
	DisplayDigit(m_sx, m_sy, &m_digit, m_hScale, m_vScale);

	int j = 1;
	gotoxy(2, m_sy + j++);
	printf("h, j, k, l keys: move");
	gotoxy(2, m_sy + j++);
	printf("(left, down, up, right)");

	gotoxy(2, m_sy + j++);
	printf("space: toggle pixel");

	j++;
	gotoxy(2, m_sy + j++);
	printf("0-9: set digit shape");
	gotoxy(2, m_sy + j++);
	printf("n: add 5%% noise");

	j++;
	gotoxy(2, m_sy + j++);
	printf("r: restore");

	gotoxy(2, m_sy + j++);
	printf("t: train");

	gotoxy(2, m_sy + j++);
	printf("T: train with noise");

	gotoxy(2, m_sy + j++);
	printf("i: init neural network");

	j++;
	gotoxy(2, m_sy + j++);
	printf("ESC: quit");
}

void HGUAutoEncoderUI::DisplayDigit(int sx, int sy, HGUDigitImage *pDigit, int hScale, int vScale)
{
	if (hScale == 0)
		hScale = m_hScale;
	if (vScale == 0)
		vScale = m_vScale;

	int i = 0;
	for (int y = 0; y < DigitHeight; y++){
		for (int x = 0; x < DigitWidth; x++, i++)
			DrawBigDot(sx + x * hScale, sy + y * vScale, hScale, vScale, ((*pDigit)[i] >= 0.5F ? '=' : ' '));
	}
}

void HGUAutoEncoderUI::DrawBigDot(int sx, int sy, int width, int height, char v)
{
	for (int y = 0; y < height; y++){
		gotoxy(sx, sy + y);
		for (int j = 0; j < width; j++)
			putchar(v);
	}
}

void HGUAutoEncoderUI::DrawBox(int left, int top, int right, int bottom, char v)
{
	int x = 0, y = 0;
	for (y = top; y <= bottom; y++){
		gotoxy(left, y);
		putchar(v);
		gotoxy(right, y);
		putchar(v);
	}

	gotoxy(left, top);
	for (x = left; x <= right; x++)
		putchar(v);

	gotoxy(left, bottom);
	for (x = left; x <= right; x++)
		putchar(v);
}

int HGUAutoEncoderUI::LoadAutoEncoder(int noLayer, int pNetStruct[], const char *weightFile)
{
	m_pNN = new HGUNeuralNetwork;
	if (m_pNN == NULL || m_pNN->Alloc(noLayer, pNetStruct, NULL) == FALSE){
		printf("Failed to allocate memory in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
		if (m_pNN)
			m_pNN = NULL;
		return FALSE;
	}

	if (m_pNN->Load(weightFile) == FALSE){
		gotoxy(1, 4);
		printf("Failed to load neural network from %s in %s (%s %d)\n", weightFile, __FUNCTION__, __FILE__, __LINE__);
		return FALSE;
	} else {
		gotoxy(1, 4);
		printf("Succeeded to load neural network from %s.\n", weightFile);
	}

	return TRUE;
}

int HGUAutoEncoderUI::RestoreDigit() //구현해야 될 함수1 
{
	// Propagate neural network feeding m_digit as the input
    //      use GetVector() of HGUDigitImage  (m_digit)
    //      use Propagate(float *pInput) of HGUNeuralNetwork (m_pNN)
	m_pNN->Propagate(m_digit.GetVector());

    // Copy output node values of the neural network to m_digit
    //		use GetOutput() of HGUNeuralNetwork (m_pNN)
    //		use SetImage() of HGUDigitImage (m_digit)
	m_digit.SetImage(m_pNN->GetOutput());

    // draw restored image
	if (m_sx > 0 && m_sy > 0)
		DrawBox(m_sx-1, m_sy - 1, m_sx + DigitWidth * m_hScale, m_sy + DigitHeight * m_vScale, '#');
	DisplayDigit(m_sx, m_sy, &m_digit, m_hScale, m_vScale);


	// display  values of hidden nodes
	int hiddenDim = aNetStruct[1];		// retrieve # of hidden nodes
	int sx2 = m_sx + DigitWidth * m_hScale + 2;
	int j = m_sy - 1;					// starting y coordinate
	for(int i = 0; i < hiddenDim; i++){
		gotoxy(sx2, j++);
		printf("%2d:%4.2f", i, m_pNN->GetLayerOutput(0, i));
	}

	return TRUE;
}

int HGUAutoEncoderUI::TrainAutoEncoder(int maxEpoch, float noiseProb)
{
	gotoxy(1, 1);
	printf("=== Training neural network... Start.\n");

	// allocate neural network if necessary
	if (m_pNN == NULL){
		m_pNN = new HGUNeuralNetwork;
		if (m_pNN == NULL || m_pNN->Alloc(noLayer, aNetStruct, NULL) == FALSE){
			printf("Failed to allocate neural network in %s (%s %d)\n", __FUNCTION__, __FILE__, __LINE__);
			if (m_pNN != NULL){
				delete m_pNN;
				m_pNN = NULL;
			}
			return FALSE;
		}

		m_pNN->InitializeWeight();
	}

	int epochSample = 10;
	HGUDigitImage noisySample;
	float error=0.F;
	int n=0;
	HGUDigitImage aSample[10];

	// set training data and desired labels
	for (int i = 0; i < 10; i++)
		aSample[i].SetDigit(i);

	printf("=== Training Auto-Encoder...\n");
	//	Repeat up to maxEpoch
	for(int epoch = 0; epoch < maxEpoch; epoch++) {
	//		for each sample
		for (int i = 0; i < epochSample; i++){
	// 			if noiseProb is zero, compute gradient using aSample[i] as both input and desired output
	// 				(call 'int ComputeGradient(float *pInput, float *pDesiredOutput);' on m_pNN)
			if(noiseProb==0) {m_pNN->ComputeGradient(aSample[i].GetVector(), aSample[i].GetVector());}

	// 			otherwise,
	//				generate noisySample from aSample[i] //
	//					use the "noisySample = aSample[i];" and 'void AddNoise(float flipProb);' of HGUDigitImage
	//				compute gradient using noisySample as input and aSample[i] as desired output (call ComputeGradient() on m_pNN)
			noisySample = aSample[i];
			m_digit.AddNoise(noiseProb);
			m_pNN->ComputeGradient(noisySample.GetVector(), aSample[i].GetVector());
	
	//			check MSE loss
			error = m_pNN->GetError(noisySample.GetVector());
		}
		n += epochSample;

    //		update weight by UpdateWeight(0.01F / epochSample) of HGUNeuralNetwork
		m_pNN->UpdateWeight(0.01F / epochSample);

    //  	every 1000-th epoch, print average MSE loss up to now
    //          then, flash all statistics
		if(epoch == 0 || (epoch + 1) % 1000 == 0) {
			printf("epoch = %d, loss = %f\n", epoch+1, error/n);
			n = 0;
			error = 0.F;
		}
	}

    //  Save model in ModelFile
	m_pNN->Save("ModelFile");

    //  Display message to notice training is complete.
	printf("Training is completed.\n");

	system("PAUSE");

	return TRUE;
}

int RunAutoEncoderUI()
{
	HGUAutoEncoderUI digitUI;
	digitUI.LoadAutoEncoder(noLayer, aNetStruct, ModelFile);
	digitUI.Run();

	return TRUE;
}
