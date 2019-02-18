/***
	Copyright 2012 Injung Kim

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

#ifndef __HGULayer__
#define __HGULayer__

#ifndef TRUE
	#define TRUE 1
	#define FALSE 0
#endif // !TRUE

class HGULayer {
protected:
	int m_inputDim;
	int m_outputDim;

	float *m_pInput;	// size: m_inputDim
	float *m_aOutput;	// size: m_outputDim
	float *m_aWeight;	// size: (m_inputDim + 1 ) * m_outputDim
	int m_weightDim;
	unsigned char m_bWeightShared;

	// only for training
	float *m_aGradient;	// size: (m_inputDim + 1 ) * m_outputDim
	float *m_aDelta;	// size: m_outputDim
	float *m_aDeltaBar;	// size: m_outputDim

public:
	HGULayer();
	HGULayer(int inputDim, int outputDim, HGULayer *pShareSrc);

	virtual ~HGULayer()			{ Delete();	}

	int IsAllocated()			{ return m_aWeight != NULL; }
	virtual int Alloc(int inputDim, int outputDim, HGULayer *pShareSrc);
	void Delete();

	void InitializeWeight();		// Xavier initialization

	virtual int Load(FILE *fp);
	virtual int Save(FILE *fp);

	int GetInputDim()			{ return m_inputDim; }
	int GetOutputDim()			{ return m_outputDim; }
	float* GetInput()			{ return m_pInput; }
	float* GetOutput()			{ return m_aOutput; }
	float GetOutput(int idx)	{ return m_aOutput[idx]; }
	float* GetWeight()			{ return m_aWeight; }
	float* GetGradient()		{ return m_aGradient; }
	float* GetDeltaBar()		{ return m_aDeltaBar; }
	int IsWeightShared()		{ return m_bWeightShared; }
	void SetWeightShared(int flag)	{ m_bWeightShared = (unsigned char) flag; }

	int Propagate(float *pInput);
	virtual float GetError(float *pDesiredOutput);
	int GetMaxOutputIndex();

	int ComputeGradient(float *pInput, float *pDesiredOutput);
	int ComputeDeltaBar(float *pDesiredOutput);
	int ComputeGradientFromDeltaBar();
	int Backpropagate(float *pPrevDeltaBar);

	virtual int UpdateWeight(float learningRate);
	int UpdateBias(float learningRate);			// for autoencoder

	float Activation(float net)		{ return 1.F/(1.F + (float)exp(-net)); }
	float DerActivationFromOutput(float output){ return output * (1.F-output); }
	float DerActivation(float net)	{ return DerActivationFromOutput(Activation(net)); }

	virtual void ResetGradient();
	virtual void MergeGradient(HGULayer *pSrc);
};

void PropagateOnDevice(float *pInput, float *pWeight, int inDim, int outDim, float *pOutput);		// CUDA Code

#endif // !__HGULayer__
