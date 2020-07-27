/**
 * \file service_client_manager.h
 * \author Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date 23/05/2015
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

#ifndef SONIA_COMMON_ROS_SERVICE_CLIENT_MANAGER_H_
#define SONIA_COMMON_ROS_SERVICE_CLIENT_MANAGER_H_

#include <sonia_common/macros.h>
#include <ros/ros.h>
#include <memory>

namespace sonia_common {

/// This class is an helper for storing ServiceClient.
///
/// By inheriting this class and then call the RegisterService, you abstract
/// the managment of storing and deleting ROS Services.
class ServiceClientManager {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<ServiceClientManager>;

  static constexpr unsigned short kConnectionAttempts = 3;

  //============================================================================
  // C O N S T R U C T O R S   A N D   D E S T R U C T O R

  explicit ServiceClientManager() ATLAS_NOEXCEPT;

  virtual ~ServiceClientManager() {
    for (auto &service : services_) {
      service.second.shutdown();
    }
  }

  //============================================================================
  // P U B L I C   M E T H O D S

  /// The method register a service given its name and a pointer to the callback
  /// method that will handle the callback.
  ///
  /// \param name  The name of the service you want to register.
  /// \param function  A pointer to the the defined callback.
  /// \param manager Take a reference to the real object in order to call
  /// ROS advertiseService.
  template <typename M>
  void RegisterService(const std::string &service_name) {
    auto result_advertise = node_handler_.serviceClient<M>(service_name);
    auto pair = std::pair<std::string, ros::ServiceClient>(service_name,
                                                           result_advertise);
    services_.insert(pair);
  }

  /// Shutdown a service given its name.
  ///
  /// \return True if the service was shutdown correctly.
  bool ShutdownService(const std::string &service_name) {
    for (auto &service : services_) {
      if (service.first == service_name) {
        service.second.shutdown();
        services_.erase(service.first);
        return true;
      }
    }
    return false;
  }

  /// Get a service given its name.
  ///
  /// \return A pointer to the service. This will return nullptr if there is no
  /// pointer with this name.
  ros::ServiceClient *const GetService(const std::string &service_name) {
    for (auto &service : services_) {
      if (service.first == service_name) {
        return &(service.second);
      }
    }
    return nullptr;
  }

  template <typename T>
  bool SecureCall(const T &service, std::string &node) {
    for (int i = 0; i < kConnectionAttempts; ++i) {
      if (services_.at(node).call(service)) {
        return true;
      }
    }
    return false;
  }

 private:
  //============================================================================
  // P R I V A T E   M E M B E R S

  /// The Node Handler provided by ROS to manage nodes
  ros::NodeHandle node_handler_;

  /// List of ROS services offered by this class
  std::map<std::string, ros::ServiceClient> services_;
};

}  // namespace sonia_common

#endif  // SONIA_COMMON_ROS_SERVICE_CLIENT_MANAGER_H_