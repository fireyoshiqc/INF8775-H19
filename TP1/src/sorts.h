#pragma once

#include <vector>
#include <string>
#include <cstdint>

using namespace std;

vector<int64_t> LoadVector(string path);
vector<int64_t> CountingSort(vector<int64_t>& numbers);
vector<int64_t> QuickSort(vector<int64_t>& numbers);
vector<int64_t> QuickThreshedSort(vector<int64_t>& numbers);
vector<int64_t> QuickRandomThreshedSort(vector<int64_t>& numbers);



