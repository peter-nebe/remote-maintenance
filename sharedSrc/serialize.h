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

#ifndef SERIALIZE_H_
#define SERIALIZE_H_

#include "FileTransferJob.h"
#include "JobResult.h"
#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/serialization/vector.hpp"
#include <sstream>

template<typename Archive>
void serialize(Archive &ar, FilterCondition &cond, unsigned int version)
{
  ar & cond.conditionType;
  ar & cond.comparisonType;
  ar & cond.value;
}

template<typename Archive>
void serialize(Archive &ar, FileTransferJob &job, unsigned int version)
{
  ar & job.jobId;
  ar & job.transferType;
  ar & job.filePath;
  ar & job.fileType;
  ar & job.fileCategory;
  ar & job.filter;
}

template<typename Archive>
void serialize(Archive &ar, JobResult &result, unsigned int version)
{
  ar & result.jobId;
  ar & result.error;
}

typedef boost::archive::text_oarchive oarchive;
typedef boost::archive::text_iarchive iarchive;

template<typename T>
std::string serialize(const T &data)
{
  std::ostringstream oss;
  oarchive(oss) << data;

  return oss.str();
}

template<typename T>
T deserialize(const std::string &buffer)
{
  T data;
  if(!buffer.empty())
  {
    std::istringstream iss(buffer);
    iarchive(iss) >> data;
  }
  return data;
}

#endif /* SERIALIZE_H_ */
