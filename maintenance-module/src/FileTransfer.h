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

#ifndef FILETRANSFER_H_
#define FILETRANSFER_H_

#include "FileTransferJob.h"
#include "JobResult.h"
#include <functional>

class FileTransfer
{
public:
  FileTransfer(const std::string &deviceId);

  typedef std::function<void(const std::string &localFilePath, FileType fileType, FileCategory fileCategory)> NotifyDownloadCompleted;
  void setNotifyDownloadCompleted(const NotifyDownloadCompleted &notifyDownloadCompleted);

  typedef std::function<int(const std::string &localFilePath, FileType fileType, FileCategory fileCategory, const Filter &filter)> GenerateFileToUpload;
  void setGenerateFileToUpload(const GenerateFileToUpload &generateFileToUpload);

  int doJobs(const FileTransferJobs &fileTransferJobs, JobResults &jobResults);

private:
  std::string remoteUploadPath;
  NotifyDownloadCompleted notifyDownloadCompleted = [](const std::string&, FileType, FileCategory){};
  GenerateFileToUpload generateFileToUpload = [](const std::string&, FileType, FileCategory, const Filter&){ return 0; };
};

#endif /* FILETRANSFER_H_ */
