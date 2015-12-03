#pragma once
#ifndef _EE_GENETICALGORITHM_H_
#define _EE_GENETICALGORITHM_H_

#include <string>
#include <map>
#include <list>


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

	protected:
		bool m_isAlive;
		std::string m_gene; // bits
		float m_fitness;
	};

	// 
	//----------------------------------------------------------------------------------------------------
	class EEGeneController
	{
	public:
		EEGeneController();
		virtual ~EEGeneController();

	protected:
		void Crossover();
		void Mutate();

	protected:
		int m_maxPopulationSize;
		float m_crossoverRate;
		float m_mutationRate;
		int m_geneLength;
		std::map<std::string, std::string> m_geneTranslation;
		std::list<EEChromosome> m_chromosomes;
	};
}

#endif