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

constexpr auto HEURISTIQUE_TEMPS_MAX = 180;

using namespace std;

int main(int argc, char *argv[])
{
    try {
        vector<string> args(argv + 1, argv + argc);

        string typeAlgo = "";
        string path = "";
        bool printIt = false;
        bool timeIt = false;
        bool revenuIt = false;

        auto aPos = find(args.begin(), args.end(), "-a");
        auto ePos = find(args.begin(), args.end(), "-e");
        auto pPos = find(args.begin(), args.end(), "-p");
        auto tPos = find(args.begin(), args.end(), "-t");
        auto rPos = find(args.begin(), args.end(), "-r");

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

        if (rPos != args.end())
        {
            revenuIt = true;
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
                    cout << delay.count() << (revenuIt ? "," : "");
                    if (!revenuIt)
                        cout << endl;
                }
                else
                    solution = Glouton(restos, capaciteFournisseur);
            }
            else if (typeAlgo == "progdyn")
            {
                bool okayToGo = true;
                uint64_t ramNeeded = restos.size() * capaciteFournisseur * sizeof(uint32_t);
                uint64_t gb8 = ((uint64_t)1 << 33) * 1.5;
                if (ramNeeded > gb8)
                {
                    cerr << "Capacité mémoire excédée (nécessiterait ~" << (ramNeeded >> 30) << "GB de RAM)." << endl;
                    okayToGo = false;
                }

                if (timeIt)
                {
                    using namespace std::chrono;
                    auto start = steady_clock::now();
                    solution = okayToGo ? ProgDyn(restos, capaciteFournisseur) : vector<Resto>();
                    auto end = steady_clock::now();
                    duration<double, std::milli> delay = end - start;
                    cout << (okayToGo? delay.count() : 0) << (revenuIt ? "," : "");
                    if (!revenuIt)
                        cout << endl;
                }
                else
                    solution = okayToGo ? ProgDyn(restos, capaciteFournisseur) : vector<Resto>();
            }
            else if (typeAlgo == "local")
            {
                if (timeIt)
                {
                    using namespace std::chrono;
                    auto start = steady_clock::now();
                    solution = Heuristique(restos, capaciteFournisseur);
                    auto end = steady_clock::now();
                    duration<double, std::milli> delay = end - start;
                    cout << delay.count() << (revenuIt ? "," : "");
                    if (!revenuIt)
                        cout << endl;
                }
                else
                    solution = Heuristique(restos, capaciteFournisseur);
            }
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

            if (revenuIt)
            {
                uint32_t revenu = accumulate(solution.begin(), solution.end(), 0, [](uint32_t sum, Resto resto) { return sum + resto.revenu; });
                cout << revenu << endl;
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
    // Construire le tableau dynamique (attention à la RAM)
    vector<vector<uint32_t>> dyn(restos.size(), vector<uint32_t>(capaciteFournisseur + 1)); // Zero-inited anyway
    for (int i = 0; i < restos.size(); i++)
    {
        for (int j = 1; j < capaciteFournisseur + 1; j++)
        {
            int qi = restos.at(i).quantite;
            int ri = qi <= j ? restos.at(i).revenu : 0;

            uint32_t maxLeft = (((i - 1) >= 0 && (j - qi) >= 0) ? dyn[i - 1][j - qi] : 0) + ri;
            uint32_t maxTop = (((i - 1) >= 0) ? dyn[i - 1][j] : 0);

            dyn[i][j] = max(maxLeft, maxTop);
        }
    }

    // Extraire la solution
    vector<Resto> solution;
    int quantiteRestante = dyn[0].size() - 1;

    for (int i = dyn.size() - 1; i >= 0; i--)
    {
        if ((i > 0 && dyn[i][quantiteRestante] != dyn[i - 1][quantiteRestante]) || (i == 0 && dyn[i][quantiteRestante] != 0))
        {
            solution.push_back(restos.at(i));
            quantiteRestante = quantiteRestante - restos.at(i).quantite;
            if (quantiteRestante <= 0)
                break;
        }
    }

    return solution;
}

vector<Resto> Heuristique(vector<Resto>& restos, uint32_t capaciteFournisseur)
{
    vector<Resto> solution = Glouton(restos, capaciteFournisseur);
    uint32_t revenuSolution = accumulate(solution.begin(), solution.end(), 0, [](uint32_t sum, Resto resto) { return sum + resto.revenu; });
    int capaciteSolution = (int)capaciteFournisseur - (int)accumulate(solution.begin(), solution.end(), 0, [](uint32_t sum, Resto resto) { return sum + resto.quantite; });
    uint32_t meilleurRevenu = revenuSolution;

    // VERSION ALL SWAPS
    sort(solution.begin(), solution.end(), [](Resto a, Resto b) { return a.id < b.id; });
    restos.erase(
        remove_if(restos.begin(), restos.end(),
            [&](Resto r) { return binary_search(solution.begin(), solution.end(), Resto(r.id)); }
        ), restos.end()
    );

    using namespace std::chrono;
    auto start = steady_clock::now();

    bool shouldStop = false;
    while (!shouldStop)
    {
        HeuristicSwap swapIndexes;
        for (int i = 0; i < solution.size(); i++)
        {
            Resto& restoSub1 = solution.at(i);

            for (int j = i + 1; j < solution.size(); j++)
            {
                Resto& restoSub2 = solution.at(j);

                for (int k = 0; k < restos.size(); k++)
                {
                    Resto& restoAdd1 = restos.at(k);

                    uint32_t revenuSwap21 = revenuSolution - restoSub1.revenu - restoSub2.revenu + restoAdd1.revenu;
                    uint32_t capaciteSwap21 = capaciteSolution + restoSub1.quantite + restoSub2.quantite;
                    if (restoAdd1.quantite <= capaciteSwap21)
                    {
                        for (int l = k + 1; l < restos.size(); l++)
                        {
                            Resto& restoAdd2 = restos.at(l);

                            uint32_t revenuSwap22 = revenuSolution - restoSub1.revenu - restoSub2.revenu + restoAdd1.revenu + restoAdd2.revenu;
                            uint32_t capaciteSwap22 = capaciteSolution + restoSub1.quantite + restoSub2.quantite;
                            if (restoAdd1.quantite + restoAdd2.quantite <= capaciteSwap22 && revenuSwap22 > meilleurRevenu)
                            {
                                meilleurRevenu = revenuSwap22;
                                swapIndexes = { i, j, k, l };
                            }
                        }
                        if (revenuSwap21 > meilleurRevenu)
                        {
                            meilleurRevenu = revenuSwap21;
                            swapIndexes = { i, j, k, -1 };
                        }
                    }
                }
            }
            for (int k = 0; k < restos.size(); k++)
            {
                Resto& restoAdd1 = restos.at(k);

                uint32_t revenuSwap11 = revenuSolution - restoSub1.revenu + restoAdd1.revenu;
                uint32_t capaciteSwap11 = capaciteSolution + restoSub1.quantite;
                if (restoAdd1.quantite <= capaciteSwap11)
                {
                    for (int l = k + 1; l < restos.size(); l++)
                    {
                        Resto& restoAdd2 = restos.at(l);

                        uint32_t revenuSwap12 = revenuSolution - restoSub1.revenu + restoAdd1.revenu + restoAdd2.revenu;
                        uint32_t capaciteSwap12 = capaciteSolution + restoSub1.quantite;
                        if (restoAdd1.quantite + restoAdd2.quantite <= capaciteSwap12 && revenuSwap12 > meilleurRevenu)
                        {
                            meilleurRevenu = revenuSwap12;
                            swapIndexes = { i, -1, k, l };
                        }
                    }
                    if (revenuSwap11 > meilleurRevenu)
                    {
                        meilleurRevenu = revenuSwap11;
                        swapIndexes = { i, -1, k, -1 };
                    }
                }
            }
        }

        if (swapIndexes.add2 >= 0 && swapIndexes.sub2 >= 0)
        {
            Resto& ajout = restos.at(swapIndexes.add2);
            Resto& enleve = solution.at(swapIndexes.sub2);
            revenuSolution = revenuSolution - enleve.revenu + ajout.revenu;
            capaciteSolution = capaciteSolution - (int)ajout.quantite + (int)enleve.quantite;
            swap(ajout, enleve);
        }
        else if (swapIndexes.add2 >= 0)
        {
            Resto& ajout = restos.at(swapIndexes.add2);
            revenuSolution = revenuSolution + ajout.revenu;
            capaciteSolution = capaciteSolution - (int)ajout.quantite;
            solution.emplace_back(move(ajout));
            restos.erase(restos.begin() + swapIndexes.add2);
        }
        else if (swapIndexes.sub2 >= 0)
        {
            Resto& enleve = solution.at(swapIndexes.sub2);
            revenuSolution = revenuSolution - enleve.revenu;
            capaciteSolution = capaciteSolution + (int)enleve.quantite;
            restos.emplace_back(move(enleve));
            solution.erase(solution.begin() + swapIndexes.sub2);
        }

        if (swapIndexes.add1 >= 0 && swapIndexes.sub1 >= 0)
        {
            Resto& ajout = restos.at(swapIndexes.add1);
            Resto& enleve = solution.at(swapIndexes.sub1);
            revenuSolution = revenuSolution - enleve.revenu + ajout.revenu;
            capaciteSolution = capaciteSolution - (int)ajout.quantite + (int)enleve.quantite;
            swap(ajout, enleve);
        }
        else
        {
            shouldStop = true;
        }
        auto currTime = steady_clock::now();
        duration<double, std::ratio<1>> elapsedTime = currTime - start;
        if (elapsedTime.count() >= HEURISTIQUE_TEMPS_MAX)
            break;
    }
    
    // VERSION 1-1 efficient
    /*
    ///////////////////

    // Marquage des restos déjà choisis
    for (auto&& restoInit : solution)
    {
        // Hack possible car le ID du resto correspond à son index + 1 dans le vecteur restos
        restos.at(restoInit.id - 1).choisi = true;
    }

    using namespace std::chrono;
    auto start = steady_clock::now();

    bool modified = true;
    while (modified)
    {
        modified = false;
        int idxRestoEnleve = -1;
        int idxRestoAjoute = -1;
        for (int i = 0; i < solution.size(); i++)
        {
            for (int j = 0; j < restos.size(); j++)
            {
                if (!restos.at(j).choisi)
                {
                    uint32_t nouveauRevenu = revenuSolution - solution.at(i).revenu + restos.at(j).revenu;
                    uint32_t capacitePossible = capaciteSolution + solution.at(i).quantite;
                    if (restos.at(j).quantite <= capacitePossible && nouveauRevenu > meilleurRevenu)
                    {
                        meilleurRevenu = nouveauRevenu;
                        idxRestoEnleve = i;
                        idxRestoAjoute = j;
                    }
                }
            }
        }

        if (idxRestoEnleve >= 0 && idxRestoAjoute >= 0)
        {
            Resto& restoEnleve = restos.at(solution.at(idxRestoEnleve).id - 1);
            Resto& restoAjoute = restos.at(idxRestoAjoute);

            restoEnleve.choisi = false;
            restoAjoute.choisi = true;

            revenuSolution = revenuSolution - restoEnleve.revenu + restoAjoute.revenu;
            capaciteSolution = capaciteSolution - restoAjoute.quantite + restoEnleve.quantite;

            solution.erase(solution.begin() + idxRestoEnleve);
            solution.push_back(restoAjoute);

            modified = true;
        }
        auto currTime = steady_clock::now();
        duration<double, std::ratio<1>> elapsedTime = currTime - start;
        if (elapsedTime.count() >= HEURISTIQUE_TEMPS_MAX)
            break;
    }
    */
    //cout << "Revenu heuristique: " << revenuSolution << endl;
    //cout << "Capacité heuristique: " << capaciteSolution << endl;
    return solution;
}
