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

#include "FileTransfer.h"
#include "network.h"
#include <sstream>
using namespace std;

namespace
{
  const string localPathPrefix = ".";
  const string remotePathPrefix = network::fileServer + ":~/maintenance-control";
  const string downloadDir = "/download";
  const string uploadDir = "/upload";
  const string appConfigDir = localPathPrefix + "/config";
  const string appLogDir = localPathPrefix + "/log";
}

FileTransfer::FileTransfer(const string &deviceId)
: remoteUploadPath(remotePathPrefix + uploadDir + deviceId)
{
}

void FileTransfer::setNotifyDownloadCompleted(const NotifyDownloadCompleted &notify)
{
  notifyDownloadCompleted = notify;
}

void FileTransfer::setGenerateFileToUpload(const GenerateFileToUpload &generate)
{
  generateFileToUpload = generate;
}

namespace
{

const string copyCmd = "scp -i ";
const string identityFile = "~/maintenance-module/sshIdentityFile";

int transferFile(const string &source, const string &destination)
{
  ostringstream command;
  command << copyCmd << identityFile << ' '
          << source << ' ' << destination;
  return system(command.str().c_str());
}

string getLocalPathPrefix(FileType fileType, FileCategory fileCategory)
{
  if(fileCategory == FileCategory::application)
  {
    switch(fileType)
    {
      case FileType::configuration:
        return appConfigDir;

      case FileType::logFile:
      case FileType::statusReport:
      case FileType::errorReport:
        return appLogDir;

      default:
        ;
    }
  }

  return string();
}

} // namespace

int FileTransfer::doJobs(const FileTransferJobs &fileTransferJobs, JobResults &jobResults)
{
  jobResults.clear();
  jobResults.reserve(fileTransferJobs.size());

  for(const FileTransferJob &job : fileTransferJobs)
  {
    JobError jobError = JobError::noError;

    switch(job.transferType)
    {
      case FileTransferType::download:
      {
        const string localFilePath = localPathPrefix + downloadDir + job.filePath;
        if(transferFile(remotePathPrefix + downloadDir + job.filePath, localFilePath) == 0)
          notifyDownloadCompleted(localFilePath, job.fileType, job.fileCategory);
        else
          jobError = JobError::fileTransferErr;
        break;
      }

      case FileTransferType::upload:
      {
        const string localFilePath = localPathPrefix + uploadDir + job.filePath;
        if(generateFileToUpload(localFilePath, job.fileType, job.fileCategory, job.filter))
          jobError = JobError::fileGenerationErr;
        else if(transferFile(localFilePath, remoteUploadPath + job.filePath))
          jobError = JobError::fileTransferErr;
        break;
      }

      case FileTransferType::deleteFile:
        if(remove((getLocalPathPrefix(job.fileType, job.fileCategory) + job.filePath).c_str()))
          jobError = JobError::fileTransferErr;
        break;

      default:
        jobError = JobError::jobInvalid;
    }

    jobResults.push_back({ job.jobId, jobError });
  }

  return 0;
}
