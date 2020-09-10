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

#ifndef MAINTENANCE_H_
#define MAINTENANCE_H_

#include "JobSlave.h"

class Maintenance
{
public:
  Maintenance(const std::string &deviceId);
  virtual ~Maintenance();

  typedef std::function<void(const FileTransferJobs &fileTransferJobs, JobResults &jobResults)> JobHandler;
  typedef std::function<void()> IdleHandler;

  void setJobHandler(const JobHandler &jobHandler);
  void setIdleHandler(const IdleHandler &idleHandler);
  void setQueryPeriod(const std::chrono::seconds &queryPeriod);

  void startMaintenanceMode();
  void stopMaintenanceMode();

private:
  void threadFunc();

  JobSlave<FileTransferJobs> jobSlave;
  JobHandler jobHandler = [](const FileTransferJobs&, JobResults&){};
  IdleHandler idleHandler = []{};
  std::chrono::seconds queryPeriod = std::chrono::seconds(3600);
  std::thread thrd;
  bool stop = true;
  std::mutex stopMutex;
  std::condition_variable stopCondition;
};

#endif /* MAINTENANCE_H_ */
