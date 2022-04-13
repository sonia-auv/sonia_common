/**
 * \file	config.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	28/06/2015
 *
 * \copyright Copyright (c) 2015 S.O.N.I.A. All rights reserved.
 *
 * \section LICENSE
 *
 * This file is part of S.O.N.I.A. software.
 *
 * S.O.N.I.A. software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * S.O.N.I.A. software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with S.O.N.I.A. software. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string>
#include "sonia_common/macros.h"

#ifndef SONIA_COMMON_CONFIG_H_
#define SONIA_COMMON_CONFIG_H_

namespace sonia_common {

/// The path where the system will save all the configurations.
#ifdef OS_DARWIN
    const std::string kWorkspaceRoot = std::getenv("ROS_WS_SETUP") ? std::getenv("ROS_WS_SETUP") : "/opt/ros/noetic/setup.bash";
#else
    const std::string kWorkspaceRoot = std::getenv("ROS_WS_SETUP") ? std::getenv("ROS_WS_SETUP") : "/opt/ros/noetic/setup.bash";
#endif

/// The path where the system will save all the log files (e.g. from Logger).
const std::string kLogPath = kWorkspaceRoot + std::string{"log"};

}  // namespace sonia_common

#endif  // SONIA_COMMON_CONFIG_H_
