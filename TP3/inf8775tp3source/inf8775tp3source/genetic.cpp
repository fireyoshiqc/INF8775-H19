#include <algorithm>
#include "genetic.h"

//TODO: make objects for solver to store problem in a bigger scope (too many functions needs the problem data)
// also in object: store the values for greedy fill (the best model for each type of piece)
//TODO: compute cost for each model and store it in structures Model to avoid recalculating it everytime

//#define REMISE

float modelScoreForType(Model model, int pieceTypeIdx)
{
	int pieceQuantity = model.modelCosts[pieceTypeIdx];
	float totalModelCost = (float) std::accumulate(model.modelCosts.begin(), model.modelCosts.end(), 0);
	float score = (model.modelCosts[pieceTypeIdx] / totalModelCost);
	return score;
}

std::vector<int> findBestModelsPerPieceType(const Problem & problem)
{
	std::vector<int> bestModelsPerPieceType(problem.nPieces);
	for (int i = 0; i < problem.nPieces; i++) 
	{
		int bestModelIdx = 0;
		float bestScore = 0.0;
		for (int j = 0; j < problem.nModels; j++) 
		{
			float model_j_ScoreForPiece_i = modelScoreForType(problem.models[j], i);
			if (model_j_ScoreForPiece_i > bestScore)
			{
				bestScore = model_j_ScoreForPiece_i;
				bestModelIdx = j;
			}
		}
		bestModelsPerPieceType[i] = bestModelIdx;
	}
	return bestModelsPerPieceType;
}

int findHighestPieceTypeSurplus(const GeneticIndividual & individual) {
	return std::distance(individual.budget.begin(), std::max_element(individual.budget.begin(), individual.budget.end()));
}

void greedyFill(GeneticIndividual & individual, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	bool areAllUsed = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= 0; });
	while (!areAllUsed)
	{
		int highestPieceTypeSurplus = findHighestPieceTypeSurplus(individual);
		int addModelIdx = bestModelsPerPieceType[highestPieceTypeSurplus];
		addIndividualGenes(individual, problem, addModelIdx);
		areAllUsed = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= 0; });
	}

}

void randomFill(GeneticIndividual & individual, const Problem & problem) 
{
	bool areAllUsed = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= 0; });
	while (!areAllUsed)
	{
		int addModelIdx = rand() % individual.modelGenes.size();
		addIndividualGenes(individual, problem, addModelIdx);
		areAllUsed = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= 0; });
	}
}

GeneticIndividual generateRandomIndividual(const Problem & problem) 
{
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

GeneticIndividual generateGreedyIndividual(const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	GeneticIndividual individual;
	individual.budget = problem.availablePieceBudget;
	individual.modelGenes = std::vector<int>(problem.nModels);
	bool areAllUnderThreshold = false;
	while (!areAllUnderThreshold)
	{
		size_t selectIdx = rand() % problem.nModels;
		individual.modelGenes[selectIdx]++;
		for (size_t i = 0; i < individual.budget.size(); i++)
		{
			individual.budget[i] -= problem.models[selectIdx].modelCosts[i];
		}
		areAllUnderThreshold = std::all_of(individual.budget.begin(), individual.budget.end(), [](int amount) { return amount <= RANDOM_GENERATION_FILL_THRESHOLD; });
	}
	individual.totalCost = abs(std::accumulate(individual.budget.begin(), individual.budget.end(), 0));
	greedyFill(individual, problem, bestModelsPerPieceType);

	return individual;
}

GeneticPopulation generateRandomPopulation(const Problem & problem) 
{
	GeneticPopulation population;
	population.individuals.reserve(POPULATION_SIZE);
	for (size_t i = 0; i < POPULATION_SIZE; i++) 
	{
		population.individuals.emplace_back(generateRandomIndividual(problem));
	}
	return population;
}

GeneticPopulation generatGreedyPopulation(const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	GeneticPopulation population;
	population.individuals.reserve(POPULATION_SIZE);
	for (size_t i = 0; i < POPULATION_SIZE; i++)
	{
		population.individuals.emplace_back(generateGreedyIndividual(problem, bestModelsPerPieceType));
	}
	return population;
}

void switchIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t addModelIdx, size_t subModelIdx) 
{
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

void addIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t index) 
{
	individual.modelGenes[index] ++;

	// update budget
	for (size_t i = 0; i < individual.budget.size(); i++)
	{
		individual.budget[i] -= problem.models[index].modelCosts[i];
	}

	//update cost
	individual.totalCost += std::accumulate(begin(problem.models[index].modelCosts), end(problem.models[index].modelCosts), 0);
}

void subIndividualGenes(GeneticIndividual & individual, const Problem & problem, size_t index)
{
	/*if (individual.modelGenes[index] <= 0) {
		std::cout << "THE NUMBER OF MODELS CAN'T BE NEGATIVE!!!" << std::endl;
	}
	individual.modelGenes[index] --;*/

	// update budget
	for (size_t i = 0; i < individual.budget.size(); i++)
	{
		individual.budget[i] += problem.models[index].modelCosts[i];
	}

	//update cost
	individual.totalCost -= std::accumulate(begin(problem.models[index].modelCosts), end(problem.models[index].modelCosts), 0);
}

void mutateIndividual(GeneticIndividual & individual, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	size_t addModelIdx, subModelIdx;
	GeneticIndividual newIndividual = individual;
	// switch MINIMAL_MUTATION_ITERATIONS models 
	for (int i = 0; i < MINIMAL_MUTATION_ITERATIONS; i++) 
	{

		//find valid random genes to modify
		addModelIdx = rand() % newIndividual.modelGenes.size();
		do 
		{
			subModelIdx = rand() % newIndividual.modelGenes.size();
		} while (newIndividual.modelGenes[subModelIdx] == 0);

		switchIndividualGenes(newIndividual, problem, addModelIdx, subModelIdx);

	}

	if (newIndividual.totalCost > individual.totalCost) 
	{
		return; // If the new indivdual is already worst when it is not even valid yet, stop. 
	}
	
	// I commented this out to keep in mind that a possible acceleration is to stop once the individual is already worst
	/*
	bool areAllUsed = std::all_of(newIndividual.budget.begin(), newIndividual.budget.end(), [](int amount) { return amount <= 0; });
	bool isCostGreater = false;
	while (!areAllUsed)
	{
		addModelIdx = rand() % newIndividual.modelGenes.size();
		addIndividualGenes(newIndividual, problem, addModelIdx);
		areAllUsed = std::all_of(newIndividual.budget.begin(), newIndividual.budget.end(), [](int amount) { return amount <= 0; });
		if (newIndividual.totalCost > individual.totalCost) 
		{
			return; // If the new indivdual is already worst when it is not even valid yet, stop. 
		}
	}*/

	bool flip = rand() % 2;
	if (flip)
	{
		greedyFill(newIndividual, problem, bestModelsPerPieceType);
	}
	else
	{
		randomFill(newIndividual, problem);
	}

	//greedyFill(newIndividual, problem, bestModelsPerPieceType);
	//randomFill(newIndividual, problem);

	if (newIndividual.totalCost < individual.totalCost)
	{
		individual = newIndividual;
	}

}

void alterIndividual(GeneticIndividual & individual, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	size_t addModelIdx, subModelIdx;
	GeneticIndividual newIndividual = individual;
	// switch MINIMAL_MUTATION_ITERATIONS models 
	for (int i = 0; i < MINIMAL_ATLER_ITERATIONS; i++)
	{
		//find valid random genes to modify
		addModelIdx = rand() % newIndividual.modelGenes.size();
		do 
		{
			subModelIdx = rand() % newIndividual.modelGenes.size();
		} while (newIndividual.modelGenes[subModelIdx] == 0);

		switchIndividualGenes(newIndividual, problem, addModelIdx, subModelIdx);
	}

	// add models until individual is valid
	greedyFill(newIndividual, problem, bestModelsPerPieceType);
	//randomFill(newIndividual, problem);
	if (newIndividual.totalCost < individual.totalCost)
	{
		individual = newIndividual;
	}
}

void regressIndividual(int numberOfIterations, GeneticIndividual & individual, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	size_t modelIdx;
	GeneticIndividual newIndividual = individual;
	// substract numberOfIterations models 
	for (int i = 0; i < numberOfIterations; i++)
	{
		//find valid random genes to modify
		do
		{
			modelIdx = rand() % newIndividual.modelGenes.size();
		} while (newIndividual.modelGenes[modelIdx] == 0);

		subIndividualGenes(newIndividual, problem, modelIdx);
	}
	
	bool flip = rand() % 2;
	if (flip)
	{
		greedyFill(newIndividual, problem, bestModelsPerPieceType);
	}
	else
	{
		randomFill(newIndividual, problem);
	}

	//greedyFill(newIndividual, problem, bestModelsPerPieceType);
	//randomFill(newIndividual, problem);

	if (newIndividual.totalCost < individual.totalCost)
	{
		individual = newIndividual;
	}
}

GeneticIndividual crossoverMin(const GeneticIndividual & individual_a, const GeneticIndividual & individual_b, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	//TODO: make a baby from the parents using something like taking the average, taking the mins of each and filling the rest, possibly using flip coins
	GeneticIndividual baby;
	baby.budget = problem.availablePieceBudget;
	baby.modelGenes = std::vector<int>(problem.nModels);

	int nGenesPreserved = problem.nModels * PROPORTION_PRESERVED_GENES_CROSSOVER / 100;
	for (int i = 0; i < nGenesPreserved; i++)
	{
		if (individual_a.modelGenes[i] < individual_b.modelGenes[i]) 
		{
			baby.modelGenes[i] = individual_a.modelGenes[i];
		}
		else
		{
			baby.modelGenes[i] = individual_b.modelGenes[i];
		}
		for (int k = 0; k < baby.budget.size(); k++)
		{
			baby.budget[k] -= problem.models[i].modelCosts[k] * baby.modelGenes[i];
		}
	}
	baby.totalCost = abs(std::accumulate(baby.budget.begin(), baby.budget.end(), 0));
	
	bool flip = rand() % 2;
	if (flip)
	{
		greedyFill(baby, problem, bestModelsPerPieceType);
	}
	else
	{
		randomFill(baby, problem);
	}
	
	//randomFill(baby, problem);
	//greedyFill(baby, problem, bestModelsPerPieceType);

	return baby;
}

bool compareByCost(const GeneticIndividual &a, const GeneticIndividual &b)
{
	return a.totalCost < b.totalCost;
}

void sortPopulation(GeneticPopulation & population) 
{
	std::sort(population.individuals.begin(), population.individuals.end(), compareByCost);
}

std::vector<GeneticIndividual> selectSurvivors(GeneticPopulation & population) 
{
	sortPopulation(population);
	std::vector<GeneticIndividual> survivors(NUMBER_OF_SURVIVORS);
	survivors = slice(population.individuals, 0, NUMBER_OF_SURVIVORS - 1);
	return survivors;
}

void nuclearCataclysm(GeneticPopulation & population, std::vector<GeneticIndividual> survivors, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	for (int i = NUMBER_OF_SURVIVORS; i < POPULATION_SIZE; i++) 
	{
		int parentIdx = rand() % NUMBER_OF_SURVIVORS;
		GeneticIndividual newIndividual = survivors[parentIdx];
		alterIndividual(newIndividual, problem, bestModelsPerPieceType);
		population.individuals[i] = newIndividual;
	}
}

void adaptation(int numberOfIterations, GeneticPopulation & population, std::vector<GeneticIndividual> survivors, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	for (int i = NUMBER_OF_SURVIVORS; i < POPULATION_SIZE; i++)
	{
		int parentIdx = rand() % NUMBER_OF_SURVIVORS;
		GeneticIndividual newIndividual = survivors[parentIdx];
		regressIndividual(numberOfIterations, newIndividual, problem, bestModelsPerPieceType);
		population.individuals[i] = newIndividual;
	}
}

void violentBreeding(GeneticPopulation & population, std::vector<GeneticIndividual> survivors, const Problem & problem, std::vector<int> & bestModelsPerPieceType) {
	for (int i = NUMBER_OF_SURVIVORS; i < POPULATION_SIZE; i++)
	{
		GeneticIndividual alphaMale = survivors[0];
		int willingPartner;
		do
		{
			willingPartner = rand() % NUMBER_OF_SURVIVORS;
		} while (willingPartner == 0);
		GeneticIndividual newIndividual = crossoverMin(alphaMale, survivors[willingPartner], problem, bestModelsPerPieceType);
		population.individuals[i] = newIndividual;
	}
}

void printIndividual(GeneticIndividual ind) 
{
#ifndef REMISE
	std::cout << "Best individual cost: " << ind.totalCost << std::endl;
	std::cout << "Best individual solution: ";
#endif
	for (int i = 0; i < ind.modelGenes.size(); i++) 
	{
		std::cout << ind.modelGenes[i] << " ";
	}
	std::cout << std::endl;
}

void evolve(int i, GeneticPopulation & population, const Problem & problem, std::vector<int> & bestModelsPerPieceType)
{
	for (int i = 0; i < POPULATION_SIZE; i++) 
	{
		mutateIndividual(population.individuals[i], problem, bestModelsPerPieceType);
	}
	std::vector<GeneticIndividual> survivors = selectSurvivors(population);

	printIndividual(survivors[0]);
	
	bool flip = 1;// rand() % 3;
	if (flip == 0) 
	{
		//violentBreeding(population, survivors, problem, bestModelsPerPieceType);
		adaptation((MINIMAL_REGRESS_ITERATIONS + i/10), population, survivors, problem, bestModelsPerPieceType);
	}
	else if (flip == 1)
	{
		adaptation((MINIMAL_REGRESS_ITERATIONS), population, survivors, problem, bestModelsPerPieceType);
	}
	else {
		adaptation((MINIMAL_REGRESS_ITERATIONS + i / 5), population, survivors, problem, bestModelsPerPieceType);
	}

	//violentBreeding(population, survivors, problem, bestModelsPerPieceType);
	//nuclearCataclysm(population, survivors, problem, bestModelsPerPieceType);
	//adaptation(population, survivors, problem, bestModelsPerPieceType);
}

bool isInvividualValid(GeneticIndividual & ind) {
	return std::all_of(ind.budget.begin(), ind.budget.end(), [](int amount) { return amount <= 0; });
}

void heuristicSearch(GeneticIndividual ind, Problem problem) {
	
	GeneticIndividual currentInd = ind;
	GeneticIndividual oneToOneIndividual, oneToTwoIndividual, twoToOneIndividual, twoToTwoIndividual;
	bool shouldContinue = true;
	while (shouldContinue) {
		shouldContinue = false;
		for (int i = 0; i < problem.nModels; i++) {
			for (int j = 0; j < problem.nModels; j++) {
				for (int k = 0; k < problem.nModels; k++) {
					if (k != i) {
						for (int l = 0; l < problem.nModels; l++) {
							oneToOneIndividual = currentInd;
							addIndividualGenes(oneToOneIndividual, problem, i);
							subIndividualGenes(oneToOneIndividual, problem, j);

							oneToTwoIndividual = currentInd;
							addIndividualGenes(oneToTwoIndividual, problem, i);
							subIndividualGenes(oneToTwoIndividual, problem, j);
							subIndividualGenes(oneToTwoIndividual, problem, l);

							twoToOneIndividual = currentInd;
							addIndividualGenes(twoToOneIndividual, problem, i);
							addIndividualGenes(twoToOneIndividual, problem, k);
							subIndividualGenes(twoToOneIndividual, problem, j);

							twoToTwoIndividual = currentInd;
							addIndividualGenes(twoToTwoIndividual, problem, i);
							addIndividualGenes(twoToTwoIndividual, problem, k);
							subIndividualGenes(twoToTwoIndividual, problem, j);
							subIndividualGenes(twoToTwoIndividual, problem, l);

							std::vector<int> potentialCosts = { oneToOneIndividual.totalCost, oneToTwoIndividual.totalCost, twoToOneIndividual.totalCost, twoToTwoIndividual.totalCost };
							std::sort(potentialCosts.begin(), potentialCosts.end());

							for (int c = potentialCosts.size() - 1; c >= 0; c--) {
								if (potentialCosts[c] >= currentInd.totalCost) 
								{
									break;
								}
								if (potentialCosts[c] == oneToOneIndividual.totalCost && isInvividualValid(oneToOneIndividual))
								{
									currentInd = oneToOneIndividual;
									shouldContinue = true;
									printIndividual(currentInd);
									break;
								}
								else if (potentialCosts[c] == oneToTwoIndividual.totalCost && isInvividualValid(oneToTwoIndividual))
								{
									currentInd = oneToTwoIndividual;
									shouldContinue = true;
									printIndividual(currentInd);
									break;
								}
								else if (potentialCosts[c] == twoToOneIndividual.totalCost && isInvividualValid(twoToOneIndividual))
								{
									currentInd = twoToOneIndividual;
									shouldContinue = true;
									printIndividual(currentInd);
									break;
								}
								else if (potentialCosts[c] == twoToTwoIndividual.totalCost && isInvividualValid(twoToTwoIndividual))
								{
									currentInd = twoToTwoIndividual;
									shouldContinue = true;
									printIndividual(currentInd);
									break;
								}

							}
						}
					}
				}
			}
		}
	}
	
}

void solveGenetic(const Problem & problem, int nIterations)
{
	std::vector<int> bestModelsPerPieceType = findBestModelsPerPieceType(problem);
	GeneticPopulation population = generateRandomPopulation(problem);
	//GeneticPopulation population = generatGreedyPopulation(problem, bestModelsPerPieceType);

	for (int i = 0; i < nIterations; i++) 
	{
#ifndef REMISE
		std::cout << "starting evolution #" << i << std::endl;
#endif
		evolve(i, population, problem, bestModelsPerPieceType);
#ifndef REMISE
		std::cout << "finished evolution #" << i << std::endl;
#endif
	}
	std::cout << "Genetic algorithm completed. Starting local search." << std::endl;
	GeneticIndividual foundIndividual = population.individuals[0];
	heuristicSearch(foundIndividual, problem);

}
