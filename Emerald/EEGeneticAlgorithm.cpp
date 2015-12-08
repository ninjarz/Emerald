#include "EEGeneticAlgorithm.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEChromosome
	//----------------------------------------------------------------------------------------------------
	EEChromosome::EEChromosome(int _geneLength)
		:
		m_gene('0', _geneLength)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEChromosome::~EEChromosome()
	{

	}

	// EEGeneController
	//----------------------------------------------------------------------------------------------------
	EEGeneController::EEGeneController()
		:
		m_maxPopulationSize(0),
		m_crossoverRate(0.f),
		m_mutationRate(0.f),
		m_geneLength(0),
		m_geneTranslation(),
		m_chromosomes()
	{
		// Init
	}

	//----------------------------------------------------------------------------------------------------
	EEGeneController::~EEGeneController()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::Process()
	{
		Crossover();

		for (auto& chromosome : m_chromosomes)
		{

		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	void EEGeneController::Crossover()
	{
		EEChromosomePtr mom, dad; // selfing X
		if (!m_chromosomes.RandomSelect(mom) || !m_chromosomes.RandomSelect(dad))
			return;

		
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::Mutate(EEChromosomePtr& _chromosome)
	{
		return true;
	}
}