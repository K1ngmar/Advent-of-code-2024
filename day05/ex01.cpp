
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

bool operator == (const Position& lhs, const Position& rhs)
{
    return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.dir == rhs.dir);
}

bool operator < (const Position& lhs, const Position& rhs)
{
    return (lhs.y < rhs.y || lhs.x < rhs.x || lhs.dir < rhs.dir);
}

static char CharAtPosition(const std::vector<std::string>& map, Position pos)
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

static bool GetsStuckInLoop(const std::vector<std::string>& map, Position startingPos)
{
    std::set<Position> visitedPositions;
    constexpr auto obstacle = '#';
    for (
        Position currentPos = startingPos;
        currentPos.y >= 0 && currentPos.y < map.size() &&
        currentPos.x >= 0 && currentPos.x < map.front().length();
    )
    {
        for (const auto& position : visitedPositions)
        {
            if (position == currentPos)
            {
                return true;
            }
        }
        visitedPositions.emplace(currentPos);
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
                break;
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
                break;
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
                break;
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
    return false;
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

    size_t resultEx01 = 0;
    for (size_t y = 0; y < map.size(); y++)
    {
        for (size_t x = 0; x < map[y].length(); x++)
        {
            char& c = map[y][x];
            if (c != '.')
            {
                continue;
            }
            c = '#';
            std::cout << "Checking position: " << y << ", " << x << std::endl;
            resultEx01 += GetsStuckInLoop(map, guardStartingPosition.value());
            c = '.';
        }
    }

	std::cout << "ex01: " << resultEx01 << std::endl;

    return 0;
}
