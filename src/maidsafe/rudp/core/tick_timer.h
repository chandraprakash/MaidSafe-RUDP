/* Copyright 2012 MaidSafe.net limited

This MaidSafe Software is licensed under the MaidSafe.net Commercial License, version 1.0 or later,
and The General Public License (GPL), version 3. By contributing code to this project You agree to
the terms laid out in the MaidSafe Contributor Agreement, version 1.0, found in the root directory
of this project at LICENSE, COPYING and CONTRIBUTOR respectively and also available at:

http://www.novinet.com/license

Unless required by applicable law or agreed to in writing, software distributed under the License is
distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied. See the License for the specific language governing permissions and limitations under the
License.
*/

// Original author: Christopher M. Kohlhoff (chris at kohlhoff dot com)

#ifndef MAIDSAFE_RUDP_CORE_TICK_TIMER_H_
#define MAIDSAFE_RUDP_CORE_TICK_TIMER_H_

#include "boost/asio/deadline_timer.hpp"

namespace maidsafe {

namespace rudp {

namespace detail {

// Lightweight wrapper around a deadline_timer that avoids modifying the expiry time if it would
// move it further away.
class TickTimer {
 public:
  explicit TickTimer(boost::asio::io_service& asio_service)
      : timer_(asio_service) {
    Reset();
  }

  static boost::posix_time::ptime Now() { return boost::asio::deadline_timer::traits_type::now(); }

  void Cancel() { timer_.cancel(); }

  void Reset() { timer_.expires_at(boost::posix_time::pos_infin); }

  bool Expired() const {
    // Infinite time out will be counted as expired
    if (timer_.expires_at() == boost::posix_time::pos_infin)
      return true;
    return Now() >= timer_.expires_at();
  }

  void TickAt(const boost::posix_time::ptime& time) {
    if (time < timer_.expires_at())
      timer_.expires_at(time);
  }

  void TickAfter(const boost::posix_time::time_duration& duration) { TickAt(Now() + duration); }

  template <typename WaitHandler>
  void AsyncWait(WaitHandler handler) { timer_.async_wait(handler); }

 private:
  boost::asio::deadline_timer timer_;
};

}  // namespace detail

}  // namespace rudp

}  // namespace maidsafe

#endif  // MAIDSAFE_RUDP_CORE_TICK_TIMER_H_
