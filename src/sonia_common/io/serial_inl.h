/**
 * \file serial_inl.h
 * \author  William Woodall <wjwwood@gmail.com>
 * \author  John Harrison   <ash.gti@gmail.com>
 * \version 0.1
 *
 * \section LICENSE
 *
 * The MIT License
 *
 * Copyright (c) 2012 William Woodall
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
 */

#ifndef SONIA_COMMON_IO_SERIAL_H_
#error This file may only be included from serial.h
#endif

#include <sonia_common/io/details/serial_impl.h>
#include <algorithm>

#if !defined(__OpenBSD__)
#include <alloca.h>
#endif

namespace sonia_common {

//==============================================================================
// I N N E R   C L A S S   S E C T I O N

//------------------------------------------------------------------------------
//
class Serial::ScopedReadLock {
 public:
  ScopedReadLock(SerialImpl *pimpl) : pimpl_(pimpl) { pimpl_->ReadLock(); }
  ~ScopedReadLock() { pimpl_->ReadUnlock(); }

 private:
  // Disable copy constructors
  ScopedReadLock(const ScopedReadLock &) = delete;
  const ScopedReadLock &operator=(ScopedReadLock) = delete;

  SerialImpl *pimpl_;
};

//------------------------------------------------------------------------------
//
class Serial::ScopedWriteLock {
 public:
  ScopedWriteLock(SerialImpl *pimpl) : pimpl_(pimpl) { pimpl_->WriteLock(); }
  ~ScopedWriteLock() { pimpl_->WriteUnlock(); }

 private:
  // Disable copy constructors
  ScopedWriteLock(const ScopedWriteLock &) = delete;
  const ScopedWriteLock &operator=(ScopedWriteLock) = delete;
  SerialImpl *pimpl_;
};

//==============================================================================
// C / D T O R S   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_INLINE Serial::Serial(const std::string &port, uint32_t baudrate,
                            Timeout timeout, bytesize_t bytesize,
                            parity_t parity, stopbits_t stopbits,
                            flowcontrol_t flowcontrol)
    : pimpl_(new SerialImpl(port, baudrate, bytesize, parity, stopbits,
                            flowcontrol)) {
  pimpl_->SetTimeout(timeout);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Serial::~Serial() { delete pimpl_; }

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::Open() { pimpl_->Open(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::Close() { pimpl_->Close(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE bool Serial::IsOpen() const { return pimpl_->IsOpen(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::Available() { return pimpl_->Available(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE bool Serial::WaitReadable() {
  Timeout timeout(pimpl_->GetTimeout());
  return pimpl_->WaitReadable(timeout.read_timeout_constant);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::WaitByteTimes(size_t count) {
  pimpl_->WaitByteTimes(count);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::read_(uint8_t *buffer, size_t size) {
  return pimpl_->Read(buffer, size);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::Read(uint8_t *buffer, size_t size) {
  ScopedReadLock lock(pimpl_);
  return pimpl_->Read(buffer, size);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::Read(std::vector<uint8_t> &buffer, size_t size) {
  ScopedReadLock lock(pimpl_);
  uint8_t *buffer_ = new uint8_t[size];
  size_t bytes_read = pimpl_->Read(buffer_, size);
  buffer.insert(buffer.end(), buffer_, buffer_ + bytes_read);
  delete[] buffer_;
  return bytes_read;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::Read(std::string &buffer, size_t size) {
  ScopedReadLock lock(pimpl_);
  uint8_t *buffer_ = new uint8_t[size];
  size_t bytes_read = pimpl_->Read(buffer_, size);
  buffer.append(reinterpret_cast<const char *>(buffer_), bytes_read);
  delete[] buffer_;
  return bytes_read;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE std::string Serial::Read(size_t size) {
  std::string buffer;
  Read(buffer, size);
  return buffer;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::ReadLine(std::string &buffer, size_t size,
                                     std::string eol) {
  ScopedReadLock lock(pimpl_);
  size_t eol_len = eol.length();
  uint8_t *buffer_ = static_cast<uint8_t *>(alloca(size * sizeof(uint8_t)));
  size_t read_so_far = 0;
  while (true) {
    size_t bytes_read = read_(buffer_ + read_so_far, 1);
    read_so_far += bytes_read;
    if (bytes_read == 0) {
      break;  // Timeout occured on reading 1 byte
    }
    if (std::string(
            reinterpret_cast<const char *>(buffer_ + read_so_far - eol_len),
            eol_len) == eol) {
      break;  // EOL found
    }
    if (read_so_far == size) {
      break;  // Reached the maximum read length
    }
  }
  buffer.append(reinterpret_cast<const char *>(buffer_), read_so_far);
  return read_so_far;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE std::string Serial::ReadLine(size_t size, std::string eol) {
  std::string buffer;
  ReadLine(buffer, size, eol);
  return buffer;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE std::vector<std::string> Serial::ReadLines(size_t size,
                                                        std::string eol) {
  ScopedReadLock lock(pimpl_);
  std::vector<std::string> lines;
  size_t eol_len = eol.length();
  uint8_t *buffer_ = static_cast<uint8_t *>(alloca(size * sizeof(uint8_t)));
  size_t read_so_far = 0;
  size_t start_of_line = 0;
  while (read_so_far < size) {
    size_t bytes_read = read_(buffer_ + read_so_far, 1);
    read_so_far += bytes_read;
    if (bytes_read == 0) {
      if (start_of_line != read_so_far) {
        lines.push_back(
            std::string(reinterpret_cast<const char *>(buffer_ + start_of_line),
                        read_so_far - start_of_line));
      }
      break;  // Timeout occured on reading 1 byte
    }
    if (std::string(
            reinterpret_cast<const char *>(buffer_ + read_so_far - eol_len),
            eol_len) == eol) {
      // EOL found
      lines.push_back(
          std::string(reinterpret_cast<const char *>(buffer_ + start_of_line),
                      read_so_far - start_of_line));
      start_of_line = read_so_far;
    }
    if (read_so_far == size) {
      if (start_of_line != read_so_far) {
        lines.push_back(
            std::string(reinterpret_cast<const char *>(buffer_ + start_of_line),
                        read_so_far - start_of_line));
      }
      break;  // Reached the maximum read length
    }
  }
  return lines;
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::Write(const std::string &data) {
  ScopedWriteLock lock(pimpl_);
  return write_(reinterpret_cast<const uint8_t *>(data.c_str()), data.length());
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::Write(const std::vector<uint8_t> &data) {
  ScopedWriteLock lock(pimpl_);
  return write_(&data[0], data.size());
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::Write(const uint8_t *data, size_t size) {
  ScopedWriteLock lock(pimpl_);
  return write_(data, size);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE size_t Serial::write_(const uint8_t *data, size_t length) {
  return pimpl_->Write(data, length);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetPort(const std::string &port) {
  ScopedReadLock rlock(pimpl_);
  ScopedWriteLock wlock(pimpl_);
  bool was_open = pimpl_->IsOpen();
  if (was_open) {
    Close();
  }
  pimpl_->SetPort(port);
  if (was_open) {
    Open();
  }
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE std::string Serial::GetPort() const { return pimpl_->GetPort(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetTimeout(const Timeout &timeout) {
  pimpl_->SetTimeout(timeout);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE Timeout Serial::GetTimeout() const { return pimpl_->GetTimeout(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetBaudrate(uint32_t baudrate) {
  pimpl_->SetBaudrate(baudrate);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE uint32_t Serial::GetBaudrate() const {
  return uint32_t(pimpl_->GetBaudrate());
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetBytesize(bytesize_t bytesize) {
  pimpl_->SetBytesize(bytesize);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE bytesize_t Serial::GetBytesize() const {
  return pimpl_->GetBytesize();
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetParity(parity_t parity) {
  pimpl_->SetParity(parity);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE parity_t Serial::GetParity() const { return pimpl_->GetParity(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetStopbits(stopbits_t stopbits) {
  pimpl_->SetStopbits(stopbits);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE stopbits_t Serial::GetStopbits() const {
  return pimpl_->GetStopbits();
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetFlowcontrol(flowcontrol_t flowcontrol) {
  pimpl_->SetFlowcontrol(flowcontrol);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE flowcontrol_t Serial::GetFlowcontrol() const {
  return pimpl_->GetFlowcontrol();
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::Flush() {
  ScopedReadLock rlock(pimpl_);
  ScopedWriteLock wlock(pimpl_);
  pimpl_->Flush();
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::FlushInput() {
  ScopedReadLock lock(pimpl_);
  pimpl_->FlushInput();
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::FlushOutput() {
  ScopedWriteLock lock(pimpl_);
  pimpl_->FlushOutput();
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SendBreak(int duration) {
  pimpl_->SendBreak(duration);
}

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetBreak(bool level) { pimpl_->SetBreak(level); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetRTS(bool level) { pimpl_->SetRTS(level); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE void Serial::SetDTR(bool level) { pimpl_->SetDTR(level); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE bool Serial::WaitForChange() { return pimpl_->WaitForChange(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE bool Serial::GetCTS() { return pimpl_->GetCTS(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE bool Serial::GetDSR() { return pimpl_->GetDSR(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE bool Serial::GetRI() { return pimpl_->GetRI(); }

//------------------------------------------------------------------------------
//
ATLAS_INLINE bool Serial::GetCD() { return pimpl_->GetCD(); }

}  // namespace sonia_common
