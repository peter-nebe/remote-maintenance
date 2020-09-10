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

#ifndef NETWORKCLIENT_H_
#define NETWORKCLIENT_H_

#include "mqtt/client.h"
#include <string>
#include <vector>
#include <functional>

class NetworkClient
{
public:
  NetworkClient(const std::string &clientId);

  int publish(const std::string &topic, const std::string &payload);
  int query(const std::string &topic, std::string &payload);

  typedef std::vector<std::string> TopicExtensions;
  typedef std::function<void(const std::string &topic, const std::string &payload)> MessageHandler;
  int publishStartConsuming(const std::string &topicToPublish, const std::string &payload, const TopicExtensions &topicExtensions,
                            const std::string &topicToQuery, const MessageHandler &messageHandler);
  int remove(const std::string &topicToPublish, const std::string &topicToQuery);

private:
  void connect();

  struct Callback : mqtt::callback
  {
    void message_arrived(mqtt::const_message_ptr msg) override;
    MessageHandler messageHandler;
  }
  callback;
  mqtt::client client;
};

#endif /* NETWORKCLIENT_H_ */
