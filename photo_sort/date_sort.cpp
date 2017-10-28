#include "stdafx.h"
#include "date_sort.hpp"
#include "utility.hpp"

// This is the date library by Howard Hinnant
// https://howardhinnant.github.io/date/date.html
#include <date\date.h>

namespace photo_sort
{
	namespace fs = std::experimental::filesystem;
	bool date_compare::operator()(const fs::directory_entry& lhs, const fs::directory_entry& rhs) const
	{
		if (!utility::matches_regex(lhs) || ! utility::matches_regex(rhs)) return false;

		auto lhs_time = fs::last_write_time(lhs);
		auto rhs_time = fs::last_write_time(rhs);

		const auto lhs_date = date::year_month_day(date::trunc<date::days>(lhs_time));
		const auto rhs_date = date::year_month_day(date::trunc<date::days>(rhs_time));

		const int ly = static_cast<int>(lhs_date.year());
		const unsigned int lm = static_cast<unsigned int>(lhs_date.month());
		const int ld = static_cast<unsigned int>(lhs_date.day());

		const int ry = static_cast<int>(rhs_date.year());
		const unsigned int rm = static_cast<unsigned int>(rhs_date.month());
		const int rd = static_cast<unsigned int>(rhs_date.day());

		if (ly != ry)
			return ly < ry;

		else if (lm != rm)
			return lm < rm;

		return ld < rd;

	}

	std::string date_builder::operator()(const fs::path& first, const fs::path& last) const
	{
		const auto lhs_date = date::year_month_day(date::floor<date::days>(fs::last_write_time(first)));
		const auto rhs_date = date::year_month_day(date::floor<date::days>(fs::last_write_time(last)));

		std::stringstream builder;

		builder 
			<< lhs_date.month()
			<< " " << lhs_date.day()
			<< ", " << lhs_date.year();

		builder 
			<< " - "
			<< rhs_date.month()
			<< " " << rhs_date.day()
			<< ", " << rhs_date.year();

		return builder.str();
	}

}