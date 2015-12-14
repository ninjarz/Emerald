#pragma once
#ifndef _EE_NEURALNETWORKS_H_
#define _EE_NEURALNETWORKS_H_

#include <vector>
#include "EESmartPtr.h"


// Option: time-based
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	struct EEDendrite;
	typedef EESmartPtr<EEDendrite> EEDendritePtr;
	struct EEAxon;
	typedef EESmartPtr<EEAxon> EEAxonPtr;
	struct EENeuron;
	typedef EESmartPtr<EENeuron> EENeuronPtr;

	// Dendrite (Input)
	//----------------------------------------------------------------------------------------------------
	struct EEDendrite
	{
		float weight;
		EENeuronPtr parent;

		inline EEDendrite(float _weight, EENeuronPtr _neuron)
			:
			weight(_weight),
			parent(_neuron)
		{
		}
	};

	// Axon (Output)
	//----------------------------------------------------------------------------------------------------
	struct EEAxon
	{
		float result;
		EENeuronPtr target;
	};

	// Neuron
	//----------------------------------------------------------------------------------------------------
	struct EENeuron
	{
		std::vector<EEDendritePtr> dendrites;
		std::vector<EEAxonPtr> axons;
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
		std::vector<EEDendrite> m_inputs;
		std::vector<EENeuronPtr> m_neurons;
		std::vector<EEAxon> m_outputs;
	};
}

#endif