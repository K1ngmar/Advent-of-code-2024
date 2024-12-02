
#include <fstream>
#include <set>
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
#include <cassert>

template<std::ranges::forward_range View>
static bool IsReportSafe(
	View& report,
	const size_t minimumLevelDifference,
	const size_t maximumLevelDifference)
{
	const bool isIncreasing = report.front() < *(++report.begin());
	for (const auto& values : report | std::views::slide(2))
	{
		const auto previousVal = values.front();
		const auto currentVal = *(++values.begin());

		if (previousVal < currentVal != isIncreasing)
		{
			return false;
		}

		const auto difference = std::abs(previousVal - currentVal);
		if (difference < minimumLevelDifference || difference > maximumLevelDifference)
		{
			return false;
		}
	}
	return true;
}

static bool IsReportTolerable(
	const std::ranges::ref_view<std::vector<int> >& report,
	const size_t minimumLevelDifference,
	const size_t maximumLevelDifference)
{
	if (IsReportSafe(report, minimumLevelDifference, maximumLevelDifference))
	{
		return true;
	}
	for (size_t i = 0; i < report.size(); i++)
	{
		const auto filter = [i](auto &&pair)
		{
			return std::get<0>(pair) != i;
		};

		auto filteredReport = report | std::views::enumerate | std::views::filter(filter) | std::views::values;
		if (IsReportSafe(filteredReport, minimumLevelDifference, maximumLevelDifference))
		{
			return true;
		}
	}
	return false;
}

int main(void)
{
	constexpr auto minimumLevelDifference = 1;
	constexpr auto maximumLevelDifference = 3;

	std::ifstream inputFile("input.txt");

	size_t numberOfSafeReports = 0;
	size_t numberOfTorelatedReports = 0;
	for (std::string line; std::getline(inputFile, line);)
	{
		std::vector<int32_t> report;
		for (const auto valStr : std::views::split(line, ' '))
		{
			report.emplace_back(std::stoi(valStr.data()));
		}

		auto reportView = std::views::all(report);
		numberOfSafeReports += IsReportSafe(reportView, minimumLevelDifference, maximumLevelDifference);
		numberOfTorelatedReports += IsReportTolerable(reportView, minimumLevelDifference, maximumLevelDifference);
	}
	std::cout << "ex00: " << numberOfSafeReports << std::endl;
	std::cout << "ex01: " << numberOfTorelatedReports << std::endl;
}