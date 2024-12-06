
#include <fstream>
#include <set>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>

static bool IsUpdateInCorrectOrder(const std::multimap<size_t, size_t>& rules, size_t leftPageToCheck, size_t rightPageToCheck)
{
    const auto range = rules.equal_range(rightPageToCheck);
    for (auto i = range.first; i != range.second; ++i)
    {
        const auto& [leftPage, rightPage] = *i;
        if (rightPage == leftPageToCheck)
        {
            return false;
        }
    }
    return true;
}

int main(void)
{
	std::ifstream inputFile("input.txt");
    std::stringstream buffer;
	buffer << inputFile.rdbuf();
    const auto input = buffer.str();

    const auto endOfRulesPosition = input.find("\n\n");
    const std::string_view ruleData(input.begin(), input.begin() + endOfRulesPosition);
    const std::string_view updateData(input.begin() + endOfRulesPosition + 2, std::prev(input.end()));

    std::multimap<size_t, size_t> rules;
    for (const auto& rule : std::views::split(ruleData, '\n'))
    {
        auto splitRule = std::views::split(rule, '|');
        const auto& leftPage = std::string_view{splitRule.front()};
        const auto& rightPage = std::string_view{*(std::next(splitRule.begin()))};
        
        rules.emplace(std::stoll(leftPage.begin()), std::stoll(rightPage.begin()));
    }
    
    size_t ex00Result = 0;
    size_t ex01Result = 0;
    for (const auto& updateString : std::views::split(updateData, '\n'))
    {
        std::vector<size_t> update;
        for (const auto& value : std::views::split(updateString, ','))
        {
            update.emplace_back(std::stoll(std::string_view{value}.begin()));
        }

        bool isUpdateValidWihoutReordering = true;
        bool isUpdateValid = false;
        while (!isUpdateValid)
        {
            isUpdateValid = true;
            for (const auto& valuesToCheck : std::views::slide(update, 2))
            {
                auto& leftPageToCheck = valuesToCheck.front();
                auto& rightPageToCheck = *std::next(valuesToCheck.begin());

                if (!IsUpdateInCorrectOrder(rules, leftPageToCheck, rightPageToCheck))
                {
                    isUpdateValidWihoutReordering = false;
                    isUpdateValid = false;
                    std::swap(leftPageToCheck, rightPageToCheck);
                }
            }
        }

        size_t& exToUpdate = (isUpdateValidWihoutReordering ? ex00Result : ex01Result);
        exToUpdate += update.at(update.size() / 2);

        update.clear();
    }

    std::cout << ex00Result << std::endl;
    std::cout << ex01Result << std::endl;

}