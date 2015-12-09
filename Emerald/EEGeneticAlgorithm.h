#pragma once
#ifndef _EE_GENETICALGORITHM_H_
#define _EE_GENETICALGORITHM_H_

// #include <bitset>
#include <string>
#include <map>
#include <vector>
#include "EESmartPtr.h"
#include "EERouletteWheel.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEChromosome
	//----------------------------------------------------------------------------------------------------
	struct EEChromosome
	{
		bool isAlive;
		std::vector<std::string> genes; // bits
		float fitness; // 0.f - ¡Þ

		inline EEChromosome(int _geneAmount)
			:
			isAlive(true),
			genes(_geneAmount),
			fitness(0.f)
		{
		}

		inline EEChromosome(const EEChromosome& _chromosome)
			:
			isAlive(_chromosome.isAlive),
			genes(_chromosome.genes),
			fitness(_chromosome.fitness)
		{
		}
	};

	// EEChromosomePtr
	//----------------------------------------------------------------------------------------------------
	typedef EESmartPtr<EEChromosome> EEChromosomePtr;

	// EEGeneController
	//----------------------------------------------------------------------------------------------------
	class EEGeneController
	{
	public:
		EEGeneController();
		virtual ~EEGeneController();

		virtual bool Epoch();

	protected:
		bool CalculateFitness(EEChromosomePtr& _chromosome);
		void Crossover();
		bool Mutate(EEChromosomePtr& _chromosome);

	protected:
		int m_maxPopulationSize;
		float m_crossoverRate;
		float m_mutationRate;
		int m_geneAmount;
		float m_geneMutationRate; // Depend on the mutation rate and the amount of genes
		std::map<std::string, std::string> m_geneTranslation;

		// runtime
		EERouletteWheel<EEChromosomePtr> m_chromosomes;
	};
}

#endif