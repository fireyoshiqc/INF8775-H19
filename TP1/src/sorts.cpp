#include "sorts.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdint>

using namespace std;

int main(int argc, char *argv[])
{
    try {
        vector<string> args(argv + 1, argv + argc);

        string sortType = "";
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
            sortType = args.at(index + 1); // Can throw exception
        }
        else
        {
            cerr << "L'option -a est requise avec un argument [counting | quick | quickSeuil | quickRandomSeuil]." << endl;
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

        vector<int64_t> numbers = LoadVector(path);
        vector<int64_t> output;

        
        if (numbers.size() > 0)
        {

            if (sortType == "counting")
            {
                if (timeIt)
                {
                    using namespace std::chrono;
                    auto start = steady_clock::now();
                    output = CountingSort(numbers);
                    auto end = steady_clock::now();
                    duration<double, std::milli> delay = end - start;
                    cout << delay.count() << endl;
                }
                else
                    output = CountingSort(numbers);
            }
            else if (sortType == "quick")
            {
                output = QuickSort(numbers);
            }
            else if (sortType == "quickSeuil")
            {
                output = QuickThreshedSort(numbers);
            }
            else if (sortType == "quickRandomSeuil")
            {
                output = QuickRandomThreshedSort(numbers);
            }
            else
            {
                cerr << "Type de tri non reconnu : " << sortType << endl;
                return 1;
            }

            if (printIt)
            {
                
                for (auto&& number : output)
                {
                    cout << number << " ";
                }
                cout << numbers.size() << endl;
                cout << endl << output.size() << endl;
            }

        }
        else
        {
            cerr << "Aucun nombre à trier." << endl;
        }
    }
    catch (exception e)
    {
        return 1;
    }
    return 0;
}

vector<int64_t> LoadVector(string path)
{
    vector<int64_t> numbers;
    try {
        ifstream fs(path);
        if (!fs.is_open())
        {
            cerr << "Impossible de lire le fichier en entrée : " << path << endl;
            return numbers;
        }
        string line;
        while (getline(fs, line))
        {
            numbers.push_back(stoi(line));
        }
        return numbers;
    }
    catch (exception e)
    {
        return numbers;
    }
}

vector<int64_t> CountingSort(vector<int64_t>& numbers)
{
    vector<int64_t> output;
    if (numbers.size() > 0)
    {
        int64_t maxElm = *max_element(numbers.begin(), numbers.end());
        vector<int> counts(maxElm + 1); // Zero inits
        output.reserve(numbers.size());
        
        for (auto number : numbers)
        {
            counts[number]++;
        }
        for (int64_t i = 0; i < counts.size(); i++)
        {
            for (int j = 0; j < counts[i]; j++)
            {
                output.push_back(i);
            }
        }
    }
    return output;
}

vector<int64_t> QuickSort(vector<int64_t>& numbers)
{
    vector<int64_t> output;

    return output;
}

vector<int64_t> QuickThreshedSort(vector<int64_t>& numbers)
{
    vector<int64_t> output;

    return output;
}

vector<int64_t> QuickRandomThreshedSort(vector<int64_t>& numbers)
{
    vector<int64_t> output;

    return output;
}
