#pragma once

#include "ID3.hpp"

#include <vector>
#include <set>

void calculateConfusionMatrix(std::vector<std::vector<int>> &data, ID3 &id3, int confusionMatrix[2][2]);

double calculateAccuracy(int confusionMatrix[2][2]);

void calculateAttributesEntropy(unsigned int topN, std::set<int> &attributes, std::vector<std::vector<int>> &data);

// randomly shuffling data and dividing into training and test data
void divideData(int SEED, std::vector<std::vector<int>> &original_data, std::vector<std::vector<int>> &training_data, std::vector<std::vector<int>> &test_data);

double mean_stats(int runsNumber, std::set<int> &attributes, std::vector<std::vector<int>> &data);
