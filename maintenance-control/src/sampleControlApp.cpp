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

#include "MaintenanceControl.h"
#include "log.h"
using namespace std;

int main(int argc, char *argv[])
{
  const Filter logFilter
  {
    { FilterConditionType::time, ComparisonType::lowerLimit, "2020-09-01" },
    { FilterConditionType::severity, ComparisonType::lowerLimit, "info" }
  };
  const FileTransferJobs jobs
  {
    { 1001, FileTransferType::download, "/testDummyFile1", FileType::executable, FileCategory::application },
    { 1002, FileTransferType::download, "/testDummyFile2", FileType::configuration, FileCategory::system },
    { 1003, FileTransferType::upload, "/testDummyFile3", FileType::logFile, FileCategory::application, logFilter },
    { 1004, FileTransferType::deleteFile, "/testDummyStatusReport1", FileType::statusReport, FileCategory::application }
  };

  MaintenanceControl mtnCtl;
  mtnCtl.setJobResultHandler([](const string &deviceId, const JobResults &jobResults)
    {
      loginfo << "received " << jobResults.size() << " job results from " << deviceId;
      for(const JobResult &result : jobResults)
        loginfo << "job result " << result;
    });

  mtnCtl.startMaintenance(network::deviceIds, jobs);

  string input;
  cin >> input;

  mtnCtl.stopMaintenance(network::deviceIds);

  return 0;
}
