
#include <fstream>
#include <set>
#include <iostream>
#include <string>
#include <ranges>


int main(void)
{
    std::ifstream inputFile("input.txt");


    std::set<int32_t> leftHistoriansLocationIdList;
    std::multiset<int32_t> rightHistoriansLocationIdList;
    while (inputFile.good())
    {
        int32_t leftLocationId = 0;
        int32_t rightLocationId = 0;

        inputFile >> leftLocationId >> rightLocationId;

        leftHistoriansLocationIdList.emplace(leftLocationId);
        rightHistoriansLocationIdList.emplace(rightLocationId);
    }

    int32_t similarityScore = 0;
    for (const auto& id : leftHistoriansLocationIdList)
    {
        similarityScore += id * rightHistoriansLocationIdList.count(id);
    }

    std::cout << "ex01: " << similarityScore << std::endl;
    return 0;
}