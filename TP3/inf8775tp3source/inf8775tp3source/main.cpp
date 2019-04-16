#include "problem.h"
#include "genetic.h"
#include <ctime>

int main(int argc, char *argv[]) {
	srand(time(nullptr));


	std::vector<std::string> args(argv + 1, argv + argc);
	std::string path = "";
	auto ePos = std::find(args.begin(), args.end(), "-e");
	if (ePos != args.end())
	{
		auto index = ePos - args.begin();
		path = args.at(index + 1); // Can throw exception
	}
	else
	{
		std::cerr << "L'option -e est requise avec un argument [path]." << std::endl;
		return 1;
	}
	Problem problem = readProblem(path);
	if (problem.nModels <= 0)
	{
		std::cerr << "Fichier en entrée invalide." << std::endl;
		return 1;
	}

	solveGenetic(problem);

	#ifdef _WIN32
		system("pause");
	#endif
	return 0;
}