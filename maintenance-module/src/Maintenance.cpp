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

#include "Maintenance.h"
#include "network.h"
using namespace std;

Maintenance::Maintenance(const std::string &deviceId)
: jobSlave(deviceId)
{
}

Maintenance::~Maintenance()
{
  stopMaintenanceMode();
}

void Maintenance::setJobHandler(const JobHandler &jobHdlr)
{
  jobHandler = jobHdlr;
}
void Maintenance::setIdleHandler(const IdleHandler &idleHdlr)
{
  idleHandler = idleHdlr;
}

void Maintenance::setQueryPeriod(const chrono::seconds &queryPrd)
{
  queryPeriod = queryPrd;
}

void Maintenance::startMaintenanceMode()
{
  stop = false;
  thrd = thread(&Maintenance::threadFunc, this);
}

void Maintenance::stopMaintenanceMode()
{
  {
    unique_lock<mutex> lock(stopMutex);
    stop = true;
  }
  stopCondition.notify_one();

  if(thrd.joinable())
    thrd.join();
}

void Maintenance::threadFunc()
{
  while(true)
  {
    FileTransferJobs fileTransferJobs;
    jobSlave.queryJobs(fileTransferJobs);

    if(fileTransferJobs.empty())
    {
      idleHandler();
    }
    else
    {
      JobResults jobResults;
      jobHandler(fileTransferJobs, jobResults);
      jobSlave.publishJobResults(jobResults);
    }

    auto until = chrono::steady_clock::now() + queryPeriod;
    {
      unique_lock<mutex> lock(stopMutex);
      if(stopCondition.wait_until(lock, until, [this]{ return stop; }))
        break;
    }
  }
}
