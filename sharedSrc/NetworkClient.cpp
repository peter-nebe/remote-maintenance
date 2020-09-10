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

#include "NetworkClient.h"
#include "network.h"
using namespace std;

NetworkClient::NetworkClient(const string &clientId)
: client(network::serverUri, network::prefix + clientId)
{
}

void NetworkClient::connect()
{
  const string home = getenv("HOME");
  const string dir = home + "/.mqtt/";
  const string keyStore = dir + "client.pem"; // To do: replace key store
  const string trustStore = dir + "test-root-ca.crt"; // To do: replace trust store

  mqtt::ssl_options sslopts;
  sslopts.set_trust_store(trustStore);
  sslopts.set_key_store(keyStore);

  mqtt::connect_options connopts;
  connopts.set_ssl(sslopts);

  client.connect(connopts);
}

namespace
{
  const int defaultQos = 1;
  const bool retained = true;
}

int NetworkClient::publish(const string &topic, const string &payload)
{
  try
  {
    connect();

    client.publish(mqtt::message(topic, payload, defaultQos, retained));
    client.disconnect();
  }
  catch(const mqtt::exception &ex)
  {
    cerr << ex.what() << endl;
    return 1;
  }

  return 0;
}

int NetworkClient::query(const string &topic, string &payload)
{
  payload.clear();

  try
  {
    connect();

    client.start_consuming();
    client.subscribe(topic, defaultQos);

    mqtt::const_message_ptr msg;
    if(client.try_consume_message_for(&msg, 1s))
      payload = msg->get_payload();
    client.disconnect();
  }
  catch(const mqtt::exception &ex)
  {
    cerr << ex.what() << endl;
    return 1;
  }

  return 0;
}

int NetworkClient::publishStartConsuming(const string &topicToPublish, const string &payload, const TopicExtensions &topicExtensions,
                                         const string &topicToQuery, const MessageHandler &msgHandler)
{
  callback.messageHandler = msgHandler;
  client.set_callback(callback);

  try
  {
    connect();

    for(const string &extension : topicExtensions)
      client.publish(topicToQuery + extension, nullptr, 0, defaultQos, retained);

    client.start_consuming();
    for(const string &extension : topicExtensions)
      client.subscribe(topicToQuery + extension, defaultQos);

    for(const string &extension : topicExtensions)
      client.publish(mqtt::message(topicToPublish + extension, payload, defaultQos, retained));
  }
  catch(const mqtt::exception &ex)
  {
    cerr << ex.what() << endl;
    return 1;
  }

  return 0;
}

int NetworkClient::remove(const string &topicToPublish, const string &topicToQuery)
{
  try
  {
    client.unsubscribe(topicToQuery);
    client.publish(topicToQuery, nullptr, 0, defaultQos, retained);
    client.publish(topicToPublish, nullptr, 0, defaultQos, retained);
  }
  catch(const mqtt::exception &ex)
  {
    cerr << ex.what() << endl;
    return 1;
  }

  return 0;
}

void NetworkClient::Callback::message_arrived(mqtt::const_message_ptr msg)
{
  messageHandler(msg->get_topic(), msg->get_payload());
}
