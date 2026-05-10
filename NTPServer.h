#ifndef NTP_SERVER_H
#define NTP_SERVER_H
#include <time.h>

class NtpServer {
public:
    void config_ntp();
    tm get_time();
};

#endif