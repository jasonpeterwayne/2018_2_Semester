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
#endif	// _WINDOWS

#include "Console.h"

#include "HGULayer.h"
#include "HGUNeuralNetwork.h"

#include "HGUDigitImage.h"
#include "HGUDigitUI.h"

int noLayer = 2;
int aNetStruct[] = { DigitSize, 32, 10 };

HGUDigitUI::~HGUDigitUI()
{
	if(m_pNN != NULL){
		delete m_pNN;
		m_pNN = NULL;
	}
}

int HGUDigitUI::Run()
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
		} else if(key != 27 && key != 'q'){
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

				case 'e':					// erase
					m_digit.Pixel(x, y) = 0.F;
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, ' ');
					break;

				case ' ':					// toggle
					m_digit.Pixel(x, y) = 1.F - m_digit.Pixel(x, y);
					DrawBigDot(m_sx + x * m_hScale, m_sy + y * m_vScale, m_hScale, m_vScale, (m_digit.Pixel(x, y) == 0. ? ' ' : '='));
					break;

				case 'f':
					gotoxy(1, 1);
					m_digit.DisplayAsArray();
					break;

				case 'r':
                    RecognizeDigit();
					break;

				case 'i':				// reset model
					m_pNN->InitializeWeight();
					gotoxy(1, 4);
					printf("Model was reinitialized!");
					break;

				case 't':
					TrainRecognizer(100000);
					DrawScreen();
					break;
			}
		}
	} while(key != 27 && key != 'q');

	clrscr();
	printf("Bye!\n");

	return 0;
}

void HGUDigitUI::DrawScreen()
{
    clrscr();

   	int sx = (m_windowWidth - DigitWidth * m_hScale) / 2;
	int sy = (m_windowHeight - DigitHeight * m_vScale) / 2;

 	if (sx > 0 && sy > 0)
		DrawBox(sx - 1, sy - 1, sx + DigitWidth * m_hScale, sy + DigitHeight * m_vScale, '#');
	DisplayDigit(sx, sy, &m_digit, m_hScale, m_vScale);

	int j = 1;
	gotoxy(2, sy + j++);
	printf("h, j, k, l keys: move");
	gotoxy(2, sy + j++);
	printf("(left, down, up, right)");

	gotoxy(2, sy + j++);
	printf("space: toggle pixel");

	j++;
	gotoxy(2, sy + j++);
	printf("0-9: set digit shape");

	j++;
	gotoxy(2, sy + j++);
	printf("r: recognize");

	gotoxy(2, sy + j++);
	printf("t: train");

	gotoxy(2, sy + j++);
	printf("i: init neural network");

	j++;
	gotoxy(2, sy + j++);
	printf("ESC: quit");
}

void HGUDigitUI::DisplayDigit(int sx, int sy, HGUDigitImage *pDigit, int hScale, int vScale)
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

void HGUDigitUI::DrawBigDot(int sx, int sy, int width, int height, char v)
{
	for (int y = 0; y < height; y++){
		gotoxy(sx, sy + y);
		for (int j = 0; j < width; j++)
			putchar(v);
	}
}

void HGUDigitUI::DrawBox(int left, int top, int right, int bottom, char v)
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

int HGUDigitUI::LoadRecognizer(int noLayer, int pNetStruct[], const char *weightFile)
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

int HGUDigitUI::RecognizeDigit()
{
	/*{
		// After implementation, this part should be erased.
		gotoxy(1, 4);
		printf("Function %s was not implemented yet.\n", __FUNCTION__);
		MyPause();
		DrawScreen();
	}*/

    // Propagate neural network feeding m_digit as the input
    //      use GetVector() of HGUDigitImage  (m_digit)
    //      use Propagate(float *pInput) of HGUNeuralNetwork (m_pNN)
	m_pNN->Propagate(m_digit.GetVector());

    // Display output nodes of neural network
    //      use GetOutput(int nodeIdx) of HGUNeuralNetwork (m_pNN)
    //      use gotoxy(int x, int y) and printf() to display the output values
	int i;
	
	gotoxy(35, 27);
	for(i = 0; i < 10; i++) {
		printf("[%d] : %0.2f ", i, m_pNN->GetOutput(i));
	}
	printf("\n");

    // Display the recognition result and the score
    //      use GetMaxOutputIndex() of HGUNeuralNetwork (m_pNN) to retrieve recognition result
    //      use GetOutput(int nodeIdx) of HGUNeuralNetwork (m_pNN)
	
	gotoxy(70, 30);
	printf("Recognition : %d Score : %0.2f", m_pNN->GetMaxOutputIndex(), m_pNN->GetOutput(m_pNN->GetMaxOutputIndex()));

    return TRUE;
}

int HGUDigitUI::TrainRecognizer(int maxEpoch)
{
/*	gotoxy(1, 4);
	gotoxy(1, 4);
	printf("Function %s was not implemented yet.\n", __FUNCTION__);

    // clear screen and display a message to notice the start of training
*/
	// allocate neural network if necessary
	if(m_pNN == NULL){
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

	// training samples
	int i;
	int noSample = 10;
	HGUDigitImage aSample[10];
	float aDesiredOutput[10][10] = { 0 };

	// set training data and desired labels
	//      use SetDigit(int d) of HGUDigitImage to make training data
	//          see SetDigit() to understand how it works.
	//      fill aDesiredOutput with proper values
	
	for (i = 0; i < noSample; i++){
		aSample[i].SetDigit(i);
		aDesiredOutput[i][i] = 1.F;
	}

	
	// Repeat up to maxEpoch
    //      for each sample
    //          compute gradient using ComputeGradient() of HGUNeuralNetwork
    //          check MSE loss and recognition result by GetError() and GetMaxOutputIndex() of HGUNeuralNetwork
	float error = 0.F;
	int n = 0;
	int correct = 0;

	gotoxy(0,0);
	printf("Training start!\n");

	for(int epoch = 0; epoch < maxEpoch; epoch++) {
		for(i=0; i < noSample; i++) {
			m_pNN->ComputeGradient(aSample[i].GetVector(), aDesiredOutput[i]);

			error += m_pNN->GetError(aDesiredOutput[i]);

			if(m_pNN->GetMaxOutputIndex() == aDesiredOutput[i][1]) correct++;
		}
		n += noSample;
    //      update weight by UpdateWeight(0.01F / epochSample) of HGUNeuralNetwork
    //
		m_pNN->UpdateWeight(0.01F / noSample);

    //      every 1000-th epoch, print average MSE loss and recognition rate so far
    //          then, flash all statistics
		if(epoch == 0 || (epoch + 1) % 1000 == 0) {
			printf("epoch = %d, loss = %f, Recognition Rate = %f\n", epoch+1, error/n, correct/(float)n);
			n = 0;
			correct = 0;
			error = 0.F;
		}
	}

    //  Save model in ModelFile
	m_pNN->Save("ModelFile.wgt");

    //  Display message to notice training is complete.
	printf("Training is completed.\n");
	MyPause();

	return TRUE;
}

int RunDigitUI()
{
	HGUDigitUI digitUI;
	digitUI.LoadRecognizer(noLayer, aNetStruct, ModelFile);
	digitUI.Run();

	return TRUE;
}
