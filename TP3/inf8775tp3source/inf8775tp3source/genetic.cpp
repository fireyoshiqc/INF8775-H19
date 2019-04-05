#include <algorithm>
#include "genetic.h"

GeneticIndividual generateRandomIndividual(const Problem & problem) {
	GeneticIndividual individual;
	individual.budget = problem.availablePieceBudget;
	individual.modelGenes = std::vector<int> (problem.nModels);
	bool areAllUsed = false;
	while (!areAllUsed)
	{
		size_t selectIdx = rand() % problem.nModels;
		individual.modelGenes[selectIdx]++;
		for (size_t i = 0; i < individual.budget.size(); i++)
		{
			individual.budget[i] -= problem.models[selectIdx].modelCosts[i];
		}
		

		areAllUsed = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= 0; });
	}
	

	individual.totalCost = abs(std::accumulate(individual.budget.begin(), individual.budget.end(), 0));
	

	return individual;
}

GeneticPopulation generateRandomPopulation(const Problem & problem) {
	GeneticPopulation population;
	population.individuals.reserve(POPULATION_SIZE);
	for (size_t i = 0; i < POPULATION_SIZE; i++)
		population.individuals.emplace_back(generateRandomIndividual(problem));
	return population;
}

void switchIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t addModelIdx, size_t subModelIdx) {
	individual.modelGenes[addModelIdx] ++;
	individual.modelGenes[subModelIdx] --;

	// update budget
	for (int i = 0; i < individual.budget.size(); i++)
	{
		individual.budget[i] -= problem.models[addModelIdx].modelCosts[i];
	}

	for (int i = 0; i < individual.budget.size(); i++)
	{
		individual.budget[i] += problem.models[subModelIdx].modelCosts[i];
	}

	//update cost (or "fitness")
	individual.totalCost += std::accumulate(begin(problem.models[addModelIdx].modelCosts), end(problem.models[addModelIdx].modelCosts), 0);
	individual.totalCost -= std::accumulate(begin(problem.models[subModelIdx].modelCosts), end(problem.models[subModelIdx].modelCosts), 0);

}

void addIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t index) {
	individual.modelGenes[index] ++;

	// update budget
	for (size_t i = 0; i < individual.budget.size(); i++)
	{
		individual.budget[i] -= problem.models[index].modelCosts[i];
	}

	//update cost
	individual.totalCost += std::accumulate(begin(problem.models[index].modelCosts), end(problem.models[index].modelCosts), 0);
}

void mutateIndividual(GeneticIndividual & individual, const Problem & problem) {
	size_t addModelIdx, subModelIdx;
	GeneticIndividual newIndividual = individual;
	// switch MINIMAL_MUTATION_ITERATIONS models 
	for (int i = 0; i < MINIMAL_MUTATION_ITERATIONS; i++) 
	{

		//find valid random genes to modify
		addModelIdx = rand() % newIndividual.modelGenes.size();
		do {
			subModelIdx = rand() % newIndividual.modelGenes.size();
		} while (newIndividual.modelGenes[subModelIdx] == 0);

		switchIndividualGenes(newIndividual, problem, addModelIdx, subModelIdx);

	}

	if (newIndividual.totalCost > individual.totalCost) {
		return; // If the new indivdual is already worst when it is not even valid yet, stop. 
	}
	
	// add models until individual is valid
	bool areAllUsed = std::all_of(newIndividual.budget.begin(), newIndividual.budget.end(), [](int amount) { return amount <= 0; });
	bool isCostGreater = false;
	while (!areAllUsed)
	{
		addModelIdx = rand() % newIndividual.modelGenes.size();
		addIndividualGenes(newIndividual, problem, addModelIdx);
		areAllUsed = std::all_of(newIndividual.budget.begin(), newIndividual.budget.end(), [](int amount) { return amount <= 0; });
		if (newIndividual.totalCost > individual.totalCost) {
			return; // If the new indivdual is already worst when it is not even valid yet, stop. 
		}
	}
	individual = newIndividual;
}

void alterIndividual(GeneticIndividual & individual, const Problem & problem) {
	size_t addModelIdx, subModelIdx;
	GeneticIndividual newIndividual = individual;
	// switch MINIMAL_MUTATION_ITERATIONS models 
	for (int i = 0; i < MINIMAL_ATLER_ITERATIONS; i++)
	{
		//find valid random genes to modify
		addModelIdx = rand() % newIndividual.modelGenes.size();
		do {
			subModelIdx = rand() % newIndividual.modelGenes.size();
		} while (newIndividual.modelGenes[subModelIdx] == 0);

		switchIndividualGenes(newIndividual, problem, addModelIdx, subModelIdx);
	}

	// add models until individual is valid
	bool areAllUsed = std::all_of(newIndividual.budget.begin(), newIndividual.budget.end(), [](int amount) { return amount <= 0; });
	bool isCostGreater = false;
	while (!areAllUsed)
	{
		addModelIdx = rand() % newIndividual.modelGenes.size();
		addIndividualGenes(newIndividual, problem, addModelIdx);
		areAllUsed = std::all_of(newIndividual.budget.begin(), newIndividual.budget.end(), [](int amount) { return amount <= 0; });
	}
	individual = newIndividual;
}

bool compareByCost(const GeneticIndividual &a, const GeneticIndividual &b)
{
	return a.totalCost < b.totalCost;
}

void sortPopulation(GeneticPopulation & population) {
	std::sort(population.individuals.begin(), population.individuals.end(), compareByCost);
}

std::vector<GeneticIndividual> selectSurvivors(GeneticPopulation & population) {
	sortPopulation(population);
	std::vector<GeneticIndividual> survivors(NUMBER_OF_SURVIVORS);
	survivors = slice(population.individuals, 0, NUMBER_OF_SURVIVORS);
	return survivors;
}

void genocide(GeneticPopulation & population, std::vector<GeneticIndividual> survivors, const Problem & problem) {
	for (int i = NUMBER_OF_SURVIVORS; i < POPULATION_SIZE; i++) {
		int parentIdx = rand() % NUMBER_OF_SURVIVORS;
		GeneticIndividual newIndividual = survivors[parentIdx];
		alterIndividual(newIndividual, problem);
		population.individuals[i] = newIndividual;
	}
}

void printIndividual(GeneticIndividual ind) {
	std::cout << "Best individual cost: " << ind.totalCost << std::endl;
	std::cout << "Best individual solution: ";
	for (int i = 0; i < ind.modelGenes.size(); i++) {
		std::cout << ind.modelGenes[i] << " ";
	}
	std::cout << std::endl;
}

void evolve(GeneticPopulation & population, const Problem & problem) {
	for (int i = 0; i < POPULATION_SIZE; i++) {
		mutateIndividual(population.individuals[i], problem);
	}
	std::vector<GeneticIndividual> survivors = selectSurvivors(population);

	printIndividual(survivors[0]);
	
	genocide(population, survivors, problem);
}

void solveGenetic(const Problem & problem) {
	GeneticPopulation population = generateRandomPopulation(problem);
	for (int i = 0; i < EVOLVE_ITERATIONS; i++) {
		std::cout << "starting evolution #" << i << std::endl;
		evolve(population, problem);
		std::cout << "finished evolution #" << i << std::endl;
	}
}
