
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
	std::stringstream buffer;
	buffer << inputFile.rdbuf();

	const auto input = buffer.str();

	size_t resultEx00 = 0;
	size_t resultEx01 = 0;
	size_t x = 0;
	size_t y = 0;

	const std::regex multiplyRegex(
		"(m)(u)(l)(\\()\\d{1,3},\\d{1,3}(\\))"
		"|"
		"(d)(o)(\\()(\\))"
		"|"
		"(d)(o)(n)(')(t)(\\()(\\))"
	);
	const std::sregex_iterator matchesBegin(input.begin(), input.end(), multiplyRegex);
	const std::sregex_iterator matchesEnd;

	bool ignoreMultiplies = false;
	for (auto match = matchesBegin; match != matchesEnd; match++)
	{
		if (match->str() == "do()")
		{
			ignoreMultiplies = false;
			continue;
		}
		if (match->str() == "don't()")
		{
			ignoreMultiplies = true;
			continue;
		}

		std::sscanf(match->str().data(), "mul(%zu,%zu)", &x, &y);
		resultEx00 += (x * y);

		if (ignoreMultiplies)
		{
			continue;
		}
	
		resultEx01 += (x * y);
	}
	std::cout << "ex00: " << resultEx00 << std::endl;
	std::cout << "ex01: " << resultEx01 << std::endl;
}
