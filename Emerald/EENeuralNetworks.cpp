#include "EENeuralNetworks.h"
#include "EEMath.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EENeuron
	//----------------------------------------------------------------------------------------------------
	EENeuron::EENeuron()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EENeuron::~EENeuron()
	{

	}

	//----------------------------------------------------------------------------------------------------
	void EENeuron::AddDendrite(EESynapsePtr& _dendrite)
	{
		m_dendrites[_dendrite->source] = _dendrite;
	}

	//----------------------------------------------------------------------------------------------------
	void EENeuron::AddAxon(EESynapsePtr& _axon)
	{
		m_axons[_axon->target] = _axon;
	}

	//----------------------------------------------------------------------------------------------------
	bool EENeuron::RemoveDendrite(EENeuronPtr& _dendrite)
	{
		auto it = m_dendrites.find(_dendrite);
		if (it != m_dendrites.end())
		{
			m_dendrites.erase(it);
			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EENeuron::RemoveAxon(EENeuronPtr& _axon)
	{
		auto it = m_axons.find(_axon);
		if (it != m_axons.end())
		{
			m_axons.erase(it);
			return true;
		}
		return false;
	}

	// EENeuralNetworks
	//----------------------------------------------------------------------------------------------------
	EENeuralNetworks::EENeuralNetworks()
		:
		m_inputs(),
		m_outputs()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EENeuralNetworks::~EENeuralNetworks()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EENeuralNetworks::Generate(unsigned int _inputCount, unsigned int _outputCount, std::vector<unsigned int> _neuronCounts)
	{
		m_inputs.clear();
		m_outputs.clear();

		std::vector<EENeuronPtr> lowerLevel;
		std::vector<EENeuronPtr> higherLevel;

		for (unsigned int i = 0; i < _inputCount; ++i)
		{
			m_inputs.push_back(new EENeuron());
		}
		lowerLevel = m_inputs;

		for (auto count : _neuronCounts)
		{
			for (unsigned int i = 0; i < count; ++i)
			{
				higherLevel.push_back(new EENeuron());
				for (auto& neuron : lowerLevel)
				{
					Link(neuron, 1.f, higherLevel.back());
				}
			}

			lowerLevel = higherLevel;
			higherLevel.clear();
		}

		for (unsigned int i = 0; i < _outputCount; ++i)
		{
			m_outputs.push_back(new EENeuron());
			for (auto& neuron : lowerLevel)
			{
				Link(neuron, 1.f, m_outputs.back());
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EENeuralNetworks::Link(EENeuronPtr _source, float _weight, EENeuronPtr _target)
	{
		if (_source && _target)
		{
			EESynapsePtr synapse(new EESynapse(_source, _weight, _target));
			_source->AddAxon(synapse);
			_target->AddDendrite(synapse);

			return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EENeuralNetworks::Unlink(EENeuronPtr _source, EENeuronPtr _target)
	{
		if (_source && _target)
		{
			return _source->RemoveAxon(_target) && _target->RemoveDendrite(_source);
		}
		return false;
	}

	//----------------------------------------------------------------------------------------------------
	float EENeuralNetworks::LogisticSigmoid(float _input)
	{
		return 1 / (1 + pow(EE_e, -1 * _input));
	}
}