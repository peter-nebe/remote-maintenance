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

#ifndef JOBSLAVE_H_
#define JOBSLAVE_H_

#include "serialize.h"
#include "NetworkClient.h"

template<typename JobsType>
class JobSlave
{
public:
  JobSlave(const std::string &deviceId);

  int queryJobs(JobsType &jobs)
  {
    std::string buffer;
    const int ret = networkClient.query(topic, buffer);
    jobs = deserialize<JobsType>(buffer);

    return ret;
  }

  int publishJobResults(const JobResults &jobResults)
  {
    return networkClient.publish(resultsTopic, serialize(jobResults));
  }

private:
  const std::string topic;
  const std::string resultsTopic;
  NetworkClient networkClient;
};

#endif /* JOBSLAVE_H_ */
