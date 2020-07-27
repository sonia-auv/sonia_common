/**
 * \file	image_sequence_provider_inl.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	06/08/2015
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

#ifndef SONIA_COMMON_IO_IMAGE_SEQUENCE_WRITER_H_
#error This file may only be included from image_sequence_writer.h
#endif

#include <sonia_common/sys/timer.h>
#include <functional>

namespace sonia_common {

//==============================================================================
// C / D T O R S   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE ImageSequenceWriter::ImageSequenceWriter() ATLAS_NOEXCEPT
    : frame_count_(0),
      streaming_(false),
      running_(false) {}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE ImageSequenceWriter::~ImageSequenceWriter() ATLAS_NOEXCEPT {
}

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void ImageSequenceWriter::OnSubjectNotify(
    Subject<const cv::Mat &> &subject, const cv::Mat &image) ATLAS_NOEXCEPT {
  if (IsStreaming()) {
    WriteImage(image);
    ++frame_count_;
    return;
  }
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void ImageSequenceWriter::Write(const cv::Mat &image) {
  if (running_) {
    if (!IsStreaming()) {
      ++frame_count_;
      WriteImage(image);
      return;
    }
    throw std::logic_error(
        "The image writer is not running, cannot Write the image.");
  }
  throw std::logic_error(
      "The image writer is streaming, cannot Write the image.");
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void ImageSequenceWriter::Start() ATLAS_NOEXCEPT {
  running_ = true;
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void ImageSequenceWriter::Stop() ATLAS_NOEXCEPT {
  running_ = false;
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE uint64_t
ImageSequenceWriter::FrameCount() const ATLAS_NOEXCEPT {
  return frame_count_;
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE void ImageSequenceWriter::SetStreamingMode(bool streaming)
    ATLAS_NOEXCEPT {
  streaming_ = streaming;
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE bool ImageSequenceWriter::IsStreaming() const
    ATLAS_NOEXCEPT {
  return streaming_;
}

//------------------------------------------------------------------------------
//
ATLAS_ALWAYS_INLINE bool ImageSequenceWriter::IsRunning() const ATLAS_NOEXCEPT {
  return running_;
}

}  // namespace sonia_common
