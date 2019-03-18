#include "restos.h"

#include <algorithm>
#include <iterator>
#include <random>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdint>
#include <numeric>

#ifdef linux
#include <unistd.h>
#elif _WIN64
#include <windows.h>
#endif

using namespace std;

int main(int argc, char *argv[])
{
    try {
        vector<string> args(argv + 1, argv + argc);

        string typeAlgo = "";
        string path = "";
        bool printIt = false;
        bool timeIt = false;

        auto aPos = find(args.begin(), args.end(), "-a");
        auto ePos = find(args.begin(), args.end(), "-e");
        auto pPos = find(args.begin(), args.end(), "-p");
        auto tPos = find(args.begin(), args.end(), "-t");

        if (aPos != args.end())
        {
            auto index = aPos - args.begin();
            typeAlgo = args.at(index + 1); // Can throw exception
        }
        else
        {
            cerr << "L'option -a est requise avec un argument [glouton | progdyn | local]." << endl;
            return 1;
        }

        if (ePos != args.end())
        {
            auto index = ePos - args.begin();
            path = args.at(index + 1); // Can throw exception
        }
        else
        {
            cerr << "L'option -e est requise avec un argument [path]." << endl;
            return 1;
        }

        if (pPos != args.end())
        {
            printIt = true;
        }

        if (tPos != args.end())
        {
            timeIt = true;
        }

        uint32_t capaciteFournisseur = 0;
        vector<Resto> restos = ChargerExemplaire(path, capaciteFournisseur);
        vector<Resto> solution;


        if (restos.size() > 0)
        {

            if (typeAlgo == "glouton")
            {
                if (timeIt)
                {
                    using namespace std::chrono;
                    auto start = steady_clock::now();
                    solution = Glouton(restos, capaciteFournisseur);
                    auto end = steady_clock::now();
                    duration<double, std::milli> delay = end - start;
                    cout << delay.count() << endl;
                }
                else
                    solution = Glouton(restos, capaciteFournisseur);
            }
            else if (typeAlgo == "progdyn")
            {
                if (timeIt)
                {
                    using namespace std::chrono;
                    auto start = steady_clock::now();
                    solution = ProgDyn(restos, capaciteFournisseur);
                    auto end = steady_clock::now();
                    duration<double, std::milli> delay = end - start;
                    cout << delay.count() << endl;
                }
                else
                    solution = ProgDyn(restos, capaciteFournisseur);
            }
            /*
            else if (typeAlgo == "local")
            {
                if (timeIt)
                {
                    using namespace std::chrono;
                    auto start = steady_clock::now();
                    QuickThreshedSort(numbers, 5);
                    auto end = steady_clock::now();
                    duration<double, std::milli> delay = end - start;
                    cout << delay.count() << endl;
                }
                else
                    QuickThreshedSort(numbers, 5);

                output = numbers;
            }
            */
            else
            {
                cerr << "Type d'algo non reconnu : " << typeAlgo << endl;
                return 1;
            }

            if (printIt)
            {
                for (auto&& resto : solution)
                {
                    cout << resto.id << " ";
                }
            }

        }
        else
        {
            cerr << "Aucun nombre à trier." << endl;
        }
    }
    catch (const exception& e)
    {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}

vector<Resto> ChargerExemplaire(string path, uint32_t & capaciteFournisseur)
{
    vector<Resto> villes;
    try
    {
        ifstream fs(path);
        if (!fs.is_open())
        {
            cerr << "Impossible de lire le fichier en entrée : " << path << endl;
            return villes;
        }

        // Nombre de villes
        string line;
        getline(fs, line);
        uint32_t nbVilles = stoul(line);

        villes.reserve(nbVilles);

        // Lecture des villes
        uint32_t index = 0;
        while (index < nbVilles && getline(fs, line))
        {
            Resto ville;
            stringstream ss(line);
            ss >> ville.id;
            ss >> ville.revenu;
            ss >> ville.quantite;
            ville.choisi = false;
            villes.push_back(ville);
            index++;
        }

        // Lecture de la capacité du fournisseur
        getline(fs, line);
        capaciteFournisseur = stoul(line);
        return villes;
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
        return villes;
    }
}

vector<Resto> Glouton(vector<Resto>& restos, uint32_t capaciteFournisseur)
{
    srand(time(nullptr));
    vector<Resto> solution;
    uint32_t revenuMax = 0;
    //solution.erase(remove_if(villes.begin(), villes.end(), [capaciteFournisseur](Ville v) { return v.quantite > capaciteFournisseur; }));

    for (size_t i = 0; i < 10; i++)
    {
        uint32_t capaciteRestante = capaciteFournisseur;
        vector<Resto> solutionIteration;

        while (true)
        {

            float sommeRentabilite = 0.0f;
            for (auto&& resto : restos)
            {
                if (!resto.choisi && resto.quantite <= capaciteRestante)
                {
                    sommeRentabilite += resto.GetRentabilite();
                }
            }

            float randomPick = (float)rand() / (float)RAND_MAX;
            Resto restoChoisi;
            bool restoValide = false;

            for (auto&& resto : restos)
            {
                if (!resto.choisi && resto.quantite <= capaciteRestante)
                    randomPick -= (resto.GetRentabilite() / sommeRentabilite);
                if (randomPick < 0)
                {
                    restoChoisi = resto;
                    restoValide = true;
                    break;
                }
            }
            if (!restoValide)
                break;
            restos[restoChoisi.id - 1].choisi = true;
            capaciteRestante -= restos[restoChoisi.id - 1].quantite;
            solutionIteration.push_back(restos[restoChoisi.id - 1]);
        }

        uint32_t revenuIteration = accumulate(solutionIteration.begin(), solutionIteration.end(), 0, [](uint32_t sum, Resto resto) { return sum + resto.revenu; });
        if (revenuIteration > revenuMax)
        {
            solution = solutionIteration;
            revenuMax = revenuIteration;
        }
        for (auto&& resto : restos)
            resto.choisi = false;
    }
    return solution;
}

vector<Resto> ProgDyn(vector<Resto>& restos, uint32_t capaciteFournisseur)
{
    vector<vector<uint32_t>> dyn(restos.size(), vector<uint32_t>(capaciteFournisseur + 1)); // Zero-inited anyway
    for (int i = 0; i < restos.size(); i++)
    {
        for (int j = 0; j < capaciteFournisseur + 1; j++)
        {
            int ri = restos.at(i).revenu;
            int qi = restos.at(i).quantite;
            // TODO: Fix this
            uint32_t maxLeft = (((i - 1) >= 0 && (j - qi) >= 0) ? dyn[i - 1][j - qi] : 0) + ri;
            uint32_t maxTop = (((i - 1) >= 0 && (j - 1) >= 0) ? dyn[i - 1][j - 1] : 0);
            dyn[i][j] = max(maxLeft, maxTop);
        }
    }

    return vector<Resto>();
}
