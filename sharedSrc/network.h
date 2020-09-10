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

#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <vector>

namespace network
{
  // URI of the MQTT server
  const std::string serverUri = "ssl://localhost:18884"; // To do: replace localhost

  // Parts of the message topics
  const std::string prefix = "maintenance";
  const std::string fileTransfer = "/file transfer";
  const std::string jobResults = "/job results";

  // Name of the file transfer server
  const std::string fileServer = "localhost"; // To do: replace localhost

  // List of devices to be controlled
  typedef std::vector<std::string> DeviceIds;
  const DeviceIds deviceIds
  {
    "/device/01", // To do: replace IDs
    "/device/02",
    "/device/03"
  };
} // namespace network

#endif /* NETWORK_H_ */
