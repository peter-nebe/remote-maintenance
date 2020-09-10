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

#ifndef MAINTENANCECONTROL_H_
#define MAINTENANCECONTROL_H_

#include "JobMaster.h"
#include "fifo.h"
#include <thread>

class MaintenanceControl
{
public:
  MaintenanceControl();
  virtual ~MaintenanceControl();

  void setJobResultHandler(const JobResultHandler &jobResultHandler);

  void startMaintenance(const network::DeviceIds &deviceIds, const FileTransferJobs &fileTransferJobs);
  void stopMaintenance(const network::DeviceIds &deviceIds);

private:
  JobMaster<FileTransferJobs> jobMaster;
  JobResultHandler jobResultHandler = [](const std::string&, const JobResults&){};
  Fifo<std::string> finishedDevices;
  std::thread removerThread;
};

#endif /* MAINTENANCECONTROL_H_ */
