#include "problem.h"
#include "solution.h"
#include "genetic.h"
#include <ctime>

/*
Solution solver(const Problem & problem) {
}
*/


int main() {
	srand(time(nullptr));
	Problem problem = readProblem("../../exemplaires/LEGO_50_50_1000");
	std::vector<GeneticIndividual> population;
	population.reserve(10);
	for (size_t i = 0; i < 10; i++)
		population.emplace_back(generateRandomIndividual(problem));
	std::cout << population.size() << std::endl;

	solveGenetic(problem);

	printProblem(problem);
	#ifdef _WIN32
		system("pause");
	#endif
	return 0;
}