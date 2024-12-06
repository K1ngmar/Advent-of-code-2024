
#include <fstream>
#include <set>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <cassert>
#include <algorithm>
#include <functional>

struct Position
{
    const size_t row;
    const size_t column;
};

class WordSearch
{
private:
    const std::vector<std::string> grid; /*!< -. */

    size_t NumberOfMatchesInRow(const std::string& word, Position pos) const
    {
        if (pos.column > grid.front().length() - word.length())
        {
            return 0;
        }
        const auto row = grid.at(pos.row);
    
        
        const auto isEqual = row.compare(pos.column, word.length(), word) == 0;

        const auto isEqualReverse = std::equal(
            row.rbegin() + row.length() - (pos.column + word.length()),
            row.rbegin() + row.length() - pos.column,
            word.begin(),
            word.end()
        );

        return isEqual + isEqualReverse;
    }

    size_t NumberOfMatchesInColumn(const std::string& word, Position pos) const
    {
        const auto& rowsInColumn = std::views::drop(grid, pos.row) | std::views::take(word.length());

        if (rowsInColumn.size() != word.length())
        {
            return 0;
        }

        bool isEqual = true;
        for (const auto& [currentRow, currentCharWord] : std::views::zip(rowsInColumn, word))
        {
            isEqual &= currentRow.at(pos.column) == currentCharWord;
        }

        bool isEqualInReverse = true;
        for (const auto& [currentRow, currentCharWord] : std::views::zip(rowsInColumn | std::views::reverse, word))
        {
            isEqualInReverse &= currentRow.at(pos.column) == currentCharWord;
        }

        return isEqual + isEqualInReverse;
    }

    std::tuple<bool, bool> NumberOfMatchesInDiagonalBottomRight(const std::string& word, Position pos) const
    {
        const auto& rows = std::views::drop(grid, pos.row) | std::views::take(word.length());
        
        if (
            rows.size() != word.length() ||
            pos.column > rows.front().length() - word.length()
        )
        {
            return {0,0};
        }

        bool isEqual = true;
        for (const auto& [indexInWord, row] : rows | std::views::enumerate)
        {
            isEqual &= row.at(pos.column + indexInWord) == word.at(indexInWord);
        }

        bool isEqualReverse = true;
        for (const auto& [indexInWord, row] : rows | std::views::reverse | std::views::enumerate)
        {
            isEqualReverse &= row.at(pos.column + word.length() - indexInWord - 1) == word.at(indexInWord);
        }
        return {isEqual, isEqualReverse};
    }

    std::tuple<bool, bool> NumberOfMatchesInDiagonalBottomLeft(const std::string& word, Position pos) const
    {
        const auto& rows = std::views::drop(grid, pos.row) | std::views::take(word.length());

        if (
            rows.size() != word.length() ||
            pos.column < word.length() - 1 || 
            pos.column >= grid.size()
        )
        {
            return {0,0};
        }
        
        bool isEqual = true;
        for (const auto& [indexInWord, row] : rows | std::views::enumerate)
        {
            isEqual &= row.at(pos.column - indexInWord) == word.at(indexInWord);
        }
    
        bool isEqualReverse = true;
        for (const auto& [indexInWord, row] : rows | std::views::reverse | std::views::enumerate)
        {
            isEqualReverse &= row.at(pos.column - word.length() + 1 + indexInWord) == word.at(indexInWord);
        }
        return {isEqual, isEqualReverse};
    }

    bool WordCrossOnPosition(const std::string& word, Position pos) const
    {
        // impossible to have a cross if the word is an even amount of characters
        if (word.length() <= 1 || word.length() % 2 == 0)
        {
            return false;
        } 
        const size_t offset = word.length() / 2;

        const auto& [rMatch, rMatchReverse] = NumberOfMatchesInDiagonalBottomRight(word, {pos.row - offset, pos.column - offset});
        const auto& [lMatch, lMatchReverse] = NumberOfMatchesInDiagonalBottomLeft(word, {pos.row - offset, pos.column + offset});

        return ((rMatch || rMatchReverse) && (lMatch || lMatchReverse));
    }

    void ExecuteFunctionForAllPositions(std::function<void(Position pos)> functionToExecute) const
    {
        for (size_t row = 0; row < grid.size(); row++)
        {
            for (size_t column = 0; column < grid.at(row).size(); column++)
            {
                const Position pos = {row, column};
                functionToExecute(pos);
            }
        }
    }

public:

    WordSearch(const std::vector<std::string>& grid_) : grid(grid_)
    {}

    size_t NumberOfOccurrences(const std::string& word) const
    {
        size_t numberOfMatches = 0;
        auto countOccurrencesOnPosition = [&](Position pos)
        {
            numberOfMatches += NumberOfMatchesInRow(word, pos);
            numberOfMatches += NumberOfMatchesInColumn(word, pos);
            const auto& [rMatch, rMatchInReverse] = NumberOfMatchesInDiagonalBottomRight(word, pos);
            numberOfMatches += rMatch + rMatchInReverse;
            const auto& [lMatch, lMatchInReverse] = NumberOfMatchesInDiagonalBottomLeft(word, pos);
            numberOfMatches += lMatch + lMatchInReverse;
        };
        ExecuteFunctionForAllPositions(countOccurrencesOnPosition);
        return numberOfMatches;
    }

    size_t NumberOfWordInXes(const std::string& word) const
    {
        size_t numberOfMatches = 0;

        auto countOccurrencesOfWordsInAnX = [&](Position pos)
        {
            numberOfMatches += WordCrossOnPosition(word, pos);
        };
        ExecuteFunctionForAllPositions(countOccurrencesOfWordsInAnX);
        return numberOfMatches;
    }

};

int main(void)
{
	std::ifstream inputFile("input.txt");

    std::vector<std::string> grid;
	for (std::string line; std::getline(inputFile, line);)
	{
        grid.emplace_back(line);
	}
    
    const WordSearch wordSearcher(grid);

    std::cout << "ex00: " << wordSearcher.NumberOfOccurrences("XMAS") << std::endl;
    std::cout << "ex01: " << wordSearcher.NumberOfWordInXes("MAS") << std::endl;

    return 0;
}