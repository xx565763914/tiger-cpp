#include "pubsub/redis_pub.hpp"
#include "pubsub/redis_sub.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#define MSG_CNT 10'321

int main(int argc, char *args[]) {

  redisPublish redis_pub("127.0.0.1", 6379);

  const std::vector<std::string> chns{
      "test_channel_000",
      "channel_2222_test",
      "test-6789876-channel",
  };

  for (int i = 0; i < MSG_CNT; ++i) {
    auto surfix{":::::" + std::to_string(i)};
    redis_pub.send(chns[0], chns[0] + surfix);
    redis_pub.send(chns[1], chns[1] + surfix);
    redis_pub.send(chns[2], chns[2] + surfix);
  }

  return 0;
};