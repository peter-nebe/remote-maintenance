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

#include "FileTransferJob.h"
#include "JobResult.h"
#include <map>
using namespace std;

namespace
{
  const string delimiter = ", ";
}

ostream& operator<<(ostream& os, FilterConditionType conditionType)
{
  static const map<FilterConditionType, string> strings
  {
    { FilterConditionType::invalid, "invalid" },
    { FilterConditionType::time, "time" },
    { FilterConditionType::severity, "severity" },
    { FilterConditionType::component, "component" },
    { FilterConditionType::id, "id" }
  };
  return os << strings.at(conditionType);
}

ostream& operator<<(ostream& os, ComparisonType comparisonType)
{
  static const map<ComparisonType, string> strings
  {
    { ComparisonType::setpoint, "setpoint" },
    { ComparisonType::lowerLimit, "lowerLimit" },
    { ComparisonType::upperLimit, "upperLimit" }
  };
  return os << strings.at(comparisonType);
}

ostream& operator<<(ostream& os, const FilterCondition &cond)
{
  return os << cond.conditionType << delimiter
            << cond.comparisonType << delimiter
            << cond.value;
}

ostream& operator<<(ostream& os, const Filter &filter)
{
  if(!filter.empty())
  {
    const size_t n = filter.size() - 1;
    for(size_t i = 0; i < n; i++)
      os << filter[i] << delimiter;
    os << filter[n];
  }
  return os;
}

ostream& operator<<(ostream& os, FileTransferType transferType)
{
  static const map<FileTransferType, string> strings
  {
    { FileTransferType::invalid, "invalid" },
    { FileTransferType::download, "download" },
    { FileTransferType::upload, "upload" },
    { FileTransferType::deleteFile, "deleteFile" }
  };
  return os << strings.at(transferType);
}

ostream& operator<<(ostream& os, FileType fileType)
{
  static const map<FileType, string> strings
  {
    { FileType::unspecified, "unspecified" },
    { FileType::executable, "executable" },
    { FileType::configuration, "configuration" },
    { FileType::logFile, "logFile" },
    { FileType::statusReport, "statusReport" },
    { FileType::errorReport, "errorReport" },
    { FileType::package, "package" },
    { FileType::archive, "archive" }
  };
  return os << strings.at(fileType);
}

ostream& operator<<(ostream& os, FileCategory fileCategory)
{
  static const map<FileCategory, string> strings
  {
    { FileCategory::unspecified, "unspecified" },
    { FileCategory::application, "application" },
    { FileCategory::middleware, "middleware" },
    { FileCategory::system, "system" }
  };
  return os << strings.at(fileCategory);
}

ostream& operator<<(ostream& os, const FileTransferJob &job)
{
  os << job.jobId << delimiter
     << job.transferType << delimiter
     << job.filePath << delimiter
     << job.fileType << delimiter
     << job.fileCategory;

  if(!job.filter.empty())
    os << delimiter << job.filter;

  return os;
}

ostream& operator<<(ostream& os, JobError error)
{
  static const map<JobError, string> strings
  {
    { JobError::noError, "noError" },
    { JobError::jobInvalid, "jobInvalid" },
    { JobError::fileNotExist, "fileNotExist" },
    { JobError::fileGenerationErr, "fileGenerationErr" },
    { JobError::fileTransferErr, "fileTransferErr" }
  };
  return os << strings.at(error);
}

ostream& operator<<(ostream& os, const JobResult &result)
{
  return os << result.jobId << delimiter
            << result.error;
}
