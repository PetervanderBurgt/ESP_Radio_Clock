#ifndef NTP_SERVER_H
#define NTP_SERVER_H
#include <time.h>

class NtpServer {
public:
    void setup();
    tm get_time();
};

#endif