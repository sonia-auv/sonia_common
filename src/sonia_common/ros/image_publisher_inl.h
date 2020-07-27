/**
 * \file	image_publisher_inl.h
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

#ifndef SONIA_COMMON_ROS_IMAGE_PUBLISHER_H_
#error This file may only be included from image_publisher.h
#endif

#include <algorithm>
#include <cassert>

namespace sonia_common {

//==============================================================================
// C / D T O R S   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE ImagePublisher::ImagePublisher(
    const std::string &topic_name) ATLAS_NOEXCEPT
    : topic_name_(topic_name),
      img_transport_(ros::NodeHandle()),
      publisher_(img_transport_.advertise(topic_name_, 1)) {}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE ImagePublisher::~ImagePublisher() ATLAS_NOEXCEPT {
  publisher_.shutdown();
}

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void ImagePublisher::WriteImage(const cv::Mat &image)
    ATLAS_NOEXCEPT {
  if (!image.empty()) {
    sensor_msgs::ImagePtr msg =
        cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();
    msg->header.stamp = ros::Time::now();
    publisher_.publish(msg);
    cv::waitKey(1);
  }
}

}  // namespace sonia_common
