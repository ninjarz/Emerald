#pragma once
#ifndef _EE_GENETICALGORITHM_H_
#define _EE_GENETICALGORITHM_H_

// #include <bitset>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <boost/any.hpp>
#include "EESmartPtr.h"
#include "EERouletteWheel.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEChromosome
	//----------------------------------------------------------------------------------------------------
	struct EEChromosome
	{
		std::vector<std::string> genes; // bits
		float fitness; // 0.f - 1.f

		//----------------------------------------------------------------------------------------------------
		inline EEChromosome(const std::vector<std::string>& _genes)
			:
			genes(_genes),
			fitness(0.f)
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline EEChromosome(const EEChromosome& _chromosome)
			:
			genes(_chromosome.genes),
			fitness(_chromosome.fitness)
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline bool operator!= (const EEChromosome& _chromosome)
		{
			return genes != _chromosome.genes;
		}
	};
	typedef EESmartPtr<EEChromosome> EEChromosomePtr;

	// EEGeneController
	//----------------------------------------------------------------------------------------------------
	class EEGeneController
	{
	public:
		EEGeneController(const std::function<float(std::vector<boost::any>)>& _fitnessFunc, int _maxPopulationSize = 2000,float _crossoverRate = 0.7,float _mutationRate = 0.1);
		virtual ~EEGeneController();

		virtual bool Epoch();
		virtual bool AddSample(const std::vector<std::string>& _genes);
		virtual void AddTranslation(std::string _gene, boost::any _meaning);

	protected:
		bool CalculateFitness(EEChromosomePtr& _chromosome);
		void Crossover();
		bool Mutate(EEChromosomePtr& _chromosome);

	protected:
		int m_maxPopulationSize;
		float m_crossoverRate;
		float m_mutationRate;
		std::map<std::string, boost::any> m_geneTranslation;
		std::function<float(std::vector<boost::any>)> m_fitnessFunc;
		// Todo: crossover func, mutation func

		// runtime
		EERouletteWheel<EEChromosomePtr> m_chromosomes;
	};
}

#endif