
#include <fstream>
#include <set>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <cctype>

static bool CouldBeValidEquation(const std::vector<size_t>& equationValues, size_t testValue)
{

    constexpr auto possibleOperators = "+*";
    std::string stringToDoPermutationsOn;
    for (const auto& s : std::views::repeat(possibleOperators, equationValues.size() - 1))
    {
        // Actually checking way more permutations than necessary but this required less thinking :)
        stringToDoPermutationsOn.append(s);
    }
    std::sort(stringToDoPermutationsOn.begin(), stringToDoPermutationsOn.end());


    bool previousPermutation = true;
    while (true)
    {
        size_t currentVal = equationValues.front();
        for (const auto& [operatorIndex, value] : std::views::drop(equationValues, 1) | std::views::enumerate)
        {
            if (stringToDoPermutationsOn.at(operatorIndex) == '+') {
                currentVal += value;
            }
            else {
                currentVal *= value;
            }
        }

        if (currentVal == testValue) {
            return true;
        }

        if (!previousPermutation) {
            return false;
        }

        // Need to figure out how to do this properly because this is ugly.
        previousPermutation = std::next_permutation(stringToDoPermutationsOn.begin(), stringToDoPermutationsOn.end());
    }
}

int main(void)
{
	std::ifstream inputFile("input.txt");

    size_t resultEx01 = 0;
    size_t i = 0;
    for (std::string line; std::getline(inputFile, line);)
	{
        auto splitLine = std::views::split(line, ':');
        const auto testValue = std::stoull(std::string_view(splitLine.front()).begin());

        std::vector<size_t> equationValues;
        for (auto value : *std::next(splitLine.begin()) | std::views::split(' '))
        {
            if (std::isspace(value.front()))
            {
                continue;
            }
            equationValues.emplace_back(std::stoull(std::string_view(value).begin()));
        }
        if (CouldBeValidEquation(equationValues, testValue))
        {
            std::cout << i << ": " << testValue <<  std::endl;
            resultEx01 += testValue;
        }
        i++;
        equationValues.clear();
    }

	std::cout << "ex01: " << resultEx01 << std::endl;

    return 0;
}
