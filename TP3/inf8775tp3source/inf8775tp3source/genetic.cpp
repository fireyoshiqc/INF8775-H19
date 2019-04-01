#include <algorithm>
#include "genetic.h"

GeneticIndividual generateRandomIndividual(Problem & problem)
{
	GeneticIndividual individual;
	std::vector<int> budget = problem.availablePieceBudget;
	individual.modelGenes = std::vector<int>(problem.nModels);
	bool areAllUsed = false;
	while (!areAllUsed)
	{
		size_t selectIdx = rand() % problem.nModels;
		individual.modelGenes[selectIdx]++;
		for (size_t i = 0; i < budget.size(); i++)
		{
			budget[i] -= problem.models[selectIdx].modelCosts[i];
		}
		areAllUsed = std::all_of(budget.begin(), budget.end(), [](int amount) { return amount <= 0; });
	}

	return individual;
}

