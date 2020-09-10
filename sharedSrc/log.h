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

#ifndef LOG_H_
#define LOG_H_

#define BOOST_ALL_DYN_LINK 1
#include "boost/log/trivial.hpp"

#define logtrace BOOST_LOG_TRIVIAL(trace)
#define logdebug BOOST_LOG_TRIVIAL(debug)
#define loginfo BOOST_LOG_TRIVIAL(info)
#define logwarning BOOST_LOG_TRIVIAL(warning)
#define logerror BOOST_LOG_TRIVIAL(error)
#define logfatal BOOST_LOG_TRIVIAL(fatal)

#endif /* LOG_H_ */
