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
	class EEChromosome
	{
	public:
		EEChromosome(int _geneLength);
		virtual ~EEChromosome();

		//----------------------------------------------------------------------------------------------------
		inline float GetFitness() 
		{ 
			return m_fitness;
		}

	protected:
		bool m_isAlive;
		std::string m_gene; // bits
		float m_fitness; // 0.f - ¡Þ
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

		virtual bool Process();

	protected:
		void Crossover();
		bool Mutate(EEChromosomePtr& _chromosome);

	protected:
		int m_maxPopulationSize;
		float m_crossoverRate;
		float m_mutationRate;
		int m_geneLength;
		std::map<std::string, std::string> m_geneTranslation;

		// runtime
		EERouletteWheel<EEChromosomePtr> m_chromosomes;
	};
}

#endif