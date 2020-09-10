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

#include "network.h"
#include "log.h"
#include "FileTransfer.h"
#include "Maintenance.h"
using namespace std;

int main(int argc, char *argv[])
{
  const int index = (argc == 2) ? stoi(argv[1]) : 0;
  const string deviceId = network::deviceIds[index];
  logdebug << "this is " << deviceId;

  FileTransfer fileTransfer(deviceId);
  fileTransfer.setNotifyDownloadCompleted([](const string &localFilePath, FileType fileType, FileCategory fileCategory)
    {
      logtrace << "notifying about completed download, " << localFilePath << ", " << fileType << ", " << fileCategory;
      // To do: notify the module that should process the downloaded file (e. g. installer, configurator, updater ...)

    });
  fileTransfer.setGenerateFileToUpload([](const string &localFilePath, FileType fileType, FileCategory fileCategory, const Filter &filter)
    {
      logtrace << "invoking a module to generate the file to upload, "
        << localFilePath << ", " << fileType << ", " << fileCategory << (filter.empty() ? string() : ", ") << filter;
      // To do: invoke the module that should generate the file to upload (e. g. logger, configurator ...)

      return 0;
    });

  Maintenance mtn(deviceId);
  mtn.setJobHandler([&fileTransfer](const FileTransferJobs &fileTransferJobs, JobResults &jobResults)
    {
      loginfo << "got " << fileTransferJobs.size() << " file transfer jobs";
      for(const FileTransferJob &job : fileTransferJobs)
        loginfo << "job " << job;

      fileTransfer.doJobs(fileTransferJobs, jobResults);

      const string resultText = "job result ";
      for(const JobResult &result : jobResults)
        if(result.error == JobError::noError)
          loginfo << resultText << result;
        else
          logerror << resultText << result;
    });
  mtn.setIdleHandler([]
    {
      logtrace << "no file transfer job";
    });
  mtn.setQueryPeriod(10s);

  mtn.startMaintenanceMode();

  string input;
  cin >> input;

  mtn.stopMaintenanceMode();

  return 0;
}
