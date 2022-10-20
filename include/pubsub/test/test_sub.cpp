#include "pubsub/redis_sub.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#define MSG_CNT 10'000

int main(int argc, char *args[]) {

  redisSubscribe redis_sub(std::string("127.0.0.1"), 6379);

  const std::vector<std::string> chns{
      "test_channel_000",
      "channel_2222_test",
      "test-6789876-channel",
  };

  std::thread sub_recv([&redis_sub, &chns] { redis_sub.run(chns); });
  sub_recv.detach();

  for (int i = 0; i < MSG_CNT; ++i) {
    std::cout << redis_sub.get_channel_message() << std::endl;
  }

  redis_sub.stop();
  return 0;
};