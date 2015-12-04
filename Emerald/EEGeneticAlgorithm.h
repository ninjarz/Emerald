#pragma once
#ifndef _EE_GENETICALGORITHM_H_
#define _EE_GENETICALGORITHM_H_

// #include <bitset>
#include <string>
#include <map>
#include <list>
#include <vector>


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

		inline float GetFitness() 
		{ 
			return m_fitness;
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
		void CalcTotalFitness();
		EEChromosome* RouletteWheel();
		void Crossover();
		void Mutate();

	protected:
		int m_maxPopulationSize;
		float m_crossoverRate;
		float m_mutationRate;
		int m_geneLength;
		std::map<std::string, std::string> m_geneTranslation;

		// runtime
		float m_totalFitness;
		std::list<EEChromosome> m_chromosomes;
	};
}

#endif