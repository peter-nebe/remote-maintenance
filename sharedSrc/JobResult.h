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

#ifndef JOBRESULT_H_
#define JOBRESULT_H_

#include <cstdint>
#include <vector>
#include <ostream>

enum class JobError
{
  noError,
  jobInvalid,
  fileNotExist,
  fileGenerationErr,
  fileTransferErr
};

struct JobResult
{
  uint32_t jobId = 0;
  JobError error = JobError::noError;
};

typedef std::vector<JobResult> JobResults;

std::ostream& operator<<(std::ostream& os, JobError error);
std::ostream& operator<<(std::ostream& os, const JobResult &result);

#endif /* JOBRESULT_H_ */
