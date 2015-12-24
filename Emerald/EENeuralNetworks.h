#pragma once
#ifndef _EE_NEURALNETWORKS_H_
#define _EE_NEURALNETWORKS_H_

#include <vector>
#include <list>
#include <map>
#include "EESmartPtr.h"


// Option: time-based
// Warnning: loop
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	struct EESynapse;
	typedef EESmartPtr<EESynapse> EESynapsePtr;
	class EENeuron;
	typedef EESmartPtr<EENeuron> EENeuronPtr;

	// EESynapse (Input & Output)
	//----------------------------------------------------------------------------------------------------
	struct EESynapse
	{
	public:
		EESynapse(EENeuronPtr _source, float _weight, EENeuronPtr _target);

		void Activate(float _input);

	public:
		EENeuronPtr source;
		float weight;
		EENeuronPtr target;
	};

	// EENeuron (Core)
	//----------------------------------------------------------------------------------------------------
	class EENeuron
	{
	public:
		EENeuron();
		virtual ~EENeuron();

		void Stimulate(float _input);
		float Activate();

		float GetActivity();
		bool IsStimulusDirty();
		void ClearStimulus();
		std::map<EENeuronPtr, EESynapsePtr>& GetDendrites();
		void AddDendrite(EESynapsePtr& _dendrite);
		void AddAxon(EESynapsePtr& _axon);
		std::map<EENeuronPtr, EESynapsePtr>& GetAxons();
		bool RemoveDendrite(EENeuronPtr& _dendrite);
		bool RemoveAxon(EENeuronPtr& _axon);

		// Learning
		void AdjustWeights(float _error);

	protected:
		void AddStimulus(float _stimulus);
		float Sigmoid(float _input);
		float SigmoidD(float _input);

	protected:
		float m_activity;
		bool m_isStimulusDirty;
		float m_stimulus;
		unsigned int m_stimulatedTimes;
		std::map<EENeuronPtr, EESynapsePtr> m_dendrites; // Input
		std::map<EENeuronPtr, EESynapsePtr> m_axons; // Output

		// Learning
		float m_learningRate;
	};

	// NeuralNetworks
	//----------------------------------------------------------------------------------------------------
	class EENeuralNetworks
	{
	public:
		EENeuralNetworks();
		virtual ~EENeuralNetworks();

		bool Generate(unsigned int _inputCount, unsigned int _outputCount, std::vector<unsigned int> _neuronCounts); // Feedforword
		std::vector<float> Stimulate(const std::vector<float>& _inputs);

		// Learning
		bool BPTrain(const std::vector<float>& _inputs, const std::vector<float>& _outputs); // Back Propagation

	protected:
		void ClearStimulus();
		void ClearStimulus(EENeuronPtr _neuron);
		bool Link(EENeuronPtr _source, float _weight, EENeuronPtr _target);
		bool Unlink(EENeuronPtr _source, EENeuronPtr _target);

	protected:
		std::vector<EENeuronPtr> m_inputs;
		std::vector<EENeuronPtr> m_outputs;
	};
}

#endif