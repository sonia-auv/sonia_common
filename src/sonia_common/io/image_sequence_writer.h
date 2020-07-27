/**
 * \file	image_sequence_provider.h
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
#define SONIA_COMMON_IO_IMAGE_SEQUENCE_WRITER_H_

#include <sonia_common/macros.h>
#include <sonia_common/pattern/observer.h>
#include <sonia_common/sys/timer.h>
#include <atomic>
#include <memory>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <thread>

namespace sonia_common {

class ImageSequenceWriter : public Observer<const cv::Mat &> {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<ImageSequenceWriter>;

  //============================================================================
  // P U B L I C   C / D T O R S

  ImageSequenceWriter() ATLAS_NOEXCEPT;

  virtual ~ImageSequenceWriter() ATLAS_NOEXCEPT;

  //============================================================================
  // P U B L I C  M E T H O D S

  /**
   * \return The total of frame count from the moment the ImageSequenceProvider
   *         have been created.
   */
  virtual uint64_t FrameCount() const ATLAS_NOEXCEPT;

  /**
   * If the ImageSequenceProvider is not streaming, this will return the next
   * image of the sequence.
   *
   * If the ImageSequenceProvider is currently streaming, the image will rather
   * be sent to all the observer and the users should not try to get the image
   * manually.
   *
   * \return The next image, if the ImageSequenceProvider is not streaming.
   */
  void Write(const cv::Mat &);

  /**
   * Start the ImageSequenceProvider by Openning the media -- see Open().
   */
  void Start() ATLAS_NOEXCEPT;

  /**
   * Stop the ImageSequenceProvider by closing the media -- see Close().
   */
  void Stop() ATLAS_NOEXCEPT;

  /**
   * Returns either if the ImageSequence is running or not.
   *
   * You can set the running state of the ImageSequence by calling start or stop
   * methods.
   *
   * \return The running state of the ImageSequence
   */
  bool IsRunning() const ATLAS_NOEXCEPT;

  /**
   * Set the streaming mode to true or false.
   *
   * Synchronise the streaming flag ressource and set it to the passed argument
   *
   * \param streaming The flag to enable or disable the streaming mode.
   */
  void SetStreamingMode(bool streaming) ATLAS_NOEXCEPT;

  /**
   * Return either if the ImageSequenceProvider is in streaming mode
   *
   * \return True if in streaming mode, False else.
   */
  bool IsStreaming() const ATLAS_NOEXCEPT;

 protected:
  //============================================================================
  // P R O T E C T E D   M E T H O D S

  virtual void WriteImage(const cv::Mat &image) = 0;

 private:
  //============================================================================
  // P R I V A T E   M E T H O D S

  /**
   * The thread function that is going to notify all the observer of this
   * Image Provider if we are in streaming mode.
   *
   * This is going to wait for the running_ flag to be true, and the will start.
   */
  virtual void OnSubjectNotify(Subject<const cv::Mat &> &subject,
                               const cv::Mat &image) ATLAS_NOEXCEPT override;

  //============================================================================
  // P R I V A T E   M E M B E R S

  uint64_t frame_count_;

  bool streaming_;

  bool running_;
};

}  // namespace sonia_common

#include <sonia_common/io/image_sequence_writer_inl.h>

#endif  // SONIA_COMMON_IO_IMAGE_SEQUENCE_WRITER_H_
