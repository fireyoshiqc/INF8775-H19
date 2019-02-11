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

        vector<uint64_t> numbers = LoadVector(path);
        vector<uint64_t> output;

        
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
                QuickSort(numbers);
                output = numbers;
            }
            else if (sortType == "quickSeuil")
            {
                QuickThreshedSort(numbers);
                output = numbers;
            }
            else if (sortType == "quickRandomSeuil")
            {
                QuickRandomThreshedSort(numbers);
                output = numbers;
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

vector<uint64_t> LoadVector(string path)
{
    vector<uint64_t> numbers;
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
            numbers.push_back(stoull(line));
        }
        return numbers;
    }
    catch (exception e)
    {
        cerr << e.what() << endl;
        return numbers;
    }
}

vector<uint64_t> CountingSort(vector<uint64_t>& numbers)
{
    vector<uint64_t> output;
    if (numbers.size() > 0)
    {
        try
        {
            uint64_t maxElm = *max_element(numbers.begin(), numbers.end());
            vector<int> counts(maxElm + 1); // Zero inits
            output.reserve(numbers.size());

            for (auto number : numbers)
            {
                counts[number]++;
            }
            for (uint64_t i = 0; i < counts.size(); i++)
            {
                for (int j = 0; j < counts[i]; j++)
                {
                    output.push_back(i);
                }
            }
        }
        catch (const exception& e)
        {
            cerr << e.what() << endl;
        }
    }
    return output;
}

itr Partition(itr pivot, itr first, itr last)
{
    if (pivot != first)
        iter_swap(pivot, first);
    itr left = first;
    itr right = last;
    last--;

    while (true)
    {
        do {
            left++;
        } while (*left <= *pivot && left < last);
        do {
            right--;
        } while (*right > *pivot && right > first);
        if (left >= right)
            break;
       iter_swap(left, right);
    }
    std::iter_swap(pivot, right);
    return right;
}

void QuickSort(vector<uint64_t>& numbers)
{
    if (numbers.size() > 1)
    {
        QuickSort(numbers.begin(), numbers.end());
    }
}

void QuickSort(itr first, itr last)
{
    if (distance(first, last) > 1) // See definition of std::distance
    {
        itr sorted = Partition(first + (last - first) / 2, first, last);
        QuickSort(first, sorted);
        QuickSort(sorted + 1, last);
    }
}

void QuickThreshedSort(vector<uint64_t>& numbers)
{
}

void QuickThreshedSort(itr first, itr last)
{
}

void QuickRandomThreshedSort(vector<uint64_t>& numbers)
{
}

void QuickRandomThreshedSort(itr first, itr last)
{
}
