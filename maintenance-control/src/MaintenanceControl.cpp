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
using namespace std;

namespace
{
  const string stopDummy = "$STOPDUMMY$";
}

MaintenanceControl::MaintenanceControl()
{
  removerThread = thread([this]
                         {
                           while(true)
                           {
                             const string deviceId = finishedDevices.pop();
                             if(deviceId == stopDummy)
                               break;
                             jobMaster.removeJobs(deviceId);
                           }
                         });
}

MaintenanceControl::~MaintenanceControl()
{
  finishedDevices.push(string(stopDummy));
  removerThread.join();
}

void MaintenanceControl::setJobResultHandler(const JobResultHandler &jobResultHdlr)
{
  jobResultHandler = jobResultHdlr;
}

void MaintenanceControl::startMaintenance(const network::DeviceIds &deviceIds, const FileTransferJobs &fileTransferJobs)
{
  const JobResultHandler jobMasterResultHandler = [this](const string &deviceId, const JobResults &jobResults)
                                                  {
                                                    jobResultHandler(deviceId, jobResults);
                                                    finishedDevices.push(string(deviceId));
                                                  };
  jobMaster.publishJobs(deviceIds, fileTransferJobs, jobMasterResultHandler);
}

void MaintenanceControl::stopMaintenance(const network::DeviceIds &deviceIds)
{
  for(const string &deviceId : deviceIds)
    jobMaster.removeJobs(deviceId);
}
