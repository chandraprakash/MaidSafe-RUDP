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

#ifndef MAIDSAFE_RUDP_OPERATIONS_PROBE_OP_H_
#define MAIDSAFE_RUDP_OPERATIONS_PROBE_OP_H_

#include "boost/asio/handler_alloc_hook.hpp"
#include "boost/asio/handler_invoke_hook.hpp"
#include "boost/system/error_code.hpp"

namespace maidsafe {

namespace rudp {

namespace detail {

// Helper class to adapt a probe handler into a waiting operation.
template <typename ProbeHandler>
class ProbeOp {
 public:
  ProbeOp(ProbeHandler handler, const boost::system::error_code& ec)
      : handler_(handler),
        ec_(ec) {}

  ProbeOp(const ProbeOp& other) : handler_(other.handler_), ec_(other.ec_) {}

  void operator()(boost::system::error_code ec) {
    if (boost::asio::error::timed_out == ec)
      handler_(ec);
    else
      handler_(ec_);
  }

  friend void* asio_handler_allocate(size_t n, ProbeOp* op) {
    using boost::asio::asio_handler_allocate;
    return asio_handler_allocate(n, &op->handler_);
  }

  friend void asio_handler_deallocate(void* p, size_t n, ProbeOp* op) {
    using boost::asio::asio_handler_deallocate;
    asio_handler_deallocate(p, n, &op->handler_);
  }

  template <typename Function>
  friend void asio_handler_invoke(const Function& f, ProbeOp* op) {
    using boost::asio::asio_handler_invoke;
    asio_handler_invoke(f, &op->handler_);
  }

 private:
  // Disallow assignment.
  ProbeOp& operator=(const ProbeOp&);

  ProbeHandler handler_;
  const boost::system::error_code& ec_;
};

}  // namespace detail

}  // namespace rudp

}  // namespace maidsafe

#endif  // MAIDSAFE_RUDP_OPERATIONS_PROBE_OP_H_
