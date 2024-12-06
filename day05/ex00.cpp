
#include <fstream>
#include <set>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <cassert>
#include <regex>

enum class Direction
{
    Up,
    Right,
    Left,
    Down
};

struct Position
{
    int32_t x;
    int32_t y;
    Direction dir;
};

static char& CharAtPosition(std::vector<std::string>& map, Position pos)
{
    static char outOfMap = '.';
    if (
        pos.y < 0 ||
        pos.y >= map.size() ||
        pos.x < 0 ||
        pos.x >= map.front().length()
    )
    {
        return outOfMap;
    }
    return map.at(pos.y).at(pos.x);
}

static void PrintMap(const std::vector<std::string>& map)
{
    for (const std::string& row : map)
    {
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

int main(void)
{
    constexpr auto guardUpChar = '^';
    constexpr auto guardRightChar = '>';
    constexpr auto guardDownChar = 'v';
    constexpr auto guardLeftChar = '<';
	std::ifstream inputFile("input.txt");

	std::vector<std::string> map;
    std::optional<Position> guardStartingPosition;
    size_t y = 0;
	for (std::string line; std::getline(inputFile, line);)
	{
        map.emplace_back(line);
        if (line.contains(guardUpChar))
        {
            guardStartingPosition.emplace(line.find(guardUpChar), y, Direction::Up);
        }
        else if (line.contains(guardRightChar))
        {
            guardStartingPosition.emplace(line.find(guardRightChar), y, Direction::Right);
        }
        else if (line.contains(guardDownChar))
        {
            guardStartingPosition.emplace(line.find(guardDownChar), y, Direction::Down);
        }
        else if (line.contains(guardLeftChar))
        {
            guardStartingPosition.emplace(line.find(guardLeftChar), y, Direction::Left);
        }
        y++;
    }

    constexpr auto obstacle = '#';
    size_t resultEx00 = 0;
    map.at(guardStartingPosition->y).at(guardStartingPosition->x) = '.';
    for (
        Position currentPos = guardStartingPosition.value();
        currentPos.y >= 0 && currentPos.y < map.size() &&
        currentPos.x >= 0 && currentPos.x < map.front().length();
    )
    {
        auto& c = CharAtPosition(map, currentPos);
        if (c == '.')
        {
            resultEx00++;
            c = '_';
        }
        // PrintMap(map);
        switch (currentPos.dir)
        {
            case Direction::Up:
            {
                auto nextPos = currentPos;
                nextPos.y--;
                if (CharAtPosition(map, nextPos) != obstacle)
                {
                    currentPos = nextPos;
                    break;
                }
                currentPos.dir = Direction::Right;
                [[fallthrough]];
            }
            case Direction::Right:
            {
                auto nextPos = currentPos;
                nextPos.x++;
                if (CharAtPosition(map, nextPos) != obstacle)
                {
                    currentPos = nextPos;
                    break;
                }
                currentPos.dir = Direction::Down;
                [[fallthrough]];
            }
            case Direction::Down:
            {
                auto nextPos = currentPos;
                nextPos.y++;
                if (CharAtPosition(map, nextPos) != obstacle)
                {
                    currentPos = nextPos;
                    break;
                }
                currentPos.dir = Direction::Left;
                [[fallthrough]];
            }
            case Direction::Left:
            {
                auto nextPos = currentPos;
                nextPos.x--;
                if (CharAtPosition(map, nextPos) != obstacle)
                {
                    currentPos = nextPos;
                    break;
                }
                currentPos.dir = Direction::Up;
            }
        }
    }

	std::cout << "ex00: " << resultEx00 << std::endl;

    return 0;
}
