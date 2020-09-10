/*
 * remote-maintenance
 * Copyright (c) 2020 Peter Nebe (mail@peter-nebe.dev)
 *
 * This file is part of remote-maintenance.
 *
 * remote-maintenance is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * remote-maintenance is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with remote-maintenance.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <string>
#include <vector>
#include <ostream>

enum class FilterConditionType
{
  invalid,
  time,
  severity,
  component,
  id
};

enum class ComparisonType
{
  setpoint,
  lowerLimit,
  upperLimit
};

struct FilterCondition
{
  FilterConditionType conditionType;
  ComparisonType comparisonType;
  std::string value;
};

typedef std::vector<FilterCondition> Filter;

std::ostream& operator<<(std::ostream& os, FilterConditionType conditionType);
std::ostream& operator<<(std::ostream& os, ComparisonType comparisonType);
std::ostream& operator<<(std::ostream& os, const FilterCondition &condition);
std::ostream& operator<<(std::ostream& os, const Filter &filter);

#endif /* FILTER_H_ */
