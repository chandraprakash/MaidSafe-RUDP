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

#ifndef MAIDSAFE_RUDP_PARAMETERS_H_
#define MAIDSAFE_RUDP_PARAMETERS_H_

#include <cstdint>
#include "boost/date_time/posix_time/posix_time_duration.hpp"


namespace maidsafe {

namespace rudp {

typedef boost::posix_time::time_duration Timeout;

// This class provides the configurability to all traffic related parameters.
struct Parameters {
 public:
  // Thread count for use of asio::io_service.
  static uint32_t thread_count;

  // Maximum number of Transports per ManagedConnections object
  static int max_transports;

  // Window size permitted in RUDP.
  static uint32_t default_window_size;
  static uint32_t maximum_window_size;

  // Packet size permitted in RUDP.  Shall not exceed the UDP payload, which is 65507.
  static uint32_t default_size;
  static uint32_t max_size;
  enum { kUDPPayload = 65507 };

  // Data Payload size permitted in RUDP.  Shall not exceed Packet Size defined.
  static uint32_t max_data_size;
  static uint32_t default_data_size;

  // Timeout defined for a packet to be resent.
  static Timeout default_send_timeout;

  // Timeout defined for a neg-ack packet to be resent to request resent of an observed missing
  // packet in receiver.
  static Timeout default_receive_timeout;

  // Machine dependent parameter of send delay, depending on computation power and I/O speed.
  static Timeout default_send_delay;

  // Machine dependent parameter of receive delay, depending on computation power and I/O speed.
  static Timeout default_receive_delay;

  // Timeout defined for a Ack packet to be resent.
  static Timeout default_ack_timeout;

  // Timeout defined for the fixed interval between Ack packets.
  static Timeout ack_interval;

  // Interval to calculate speed.
  static Timeout speed_calculate_inverval;

  // Slow speed threshold to force the socket closed, in bits/s.
  static uint32_t slow_speed_threshold;

  // Timeout during normal peer-to-peer connection establishment.
  static Timeout rendezvous_connect_timeout;

  // Timeout during connection establishment while bootstrapping a new transport.
  static Timeout bootstrap_connect_timeout;

  // Timeout during ping attempt.
  static Timeout ping_timeout;

  // Timeout defined for the fixed interval between sending Keepalive packets.
  static Timeout keepalive_interval;

  // Timeout defined to receive Keepalive response packet.
  static Timeout keepalive_timeout;

  // Maximum sequential keepalive failures allowed before connection is closed.
  static uint32_t maximum_keepalive_failures;

  // Maximum length of time for Bootstrapping connection to exist.
  static Timeout bootstrap_connection_lifespan;

  // Timeout defined for allowing flushing pending data after Connection::Close is called.
  static Timeout disconnection_timeout;

  // Defined connection types.
  enum ConnectionType {
    kWireless = 0x0fffffff,
    kT1 = 0xf0ffffff,
    kE1 = 0xf1ffffff,
    k10MEthernet = 0xff0fffff,
    k100MEthernet = 0xff1fffff,
    k1GEthernet = 0xff2fffff
  };
  static ConnectionType connection_type;

#ifdef TESTING
  // Rudp Encryption switch
  static bool rudp_encrypt;
#endif

 private:
  // Disallow copying and assignment.
  Parameters(const Parameters&);
  Parameters& operator=(const Parameters&);
};

}  // namespace rudp

}  // namespace maidsafe

#endif  // MAIDSAFE_RUDP_PARAMETERS_H_
