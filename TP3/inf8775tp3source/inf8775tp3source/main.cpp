#include "problem.h"
#include "utils.h"

Solution solver(const Problem & problem) {
	
}


int main() {

	Problem problem = readProblem("../../exemplaires/LEGO_50_50_1000");
	printProblem(problem);
	#ifdef _WIN32
		system("pause");
	#endif
	return 0;
}