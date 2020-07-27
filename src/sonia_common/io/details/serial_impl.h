/**
 * \file serial_impl.h
 * \author  William Woodall <wjwwood@gmail.com>
 * \author  John Harrison <ash@greaterthaninfinity.com>
 * \version 0.1
 *
 * \section LICENSE
 *
 * The MIT License
 *
 * Copyright (c) 2012 William Woodall, John Harrison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * \section DESCRIPTION
 *
 * This provides a unix based pimpl for the Serial class. This implementation is
 * based off termios.h and uses select for multiplexing the IO ports.
 *
 */

#ifndef SONIA_COMMON_IO_DETAILS_SERIAL_IMPL_H_
#define SONIA_COMMON_IO_DETAILS_SERIAL_IMPL_H_

#include <sonia_common/exceptions.h>
#include <pthread.h>
#include <memory>

namespace sonia_common {

class Serial;

class Serial::SerialImpl {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<Serial::SerialImpl>;

  //============================================================================
  // P U B L I C   C / D T O R S

  SerialImpl(const std::string &port, unsigned long baudrate,
             bytesize_t bytesize, parity_t parity, stopbits_t stopbits,
             flowcontrol_t flowcontrol);

  virtual ~SerialImpl();

  //============================================================================
  // P U B L I C   M E T H O D S

  void Open();

  void Close();

  bool IsOpen() const;

  size_t Available();

  bool WaitReadable(uint32_t timeout);

  void WaitByteTimes(size_t count);

  size_t Read(uint8_t *buf, size_t size = 1);

  size_t Write(const uint8_t *data, size_t length);

  void Flush();

  void FlushInput();

  void FlushOutput();

  void SendBreak(int duration);

  void SetBreak(bool level);

  void SetRTS(bool level);

  void SetDTR(bool level);

  bool WaitForChange();

  bool GetCTS();

  bool GetDSR();

  bool GetRI();

  bool GetCD();

  void SetPort(const std::string &port);

  std::string GetPort() const;

  void SetTimeout(const Timeout &timeout);

  Timeout GetTimeout() const;

  void SetBaudrate(unsigned long baudrate);

  unsigned long GetBaudrate() const;

  void SetBytesize(bytesize_t bytesize);

  bytesize_t GetBytesize() const;

  void SetParity(parity_t parity);

  parity_t GetParity() const;

  void SetStopbits(stopbits_t stopbits);

  stopbits_t GetStopbits() const;

  void SetFlowcontrol(flowcontrol_t flowcontrol);

  flowcontrol_t GetFlowcontrol() const;

  void ReadLock();

  void ReadUnlock();

  void WriteLock();

  void WriteUnlock();

 protected:
  //============================================================================
  // P R O T E C T E D   M E T H O D S

  void ReconfigurePort();

 private:
  //============================================================================
  // P R I V A T E   M E M B E R S

  std::string port_;  // Path to the file descriptor
  int fd_;            // The current file descriptor

  bool is_open_;
  bool xonxoff_;
  bool rtscts_;

  Timeout timeout_;         // Timeout for read operations
  unsigned long baudrate_;  // Baudrate
  uint32_t byte_time_ns_;   // Nanoseconds to transmit/receive a single byte

  parity_t parity_;            // Parity
  bytesize_t bytesize_;        // Size of the bytes
  stopbits_t stopbits_;        // Stop Bits
  flowcontrol_t flowcontrol_;  // Flow Control

  // TODO: Use the mutex from mutex.h
  // Mutex used to lock the read functions
  pthread_mutex_t read_mutex;
  // Mutex used to lock the write functions
  pthread_mutex_t write_mutex;
};

}  // namespace sonia_common

#include <sonia_common/io/details/serial_impl_inl.h>

#endif  // SONIA_COMMON_IO_DETAILS_SERIAL_IMPL_H_
