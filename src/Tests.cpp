#include "ID3.hpp"

#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <iostream>

void calculateConfusionMatrix(std::vector<std::vector<int>> &data, ID3 &id3, int confusionMatrix[2][2]) {
    for (auto test_case : data) {
        int expected = test_case.back();
        int actual = id3.predict(test_case);
        confusionMatrix[actual][expected]++;
    }
}

void calculateAttributesEntropy(unsigned int topN, std::set<int> &attributes, std::vector<std::vector<int>> &data) {
    std::vector<std::pair<double, int>> attrib_entropy;

    for (auto attrib : attributes) {
        double entr = ID3::entropy_for_division(attrib, data);
        attrib_entropy.push_back({entr, attrib});
    }

    sort(attrib_entropy.begin(), attrib_entropy.end());
    
    std::cout << topN << " smallest attribute entropy" << std::endl;
    for (unsigned int i = 0; i < attrib_entropy.size() && i < topN; i++) {
        std::cout << "Attrib" << attrib_entropy[i].second + 1 << ": " << attrib_entropy[i].first << std::endl;
    }
    std::cout << std::endl;
}

double calculateAccuracy(int confusionMatrix[2][2]) {
    int correctlyPredictedCases = confusionMatrix[0][0] + confusionMatrix[1][1];
    int allCases = confusionMatrix[0][0] + confusionMatrix[1][1] + confusionMatrix[1][0] + confusionMatrix[0][1];

    return 100.0 * (double)(correctlyPredictedCases)/(double)allCases;
}

// randomly shuffling data and dividing into training and test data
void divideData(int SEED, std::vector<std::vector<int>> &original_data, std::vector<std::vector<int>> &training_data, std::vector<std::vector<int>> &test_data) {
    std::shuffle ( original_data.begin(), original_data.end(), std::default_random_engine(SEED) );

    // divide dataset proportionally 20:80 (test_data : training:data)
    for (size_t i = 0; i < original_data.size(); i++) {
        auto curr_record = original_data[i];
        if (i % 5 == 1)
            test_data.push_back(curr_record);
        else
            training_data.push_back(curr_record);
    }
}

void mean_stats(int runsNumber, std::set<int> &attributes, std::vector<std::vector<int>> &data) {
    double accuracySum = 0;

    int confusionMatrixAvg[2][2]{{0}};

    int SEED = 989;

    for (int i = 0; i < runsNumber; i++) {
        std::cout << "run #" << i << std::endl;
        std::vector<std::vector<int>> training_data, test_data;
        divideData(SEED++, data, training_data, test_data);

        ID3 id3(attributes, training_data);

        int confusionMatrix[2][2]{{0}};
        calculateConfusionMatrix(test_data, id3, confusionMatrix);

        std::cout << "True Positive: " << confusionMatrix[1][1] << std::endl;
        std::cout << "True Negative: " << confusionMatrix[0][0] << std::endl;
        std::cout << "False Positive: " << confusionMatrix[1][0] << std::endl;
        std::cout << "False Negative: " << confusionMatrix[0][1] << std::endl;

        std::cout << "Accuracy: " << calculateAccuracy(confusionMatrix) << "%" << std::endl << std::endl;

        accuracySum += calculateAccuracy(confusionMatrix) ;
        confusionMatrixAvg[0][0] += confusionMatrix[0][0];
        confusionMatrixAvg[0][1] += confusionMatrix[0][1];
        confusionMatrixAvg[1][0] += confusionMatrix[1][0];
        confusionMatrixAvg[1][1] += confusionMatrix[1][1];
    }

    std::cout << "AVERAGE STATS" << std::endl;
    std::cout << "Mean Accuracy: " << accuracySum / runsNumber << " %" << std::endl << std::endl;
    std::cout << "Average confusion matrix" << std::endl;
    std::cout << "True Positive: " << (double) confusionMatrixAvg[1][1] / runsNumber << std::endl;
    std::cout << "True Negative: " << (double) confusionMatrixAvg[0][0] / runsNumber << std::endl;
    std::cout << "False Positive: " << (double) confusionMatrixAvg[1][0] / runsNumber << std::endl;
    std::cout << "False Negative: " << (double) confusionMatrixAvg[0][1] / runsNumber << std::endl;
}