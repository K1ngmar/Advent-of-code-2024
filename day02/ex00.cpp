
#include <fstream>
#include <set>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <cassert>
#include <regex>

int main(void)
{
	std::ifstream inputFile("input.txt");

	size_t result = 0;
	for (std::string line; std::getline(inputFile, line);)
	{
		size_t x = 0;
		size_t y = 0;

		std::regex multiplyRegex("(m)(u)(l)(\\()\\d{1,3},\\d{1,3}(\\))");
		const auto matchesBegin = std::sregex_iterator(line.begin(), line.end(), multiplyRegex);
		const auto matchesEnd = std::sregex_iterator();
		for (auto match = matchesBegin; match != matchesEnd; match++)
		{
			std::sscanf(match->str().data(), "mul(%zu,%zu)", &x, &y);
			result += (x * y);
		}
	}
	std::cout << "ex00: " << result << std::endl;
}