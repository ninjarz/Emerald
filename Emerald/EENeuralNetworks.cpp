#include "EENeuralNetworks.h"
#include "EEMath.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EESynapse (Input & Output)
	//----------------------------------------------------------------------------------------------------
	EESynapse::EESynapse(EENeuronPtr _source, float _weight, EENeuronPtr _target)
		:
		source(_source),
		weight(_weight),
		target(_target)
	{}

	//----------------------------------------------------------------------------------------------------
	void EESynapse::Stimulate(float _input)
	{
		target->Stimulate(_input * weight);
	}



	// EENeuron
	//----------------------------------------------------------------------------------------------------
	EENeuron::EENeuron()
		:
		m_isActivityDirty(false),
		m_activity(0.f),
		m_stimulatedTimes(0)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EENeuron::~EENeuron()
	{

	}

	// 
	//----------------------------------------------------------------------------------------------------
	void EENeuron::Stimulate(float _input)
	{
		AddActivity(_input);

		if (m_dendrites.size() <= m_stimulatedTimes) // Memo: for feedforword
		{
			if (m_axons.size())
			{
				for (auto& axon : m_axons)
				{
					axon.second->Stimulate(LogisticSigmoid(_input));
				}

				ClearActivity();
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	bool EENeuron::IsActivityDirty()
	{
		return m_isActivityDirty;
	}

	//----------------------------------------------------------------------------------------------------
	float EENeuron::GetActivity()
	{
		return m_activity;
	}

	//----------------------------------------------------------------------------------------------------
	void EENeuron::ClearActivity()
	{
		m_activity = 0.f;
		m_isActivityDirty = false;
		m_stimulatedTimes = 0;
	}

	//----------------------------------------------------------------------------------------------------
	std::map<EENeuronPtr, EESynapsePtr>& EENeuron::GetDendrites()
	{
		return m_dendrites;
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
	std::map<EENeuronPtr, EESynapsePtr>& EENeuron::GetAxons()
	{
		return m_axons;
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

	//----------------------------------------------------------------------------------------------------
	void EENeuron::AddActivity(float _activity)
	{
		m_activity += _activity;
		m_isActivityDirty = true;
		++m_stimulatedTimes;
	}

	//----------------------------------------------------------------------------------------------------
	float EENeuron::LogisticSigmoid(float _input)
	{
		return 1 / (1 + pow(EE_e, -1 * _input));
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
	bool EENeuralNetworks::BPTrain(const std::vector<float>& _inputs, const std::vector<float>& _outputs)
	{
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<float> EENeuralNetworks::Stimulate(const std::vector<float>& _inputs)
	{
		if (_inputs.size() != m_inputs.size())
		{
			return std::vector<float>();
		}

		ClearActivity();

		for (unsigned int i = 0; i < m_inputs.size(); ++i)
		{
			m_inputs[i]->Stimulate(_inputs[i]);
		}

		std::vector<float> outputs;
		for (auto& ouput : m_outputs)
		{
			outputs.push_back(ouput->GetActivity());
		}

		return outputs;
	}

	//----------------------------------------------------------------------------------------------------
	void EENeuralNetworks::ClearActivity()
	{
		for (auto& neuron : m_inputs)
		{
			if (neuron->IsActivityDirty())
			{
				ClearActivity(neuron);
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EENeuralNetworks::ClearActivity(EENeuronPtr _neuron)
	{
		if (_neuron->IsActivityDirty())
		{
			_neuron->ClearActivity();
			for (auto& axon : _neuron->GetAxons())
			{
				ClearActivity(axon.first);
			}
		}
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
}