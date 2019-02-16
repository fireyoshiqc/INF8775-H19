#pragma once

#include <vector>
#include <string>
#include <cstdint>

using namespace std;
typedef vector<uint64_t>::iterator itr;

vector<uint64_t> LoadVector(string path);
vector<uint64_t> CountingSort(vector<uint64_t>& numbers);
itr Partition(itr pivot, itr first, itr last);
void QuickSort(vector<uint64_t>& numbers);
void QuickSort(itr first, itr last);
void QuickThreshedSort(vector<uint64_t>& numbers, ptrdiff_t threshold);
void QuickThreshedSort(itr first, itr last, ptrdiff_t threshold);
void QuickRandomThreshedSort(vector<uint64_t>& numbers, ptrdiff_t threshold);
void QuickRandomThreshedSort(itr first, itr last, ptrdiff_t threshold);
void BubbleSort(itr first, itr last);



