#pragma once
#ifndef _EE_NEURALNETWORKS_H_
#define _EE_NEURALNETWORKS_H_

#include <vector>
#include <list>
#include <map>
#include "EESmartPtr.h"


// Option: time-based
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
		EENeuronPtr source;
		float weight;
		EENeuronPtr target;

		//----------------------------------------------------------------------------------------------------
		inline EESynapse(EENeuronPtr _source, float _weight, EENeuronPtr _target)
			:
			source(_source),
			weight(_weight),
			target(_target)
		{}
	};

	// EENeuron (Core)
	//----------------------------------------------------------------------------------------------------
	class EENeuron
	{
	public:
		EENeuron();
		virtual ~EENeuron();

		bool IsActivityDirty();
		float GetActivity();
		void AddActivity(float _activity);
		void ClearActivity();
		std::map<EENeuronPtr, EESynapsePtr>& GetDendrites();
		void AddDendrite(EESynapsePtr& _dendrite);
		void AddAxon(EESynapsePtr& _axon);
		std::map<EENeuronPtr, EESynapsePtr>& GetAxons();
		bool RemoveDendrite(EENeuronPtr& _dendrite);
		bool RemoveAxon(EENeuronPtr& _axon);

	protected:
		bool m_isActivityDirty;
		float m_activity;
		std::map<EENeuronPtr, EESynapsePtr> m_dendrites; // Input
		std::map<EENeuronPtr, EESynapsePtr> m_axons; // Output
	};

	// NeuralNetworks
	//----------------------------------------------------------------------------------------------------
	class EENeuralNetworks
	{
	public:
		EENeuralNetworks();
		virtual ~EENeuralNetworks();

		bool Generate(unsigned int _inputCount, unsigned int _outputCount, std::vector<unsigned int> _neuronCounts); // Feedforword
		bool BPTrain(const std::vector<float>& _input, const std::vector<float>& _outputs); // Back Propagation
		std::vector<float> Stimulate(const std::vector<float>& _input);

	protected:
		void ClearActivity();
		void ClearActivity(EENeuronPtr _neuron);
		bool Link(EENeuronPtr _source, float _weight, EENeuronPtr _target);
		bool Unlink(EENeuronPtr _source, EENeuronPtr _target);
		float LogisticSigmoid(float _input);

	protected:
		std::vector<EENeuronPtr> m_inputs;
		std::vector<EENeuronPtr> m_outputs;
	};
}

#endif