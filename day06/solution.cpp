
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

class Calculator
{
    bool isPlusEnabled = false;
    bool isMultiplyEnabled = false;
    bool isConcatenateEnabled = false;

    const std::vector<size_t> equationValues;
    const size_t testValue;

    using itr = std::vector<size_t>::const_iterator;

    bool TryEquation(itr i, size_t currentValue)
    {
        if (currentValue == testValue) {
            return true;
        }
        if (i == equationValues.end()) {
            return false;
        }

        auto val = *i;
        if (isPlusEnabled && TryEquation(i + 1, currentValue + val)) {
            return true;
        }
        if (isMultiplyEnabled && TryEquation(i + 1, currentValue * val)) {
            return true;
        }
        if (isConcatenateEnabled)
        {
            while (val > 0)
            {
                currentValue = (currentValue * 10) + (val % 10);
                val /= 10;
            }
            if (TryEquation(i + 1, currentValue)) {
                return true;
            }
        }
        return false;
    }

public:

    Calculator(const std::vector<size_t>& equationValues_, size_t testValue_)
    : equationValues(equationValues_)
    , testValue(testValue_)
    {}

    bool IsValidEquationWithOperators(const std::string& operatorsToCheck)
    {
        isPlusEnabled = operatorsToCheck.contains('+');
        isMultiplyEnabled = operatorsToCheck.contains('*');
        isConcatenateEnabled = operatorsToCheck.contains('|');

        return TryEquation(equationValues.begin(), 0);
    }

};

int main(void)
{
	std::ifstream inputFile("input.txt");

    size_t resultEx00 = 0;
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
        Calculator calculator(equationValues, testValue);
        if (calculator.IsValidEquationWithOperators("+*")) {
            resultEx00 += testValue;
        }
        if (calculator.IsValidEquationWithOperators("+*|")) {
            std::cout << i << ": " << testValue << std::endl;
            resultEx01 += testValue;
        }
        i++;
        equationValues.clear();
    }

	std::cout << "ex00: " << resultEx00 << std::endl;
    std::cout << "ex01: " << resultEx01 << std::endl;

    return 0;
}
