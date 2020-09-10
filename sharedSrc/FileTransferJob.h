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

#ifndef FILETRANSFERJOB_H_
#define FILETRANSFERJOB_H_

#include "Filter.h"

enum class FileTransferType
{
  invalid,
  download,
  upload,
  deleteFile
};

enum class FileType
{
  unspecified,
  executable,
  configuration,
  logFile,
  statusReport,
  errorReport,
  package,
  archive
};

enum class FileCategory
{
  unspecified,
  application,
  middleware,
  system
};

struct FileTransferJob
{
  uint32_t jobId = 0;
  FileTransferType transferType = FileTransferType::invalid;
  std::string filePath;
  FileType fileType = FileType::unspecified;
  FileCategory fileCategory = FileCategory::unspecified;
  Filter filter;
};

typedef std::vector<FileTransferJob> FileTransferJobs;

std::ostream& operator<<(std::ostream& os, FileTransferType transferType);
std::ostream& operator<<(std::ostream& os, FileType fileType);
std::ostream& operator<<(std::ostream& os, FileCategory fileCategory);
std::ostream& operator<<(std::ostream& os, const FileTransferJob &job);

#endif /* FILETRANSFERJOB_H_ */
