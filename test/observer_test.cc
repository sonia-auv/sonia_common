/**
 * \file	observer_test.cc
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

#include "gtest/gtest.h"
#include <sonia_common/pattern/observer.h>
#include <sonia_common/pattern/subject.h>

class ConcreateObserver : public sonia_common::Observer<const std::string &, int> {
 public:
  int i_ = {0};
  std::string s_ = {""};

 protected:
  auto OnSubjectNotify(sonia_common::Subject<const std::string &, int> &subject,
                       const std::string &str,
                       int nb) ATLAS_NOEXCEPT -> void override {
    s_ = str;
    i_ = nb;
  }
};

class ConcreteSubject : public sonia_common::Subject<const std::string &, int> {
 public:
  void DoSomething(std::string s, int i) { Notify(s, i); }
};

TEST(Observer, attachToSubject) {
  ConcreteSubject subject = {};
  ConcreateObserver observer = {};
  ConcreateObserver observer_2 = {};

  observer.Observe(subject);
  ASSERT_EQ(subject.ObserverCount(), 1);
  ASSERT_TRUE(observer.IsAttached(subject));
  ASSERT_THROW(observer.Observe(subject), std::invalid_argument);

  subject.Attach(observer_2);
  ASSERT_EQ(subject.ObserverCount(), 2);
  ASSERT_TRUE(observer_2.IsAttached(subject));
  ASSERT_ANY_THROW(subject.Attach(observer_2));
}

TEST(Observer, detachFromSubject) {
  ConcreteSubject subject = {};
  ConcreateObserver observer = {};

  observer.Observe(subject);
  subject.Detach(observer);
  ASSERT_EQ(subject.ObserverCount(), 0);
  ASSERT_FALSE(observer.IsAttached(subject));

  observer.Observe(subject);
  observer.DetachFromAllSubject();
  ASSERT_EQ(subject.ObserverCount(), 0);
  ASSERT_FALSE(observer.IsAttached(subject));

  observer.Observe(subject);
  subject.DetachAll();
  ASSERT_EQ(subject.ObserverCount(), 0);
  ASSERT_FALSE(observer.IsAttached(subject));

  ConcreateObserver observer_2 = {};
  observer_2.Observe(subject);
  ASSERT_EQ(subject.ObserverCount(), 1);
}

TEST(Observer, isAttached) {
  ConcreteSubject subject = {};
  ConcreateObserver observer = {};

  observer.Observe(subject);
  ASSERT_TRUE(observer.IsAttached(subject));

  observer.DetachFromAllSubject();
  ASSERT_FALSE(observer.IsAttached(subject));
}

TEST(Observer, notify) {
  ConcreteSubject subject = {};
  ConcreateObserver observer = {};

  observer.Observe(subject);
  subject.DoSomething("Hello World !", 42);
  ASSERT_TRUE(observer.s_ == "Hello World !");
  ASSERT_TRUE(observer.i_ == 42);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}