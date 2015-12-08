#pragma once
#ifndef _EE_GENETICALGORITHM_H_
#define _EE_GENETICALGORITHM_H_

// #include <bitset>
#include <string>
#include <map>
#include <vector>
#include "EERouletteWheel.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEChromosome
	//----------------------------------------------------------------------------------------------------
	class EEChromosome
	{
	public:
		EEChromosome();
		virtual ~EEChromosome();

		//----------------------------------------------------------------------------------------------------
		inline float GetFitness() 
		{ 
			return m_fitness;
		}

		//----------------------------------------------------------------------------------------------------
		inline bool operator== (const EEChromosome& _chromosome) const
		{
			return m_isAlive == _chromosome.m_isAlive && m_gene == _chromosome.m_gene && m_fitness == _chromosome.m_fitness;
		}

		//----------------------------------------------------------------------------------------------------
		inline bool operator< (const EEChromosome& _chromosome) const
		{
			// Redundancy
			return false;
		}

	protected:
		bool m_isAlive;
		std::string m_gene; // bits
		float m_fitness; // 0.f - ¡Þ
	};

	// EEGeneController
	//----------------------------------------------------------------------------------------------------
	class EEGeneController
	{
	public:
		EEGeneController();
		virtual ~EEGeneController();

		virtual bool Update();
		virtual bool Render();
		virtual bool Process();

	protected:
		void Crossover();
		void Mutate();

	protected:
		int m_maxPopulationSize;
		float m_crossoverRate;
		float m_mutationRate;
		int m_geneLength;
		std::map<std::string, std::string> m_geneTranslation;

		// runtime
		EERouletteWheel<EEChromosome> m_chromosomes;
	};
}

#endif