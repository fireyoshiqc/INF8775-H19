#include <algorithm>
#include "genetic.h"

GeneticIndividual generateRandomIndividual(const Problem & problem) {
	GeneticIndividual individual;
	individual.budget = problem.availablePieceBudget;
	individual.modelGenes = std::vector<int> (problem.nModels);
	individual.fitness = 0.0;
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
	
	for (size_t i = 0; i < individual.modelGenes.size(); i++) {
		individual.fitness += (float)(1 / (float) std::accumulate(begin(problem.models[i].modelCosts), end(problem.models[i].modelCosts), 0));
	}

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

	for (size_t i = 0; i < individual.budget.size(); i++)
	{
		individual.budget[i] -= problem.models[addModelIdx].modelCosts[i];
	}

	for (size_t i = 0; i < individual.budget.size(); i++)
	{
		individual.budget[i] += problem.models[subModelIdx].modelCosts[i];
	}


	individual.fitness += (float)(1 / (float) std::accumulate(begin(problem.models[addModelIdx].modelCosts), end(problem.models[addModelIdx].modelCosts), 0));
	individual.fitness -= (float)(1 / (float) std::accumulate(begin(problem.models[subModelIdx].modelCosts), end(problem.models[subModelIdx].modelCosts), 0));

}

void addIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t index) {
	individual.modelGenes[index] ++;
	individual.fitness += (float)(1 / (float) std::accumulate(begin(problem.models[index].modelCosts), end(problem.models[index].modelCosts), 0));
}

void mutateIndividual(GeneticIndividual & individual, const Problem & problem) {
	size_t addModelIdx, subModelIdx;
	// switch MINIMAL_MUTATION_ITERATIONS models 
	for (size_t i = 0; i < MINIMAL_MUTATION_ITERATIONS; i++) 
	{

		//find valid random genes to modify
		addModelIdx = rand() % individual.modelGenes.size();
		do {
			subModelIdx = rand() % individual.modelGenes.size();
		} while (individual.modelGenes[subModelIdx] != 0);

		switchIndividualGenes(individual, problem, addModelIdx, subModelIdx);

	}
	
	// add models until individual is valid
	bool areAllUsed = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= 0; });
	while (!areAllUsed)
	{
		addModelIdx = rand() % individual.modelGenes.size();
		addIndividualGenes(individual, problem, addModelIdx);
		areAllUsed = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= 0; });
	}

}

void evolve(GeneticPopulation & population, const Problem & problem) {
	for (size_t i = 0; i < POPULATION_SIZE; i++) {
		mutateIndividual(population.individuals[i], problem);
	}
}

void solveGenetic(const Problem & problem) {
	GeneticPopulation population = generateRandomPopulation(problem);
	for (size_t i = 0; i < EVOLVE_ITERATIONS; i++) {
		std::cout << "starting evolution #" << i << std::endl;
		evolve(population, problem);
		std::cout << "finished evolution #" << i << std::endl;
	}
}
