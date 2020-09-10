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

#ifndef JOBMASTER_H_
#define JOBMASTER_H_

#include "network.h"
#include "serialize.h"
#include "NetworkClient.h"
#include <functional>

typedef std::function<void(const std::string &deviceId, const JobResults &jobResults)> JobResultHandler;

template<typename JobsType>
class JobMaster
{
public:
  JobMaster();

  int publishJobs(const network::DeviceIds &deviceIds, const JobsType &jobs, const JobResultHandler &jobResultHandler)
  {
    const NetworkClient::MessageHandler messageHandler = [this, jobResultHandler](const std::string &topic, const std::string &payload)
                                                         {
                                                           jobResultHandler(topic.substr(resultsTopic.length()), deserialize<JobResults>(payload));
                                                         };
    return networkClient.publishStartConsuming(topic, serialize(jobs), deviceIds, resultsTopic, messageHandler);
  }

  int removeJobs(const std::string &deviceId)
  {
    return networkClient.remove(topic + deviceId, resultsTopic + deviceId);
  }

private:
  const std::string topic;
  const std::string resultsTopic;
  NetworkClient networkClient;
};

#endif /* JOBMASTER_H_ */
