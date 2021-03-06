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

#include <vector>

#include "maidsafe/common/log.h"
#include "maidsafe/common/test.h"
#include "maidsafe/common/utils.h"

#include "maidsafe/rudp/return_codes.h"
#include "maidsafe/rudp/transport.h"
#include "maidsafe/rudp/tests/test_utils.h"
#include "maidsafe/rudp/utils.h"

namespace asio = boost::asio;
namespace bptime = boost::posix_time;

namespace maidsafe {

namespace rudp {

typedef asio::ip::udp::endpoint Endpoint;

namespace detail {

namespace test {

// class RudpTransportTest : public testing::Test {
// public:
//  RudpTransportTest()
//      : transports_(),
//        network_size_(2),
//        kTimeOut_(Parameters::keepalive_interval *
//                  (Parameters::maximum_keepalive_failures + 1)) {}
//
//  ~RudpTransportTest() {}
//
// protected:
//  struct TestPeer {
//    TestPeer() : local_endpoint(GetLocalIp(), maidsafe::test::GetRandomPort()),
//                 key_pair(),
//                 mutex(),
//                 cond_var_connection_added(),
//                 cond_var_connection_lost(),
//                 cond_var_msg_received(),
//                 asio_service(Parameters::thread_count),
//                 nat_type(NatType::kUnknown),
//                 transport(),
//                 messages_received(),
//                 peers_added(),
//                 peers_lost() {
//      asymm::GenerateKeyPair(&key_pair);
//      transport.reset(new Transport(asio_service, nat_type));
//      Endpoint chosen_endpoint;
//      std::vector<Endpoint> bootstrap_endpoints;
//      boost::signals2::connection on_message_connection;
//      boost::signals2::connection on_connection_added_connection;
//      boost::signals2::connection on_connection_lost_connection;
//      transport->Bootstrap(
//          bootstrap_endpoints,
//          std::shared_ptr<asymm::PublicKey>(new asymm::PublicKey(key_pair.public_key)),
//          local_endpoint,
//          true,
//          boost::bind(&TestPeer::OnMessageSlot, this, _1),
//          boost::bind(&TestPeer::OnConnectionAddedSlot, this, _1, _2),
//          boost::bind(&TestPeer::OnConnectionLostSlot, this, _1, _2, _3, _4),
//          boost::bind(&TestPeer::OnNatDetectionRequestedSlot, this, _1, _2, _3),
//          &chosen_endpoint,
//          &on_message_connection,
//          &on_connection_added_connection,
//          &on_connection_lost_connection);
//      asio_service.Start();
//    }
//
//    ~TestPeer() {
//       transport->Close();
//       asio_service.Stop();
//    }
//
//    void OnMessageSlot(const std::string& message) {
//      boost::mutex::scoped_lock lock(mutex);
//      messages_received.push_back(message);
//      cond_var_msg_received.notify_one();
//    }
//
//    void OnConnectionAddedSlot(const Endpoint& peer_endpoint, TransportPtr /*transport*/) {
//      boost::mutex::scoped_lock lock(mutex);
//      peers_added.push_back(peer_endpoint);
//      cond_var_connection_added.notify_one();
//    }
//
//    void OnConnectionLostSlot(const Endpoint& peer_endpoint,
//                              TransportPtr /*transport*/,
//                              bool /*connections_empty*/,
//                              bool /*temporary_connection*/) {
//      boost::mutex::scoped_lock lock(mutex);
//      peers_lost.push_back(peer_endpoint);
//      cond_var_connection_lost.notify_one();
//    }
//
//    void OnNatDetectionRequestedSlot(const Endpoint& /*this_local_endpoint*/,
//                                     const asio::ip::udp::endpoint& /*peer_endpoint*/,
//                                     uint16_t& /*another_external_port*/) {
//    }
//
//    Endpoint local_endpoint;
//    asymm::Keys key_pair;
//    boost::mutex mutex;
//    boost::condition_variable cond_var_connection_added;
//    boost::condition_variable cond_var_connection_lost;
//    boost::condition_variable cond_var_msg_received;
//    AsioService asio_service;
//    NatType nat_type;
//    std::shared_ptr<Transport> transport;
//    std::vector<std::string> messages_received;
//    std::vector<Endpoint> peers_added;
//    std::vector<Endpoint> peers_lost;
//  };
//
//  void SetUp() {
//    for (int i(0); i < network_size_; ++i) {
//      std::shared_ptr<TestPeer> test_peer(new TestPeer());
//      transports_.push_back(test_peer);
//    }
//  }
//
//  void TearDown() {
//    transports_.clear();
//  }
//
//  void ConnectTestPeers() {
//    transports_[0]->transport->Connect(transports_[1]->local_endpoint,
//                                       "validation data from node 0");
//    transports_[1]->transport->Connect(transports_[0]->local_endpoint,
//                                       "validation data from node 1");
//    boost::mutex::scoped_lock lock(transports_[1]->mutex);
//    EXPECT_TRUE(transports_[1]->cond_var_msg_received.timed_wait(lock, kTimeOut_));
//  }
//
//  std::vector<std::shared_ptr<TestPeer> > transports_;
//  uint16_t network_size_;
//  bptime::time_duration kTimeOut_;
// };
//
// TEST_F(RudpTransportTest, BEH_Connection) {
//  ConnectTestPeers();
//
//  transports_[1]->messages_received.clear();
//  std::string msg_content(RandomString(256));
//
//  int send_result(kGeneralError);
//  bool message_sent(false);
//  boost::mutex send_mutex;
//  boost::condition_variable send_cond_var;
//  boost::mutex::scoped_lock send_lock(send_mutex);
//  auto message_sent_functor([&](int result_in) {
//    boost::mutex::scoped_lock lock(send_mutex);
//    send_result = result_in;
//    message_sent = true;
//    send_cond_var.notify_one();
//  });
//
//  boost::mutex::scoped_lock lock(transports_[1]->mutex);
//  transports_[0]->transport->Send(transports_[1]->local_endpoint, msg_content,
//                                  message_sent_functor);  // NOLINT (Fraser)
//  EXPECT_TRUE(transports_[1]->cond_var_msg_received.timed_wait(lock, kTimeOut_));
//  EXPECT_TRUE(transports_[0]->cond_var_msg_received.timed_wait(
//      send_lock, kTimeOut_, [&message_sent]() { return message_sent; }));  // NOLINT (Fraser)
//  EXPECT_EQ(kSuccess, send_result);
//  ASSERT_EQ(1U, transports_[1]->messages_received.size());
//  EXPECT_NE(msg_content, transports_[1]->messages_received[0]);
//  std::string decrypted_msg;
//  EXPECT_EQ(kSuccess, asymm::Decrypt(transports_[1]->messages_received[0],
//                                     transports_[1]->key_pair.private_key,
//                                     &decrypted_msg));
//  EXPECT_EQ(msg_content, decrypted_msg);
// }
//
// TEST_F(RudpTransportTest, BEH_CloseConnection) {
//  ConnectTestPeers();
//  transports_[1]->transport->CloseConnection(transports_[0]->local_endpoint);
//  {
//    boost::mutex::scoped_lock lock(transports_[0]->mutex);
//    EXPECT_TRUE(transports_[0]->cond_var_connection_lost.timed_wait(lock, kTimeOut_));
//  }
//  EXPECT_EQ(1U, transports_[0]->peers_lost.size());
//  EXPECT_EQ(transports_[1]->local_endpoint, transports_[0]->peers_lost[0]);
//
//  int send_result(kSuccess);
//  transports_[1]->messages_received.clear();
//  std::string msg_content("testing msg from node 0");
//  transports_[0]->transport->Send(transports_[1]->local_endpoint, msg_content,
//                                  [&](int result) { send_result = result; });  // NOLINT (Fraser)
//  boost::mutex::scoped_lock lock(transports_[1]->mutex);
//  EXPECT_FALSE(transports_[1]->cond_var_msg_received.timed_wait(lock, kTimeOut_));
//  EXPECT_EQ(kInvalidConnection, send_result);
//  EXPECT_EQ(0, transports_[1]->messages_received.size());
// }
//
// TEST_F(RudpTransportTest, BEH_DropConnection) {
//  ConnectTestPeers();
//  Endpoint dropped_endpoint(transports_[0]->local_endpoint);
//  int attempts(0);
//  while ((attempts < 10) && (transports_[0]->messages_received.size() == 0)) {
//    ++attempts;
//    Sleep(bptime::milliseconds(100));
//  }
//  transports_.erase(transports_.begin());
//
//  boost::mutex::scoped_lock lock(transports_[0]->mutex);
//  EXPECT_TRUE(transports_[0]->cond_var_connection_lost.timed_wait(lock, kTimeOut_));
//
//  ASSERT_EQ(1U, transports_[0]->peers_lost.size());
//  EXPECT_EQ(dropped_endpoint, transports_[0]->peers_lost[0]);
//
//  int send_result(kSuccess);
//  transports_[0]->messages_received.clear();
//  std::string msg_content("testing msg from node 0");
//  transports_[0]->transport->Send(dropped_endpoint, msg_content,
//                                  [&](int result) { send_result = result; });  // NOLINT (Fraser)
//  EXPECT_FALSE(transports_[0]->cond_var_msg_received.timed_wait(lock, kTimeOut_));
//  EXPECT_EQ(kInvalidConnection, send_result);
//  EXPECT_EQ(0, transports_[0]->messages_received.size());
// }

}  // namespace test

}  // namespace detail

}  // namespace rudp

}  // namespace maidsafe
