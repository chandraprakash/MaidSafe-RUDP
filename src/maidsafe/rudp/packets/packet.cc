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

#include "maidsafe/rudp/packets/packet.h"

namespace asio = boost::asio;

namespace maidsafe {

namespace rudp {

namespace detail {

Packet::~Packet() {}

bool Packet::DecodeDestinationSocketId(uint32_t* id, const asio::const_buffer& data) {
  // Refuse to decode anything that's too short.
  if (asio::buffer_size(data) < 16)
    return false;

  DecodeUint32(id, asio::buffer_cast<const unsigned char*>(data) + 12);
  return true;
}

void Packet::DecodeUint32(uint32_t* n, const unsigned char* p) {
  *n = p[0];
  *n = ((*n << 8) | p[1]);
  *n = ((*n << 8) | p[2]);
  *n = ((*n << 8) | p[3]);
}

void Packet::EncodeUint32(uint32_t n, unsigned char* p) {
  p[0] = ((n >> 24) & 0xff);
  p[1] = ((n >> 16) & 0xff);
  p[2] = ((n >> 8) & 0xff);
  p[3] = (n & 0xff);
}

}  // namespace detail

}  // namespace rudp

}  // namespace maidsafe
