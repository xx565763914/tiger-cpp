#pragma once

#include <time.h>

long long getCurSecs() {
    return time((time_t*)NULL);
}

