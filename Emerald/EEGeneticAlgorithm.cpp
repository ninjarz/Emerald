#include "EEGeneticAlgorithm.h"
#include <time.h>


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEGeneController
	//----------------------------------------------------------------------------------------------------
	EEGeneController::EEGeneController(const std::function<float(std::vector<boost::any>)>& _fitnessFunc)
		:
		m_maxPopulationSize(20),
		m_crossoverRate(0.7f),
		m_mutationRate(0.01f),
		m_geneTranslation(),
		m_fitnessFunc(_fitnessFunc),
		m_chromosomes()
	{
		srand((unsigned int)time(0));
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
		if (m_maxPopulationSize > 0)
		{
			while ((unsigned int)m_maxPopulationSize < m_chromosomes.Size())
				m_chromosomes.PopFront();
		}

		Crossover();

		if (m_chromosomes.begin() != m_chromosomes.end())
		{
			if (m_chromosomes.Back()->fitness == 1.f)
				return false;
			else
				return true;
		}
		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::AddSample(const std::vector<std::string>& _genes)
	{
		EEChromosomePtr sample(new EEChromosome(_genes));

		if (CalculateFitness(sample))
		{
			m_chromosomes.Insert(sample->fitness, sample);
			return true;
		}
		
		return false;
	}

	//----------------------------------------------------------------------------------------------------
	void EEGeneController::AddTranslation(std::string _gene, boost::any _meaning)
	{
		m_geneTranslation[_gene] = _meaning;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGeneController::CalculateFitness(EEChromosomePtr& _chromosome)
	{
		std::vector<boost::any> meanings;
		for (auto& gene : _chromosome->genes)
		{
			if (m_geneTranslation.find(gene) != m_geneTranslation.end())
				meanings.push_back(m_geneTranslation[gene]);
			else
				return false;
		}

		_chromosome->fitness = m_fitnessFunc(meanings);

		return _chromosome->fitness > 0;
	}

	//----------------------------------------------------------------------------------------------------
	void EEGeneController::Crossover()
	{
		EEChromosomePtr dad, mom; // Todo: selfing X
		if (!m_chromosomes.RandomSelect(dad) || !m_chromosomes.RandomSelect(mom))
			return;

		EEChromosomePtr son(new EEChromosome(*dad)), daughter(new EEChromosome(*mom));
		if (((float)rand() / (float)RAND_MAX) <= m_crossoverRate && mom != dad)
		{
			for (unsigned int i = 0; i < son->genes.size(); ++i)
			{
				if (((float)rand() / (float)RAND_MAX) <= 0.5f)
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
		float geneMutationRate = float(1 - pow(1 - m_mutationRate, 1.0 / (double)_chromosome->genes.size()));
		for (auto& gene : _chromosome->genes)
		{
			if (((float)rand() / (float)RAND_MAX) <= geneMutationRate)
			{
				for (unsigned int i = 0; i < gene.size(); ++i)
				{
					if (((float)rand() / (float)RAND_MAX) <= 0.5f)
					{
						gene[i] = gene[i] == '0' ? '1' : '0';
					}
				}
			}
		}

		return true;
	}
}