#include "EEGeneticAlgorithm.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEChromosome
	//----------------------------------------------------------------------------------------------------
	EEChromosome::EEChromosome()
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
		m_geneLength(0)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEGeneController::~EEGeneController()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::Update()
	{

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::Render()
	{
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::Process()
	{
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEGeneController::CalcTotalFitness()
	{
		m_totalFitness = 0.f;

		for (auto& chromosome : m_chromosomes)
		{
			m_totalFitness += chromosome.GetFitness();
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEChromosome* EEGeneController::RouletteWheel()
	{


		return nullptr;
	}

	//----------------------------------------------------------------------------------------------------
	void EEGeneController::Crossover()
	{
		/*
		for (auto first = m_chromosomes.begin(); first != m_chromosomes.end(); ++first)
		{
			for (auto second = ++first; second != m_chromosomes.end(); ++second)
			{
				if ((float)rand() / (float)RAND_MAX < m_crossoverRate)
				{

				}
			}
		}
		*/
	}

	//----------------------------------------------------------------------------------------------------
	void EEGeneController::Mutate()
	{

	}
}