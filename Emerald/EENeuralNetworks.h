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

		void Stimulate(float _input);

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

		bool IsActivityDirty();
		float GetActivity();
		void ClearActivity();
		std::map<EENeuronPtr, EESynapsePtr>& GetDendrites();
		void AddDendrite(EESynapsePtr& _dendrite);
		void AddAxon(EESynapsePtr& _axon);
		std::map<EENeuronPtr, EESynapsePtr>& GetAxons();
		bool RemoveDendrite(EENeuronPtr& _dendrite);
		bool RemoveAxon(EENeuronPtr& _axon);

	protected:
		void AddActivity(float _activity);
		float LogisticSigmoid(float _input);

	protected:
		bool m_isActivityDirty;
		float m_activity;
		unsigned int m_stimulatedTimes;
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
		bool BPTrain(const std::vector<float>& _inputs, const std::vector<float>& _outputs); // Back Propagation
		std::vector<float> Stimulate(const std::vector<float>& _inputs);

	protected:
		void ClearActivity();
		void ClearActivity(EENeuronPtr _neuron);
		bool Link(EENeuronPtr _source, float _weight, EENeuronPtr _target);
		bool Unlink(EENeuronPtr _source, EENeuronPtr _target);

	protected:
		std::vector<EENeuronPtr> m_inputs;
		std::vector<EENeuronPtr> m_outputs;
	};
}

#endif