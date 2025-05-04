#include <stdio.h>
#include "single-threaded-statsd-connection.hpp"
#if !defined(_MSC_VER)
    #define _snprintf_s(a1,a2,a3,...) snprintf(a1,a2,__VA_ARGS__)
    #define _TRUNCATE ((size_t)-1)
#endif
namespace statsd {
    inline bool fequal(float a, float b)
    {
        const float epsilon = 0.0001;
        return ( fabs(a - b) < epsilon );
    }
    int StatsDConnection::send(const char* key, size_t value, const char* type, float sample_rate)
    {
        
        pair<char*, size_t> buf = getSendBuffer();
        int send_size = -1;
        if ( fequal( sample_rate, 1.0 ) )
        {
            send_size = _snprintf_s(buf.first, buf.second, "%s%s:%zd|%s",
                     ns.c_str(), key, value, type);
        }
        else
        {
            send_size = _snprintf_s(buf.first, buf.second, "%s%s:%zd|%s|@%.2f",
                     ns.c_str(), key, value, type, sample_rate);
        }
        if (send_size < 0)
        {
            send_size = strlen(buf.first);
        }
        return send(make_pair(buf.first, (size_t)send_size));

    }
    std::unique_ptr<StatsDConnection> CreateStatsDConnection()
    {
        std::unique_ptr<StatsDConnection> result(new SingleThreadedStatsDConnection());
    }
};