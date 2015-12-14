#pragma once
#ifndef _EE_NEURALNETWORKS_H_
#define _EE_NEURALNETWORKS_H_

#include <vector>
#include <list>
#include "EESmartPtr.h"


// Option: time-based
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	struct EEDendrite;
	typedef EESmartPtr<EEDendrite> EEDendritePtr;
	struct EEAxon;
	typedef EESmartPtr<EEAxon> EEAxonPtr;
	class EENeuron;
	typedef EESmartPtr<EENeuron> EENeuronPtr;

	// EEDendrite (Input)
	//----------------------------------------------------------------------------------------------------
	struct EEDendrite
	{
		float weight;
		EENeuronPtr parent;

		//----------------------------------------------------------------------------------------------------
		inline EEDendrite(float _weight, EENeuronPtr _neuron)
			:
			weight(_weight),
			parent(_neuron)
		{}
	};

	// EEAxon (Output)
	//----------------------------------------------------------------------------------------------------
	struct EEAxon
	{
		EEDendritePtr target;

		//----------------------------------------------------------------------------------------------------
		inline EEAxon(EEDendritePtr _dendrite)
			:
			target(_dendrite)
		{}
	};

	// EENeuron
	//----------------------------------------------------------------------------------------------------
	class EENeuron
	{
	public:
		void Link(EENeuronPtr& _target, float _weight);
		void Unlink(EENeuronPtr& _target);

	protected:
		void AddDendrite(EEDendritePtr& _dendrite);
		void AddAxon(EEAxonPtr& _axon);

	protected:
		std::list<EEDendritePtr> m_dendrites;
		std::list<EEAxonPtr> m_axons;
	};

	// NeuralNetworks
	//----------------------------------------------------------------------------------------------------
	class EENeuralNetworks
	{
	public:
		EENeuralNetworks();
		~EENeuralNetworks();

		/*
		bool Train(const std::vector<float>& _input, const std::vector<float>& _outputs);
		std::vector<float> Stimulate(const std::vector<float>& _input);
		*/

	protected:
		float LogisticSigmoid(float _input);

	protected:
		std::vector<EEDendrite> m_inputs;
		std::vector<EENeuronPtr> m_neurons;
		std::vector<EEAxon> m_outputs;
	};
}

#endif