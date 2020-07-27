/**
 * \file	image_publisher.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	23/05/2015
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

#ifndef SONIA_COMMON_ROS_IMAGE_SUBSCRIBER_H_
#define SONIA_COMMON_ROS_IMAGE_SUBSCRIBER_H_

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <ros/ros.h>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>

#include <sonia_common/io/image_sequence_capture.h>
#include <sonia_common/macros.h>

namespace sonia_common {

class ImageSubscriber : public ImageSequenceCapture {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<ImageSubscriber>;

  //============================================================================
  // C O N S T R U C T O R S   A N D   D E S T R U C T O R

  explicit ImageSubscriber(const std::string &topic_name)
      : topic_name_(topic_name),
        img_transport_(ros::NodeHandle()),
        subscriber_(img_transport_.subscribe(
            topic_name_, 1, &ImageSubscriber::ImageCallback, this)),
        image_(),
        topic_mutex_() {}

  virtual ~ImageSubscriber() = default;

  //============================================================================
  // P U B L I C   M E T H O D S

  ATLAS_ALWAYS_INLINE const cv::Mat &GetImage() const { return image_; }

 private:
  //============================================================================
  // P R I V A T E   M E T H O D S

  void ImageCallback(const sensor_msgs::ImageConstPtr &msg) {
    try {
      auto cv_image_ptr = cv_bridge::toCvCopy(msg, "bgr8");
      image_ = cv_image_ptr->image;
    } catch (cv_bridge::Exception &e) {
      ROS_ERROR("Unable to convert %s image to bgr8", msg->encoding.c_str());
    }
  }

  //============================================================================
  // P R I V A T E   M E M B E R S

  const std::string topic_name_;

  image_transport::ImageTransport img_transport_;

  image_transport::Subscriber subscriber_;

  cv::Mat image_;

  mutable std::mutex topic_mutex_;
};

}  // namespace sonia_common

#endif  // SONIA_COMMON_ROS_IMAGE_SUBSCRIBER_H_
