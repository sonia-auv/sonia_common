/**
 * \file serial_test.cc
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
 *
 * \section DESCRIPTION
 *
 * This provides a cross platform interface for interacting with Serial Ports.
 */

#include <string>
#include "gtest/gtest.h"
#include <boost/bind.hpp>
#include <sonia_common/io/serial.h>

#if defined(OS_LINUX)
#include <pty.h>
#else
#include <util.h>
#endif

using namespace sonia_common;

namespace {

class SerialTests : public ::testing::Test {
protected:
  virtual void SetUp() {
    if (openpty(&master_fd, &slave_fd, name, NULL, NULL) == -1) {
      perror("openpty");
      exit(127);
    }

    ASSERT_TRUE(master_fd > 0);
    ASSERT_TRUE(slave_fd > 0);
    ASSERT_TRUE(std::string(name).length() > 0);

    port1 = new Serial(std::string(name), 115200, Timeout::SimpleTimeout(250));
  }

  virtual void TearDown() {
    port1->Close();
    delete port1;
  }

  Serial * port1;
  int master_fd;
  int slave_fd;
  char name[100];
};

TEST_F(SerialTests, readWorks) {
  write(master_fd, "abc\n", 4);
  std::string r = port1->Read(4);
  EXPECT_EQ(r, std::string("abc\n"));
}

TEST_F(SerialTests, writeWorks) {
  char buf[5] = "";
  port1->Write("abc\n");
  read(master_fd, buf, 4);
  EXPECT_EQ(std::string(buf, 4), std::string("abc\n"));
}

TEST_F(SerialTests, timeoutWorks) {
  // Timeout a read, returns an empty string
  std::string empty = port1->Read();
  EXPECT_EQ(empty, std::string(""));
  
  // Ensure that writing/reading still works after a timeout.
  write(master_fd, "abc\n", 4);
  std::string r = port1->Read(4);
  EXPECT_EQ(r, std::string("abc\n"));
}

TEST_F(SerialTests, partialRead) {
  // Write some data, but request more than was written.
  write(master_fd, "abc\n", 4);

  // Should timeout, but return what was in the buffer.
  std::string empty = port1->Read(10);
  EXPECT_EQ(empty, std::string("abc\n"));
  
  // Ensure that writing/reading still works after a timeout.
  write(master_fd, "abc\n", 4);
  std::string r = port1->Read(4);
  EXPECT_EQ(r, std::string("abc\n"));
}

}  // namespace

int main(int argc, char **argv) {
  try {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  } catch (std::exception &e) {
    std::cerr << "Unhandled Exception: " << e.what() << std::endl;
  }
  return 1;
}
