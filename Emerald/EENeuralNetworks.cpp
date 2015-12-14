#include "EENeuralNetworks.h"
#include "EEMath.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EENeuron
	//----------------------------------------------------------------------------------------------------
	void EENeuron::Link(EENeuronPtr& _target, float _weight)
	{
		EEDendritePtr dendrite(new EEDendrite(_weight, _target));
		_target->AddDendrite(dendrite);

		EEAxonPtr axon(new EEAxon(dendrite));
		AddAxon(axon);
	}
	
	//----------------------------------------------------------------------------------------------------
	void EENeuron::Unlink(EENeuronPtr& _target)
	{
		
	}

	//----------------------------------------------------------------------------------------------------
	void EENeuron::AddDendrite(EEDendritePtr& _dendrite)
	{
		m_dendrites.push_back(_dendrite);
	}

	//----------------------------------------------------------------------------------------------------
	void EENeuron::AddAxon(EEAxonPtr& _axon)
	{
		m_axons.push_back(_axon);
	}

	// EENeuralNetworks
	//----------------------------------------------------------------------------------------------------
	EENeuralNetworks::EENeuralNetworks()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EENeuralNetworks::~EENeuralNetworks()
	{

	}

	//----------------------------------------------------------------------------------------------------
	float EENeuralNetworks::LogisticSigmoid(float _input)
	{
		return 1 / (1 + pow(EE_e, -1 * _input));
	}
}