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

#ifndef SONIA_COMMON_ROS_IMAGE_PUBLISHER_H_
#define SONIA_COMMON_ROS_IMAGE_PUBLISHER_H_

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sonia_common/io/image_sequence_writer.h>
#include <sonia_common/macros.h>
#include <ros/ros.h>
#include <memory>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <string>

namespace sonia_common {

class ImagePublisher : public ImageSequenceWriter {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<ImagePublisher>;

  //============================================================================
  // C O N S T R U C T O R S   A N D   D E S T R U C T O R

  explicit ImagePublisher(const std::string &topic_name) ATLAS_NOEXCEPT;

  ~ImagePublisher() ATLAS_NOEXCEPT;

 private:
  //============================================================================
  // P R I V A T E   M E T H O D S

  /**
   * Publish an image on the topic.
   *
   * This is using the paradigm of ImageSequenceWriter for publishing the image
   * on the topic.
   *
   * If this image writer is streaming and listening a Subject<const cv::Mat &>,
   * this method will be called for every image that is streamed by the
   * provider.
   *
   * For exemple, you could attach this ImageSequenceWriter to a class that
   * stream the content of a video file in order to publish it to a topic:
   * Everything is going to be handled by the system.
   */
  void WriteImage(const cv::Mat &image) ATLAS_NOEXCEPT override;

  //============================================================================
  // P R I V A T E   M E M B E R S

  std::string topic_name_;

  image_transport::ImageTransport img_transport_;

  image_transport::Publisher publisher_;
};

}  // namespace sonia_common

#include <sonia_common/ros/image_publisher_inl.h>

#endif  // SONIA_COMMON_ROS_IMAGE_PUBLISHER_H_
