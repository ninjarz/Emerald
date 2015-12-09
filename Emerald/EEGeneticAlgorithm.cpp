#include "EEGeneticAlgorithm.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEGeneController
	//----------------------------------------------------------------------------------------------------
	EEGeneController::EEGeneController()
		:
		m_maxPopulationSize(0),
		m_crossoverRate(0.7f),
		m_mutationRate(0.1f),
		m_geneAmount(500),
		m_geneMutationRate(0.f),
		m_geneTranslation(),
		m_chromosomes()
	{
		if (m_geneAmount)
			m_geneMutationRate = float(1 - pow(1 - m_mutationRate, 1.0 / (double)m_geneAmount));
		// m_chromosomes.Insert(10.f, new EEChromosome(5));
	}

	//----------------------------------------------------------------------------------------------------
	EEGeneController::~EEGeneController()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::Epoch()
	{
		/*
		for (auto& chromosome : m_chromosomes)
		{
			if (!Update(chromosome))
				m_chromosomes.Delete(chromosome->fitness, chromosome);
		}
		*/

		Crossover();

		return m_chromosomes.begin() != m_chromosomes.end();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::CalculateFitness(EEChromosomePtr& _chromosome)
	{
		// Todo: use lambda

		return _chromosome->isAlive;
	}

	//----------------------------------------------------------------------------------------------------
	void EEGeneController::Crossover()
	{
		EEChromosomePtr dad, mom; // Todo: selfing X
		if (!m_chromosomes.RandomSelect(dad) || !m_chromosomes.RandomSelect(mom))
			return;

		EEChromosomePtr son(new EEChromosome(*dad)), daughter(new EEChromosome(*mom));
		if (m_crossoverRate <= ((float)rand() / (float)RAND_MAX) && mom != dad)
		{
			for (unsigned int i = 0; i < son->genes.size(); ++i)
			{
				if (0.5f <= ((float)rand() / (float)RAND_MAX))
				{
					std::string tmp = son->genes[i];
					son->genes[i] = daughter->genes[i];
					daughter->genes[i] = tmp;
				}
			}
		}

		Mutate(son);
		Mutate(daughter);
		if (CalculateFitness(son))
			m_chromosomes.Insert(son->fitness, son);
		if (CalculateFitness(daughter))
			m_chromosomes.Insert(daughter->fitness, daughter);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::Mutate(EEChromosomePtr& _chromosome)
	{
		for (auto& gene : _chromosome->genes)
		{
			if (m_geneMutationRate <= ((float)rand() / (float)RAND_MAX))
			{
				for (unsigned int i = 0; i < gene.size(); ++i)
				{
					if (0.5f <= ((float)rand() / (float)RAND_MAX))
					{
						gene[i] = gene[i] == '0' ? '1' : '0';
					}
				}
			}
		}

		return true;
	}
}