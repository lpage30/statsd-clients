// a single-threaded C++ statsd client
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include "statsd-client.hpp"

namespace statsd {
    StatsDClient::StatsDClient(std::unique_ptr<StatsDConnection> connection) : m_pConnection(connection)
    {}
    StatsDClient::~StatsDClient()
    {
        m_pConnection->close();
    }

    int StatsDClient::config(const string& host, const short port, const string& ns)
    {
        return m_pConnection->open(host, port, ns);
    }
    string StatsDClient::getLastError(bool clearError)
    {
        return m_pConnection->getLastError(clearError);
    }
    int StatsDClient::dec(const char* key, float sample_rate)
    {
        return count(key, -1, sample_rate);
    }
    
    int StatsDClient::inc(const char* key, float sample_rate)
    {
        return count(key, 1, sample_rate);
    }
    
    int StatsDClient::count(const char* key, size_t value, float sample_rate)
    {
        return m_pConnection->send(key, value, "c", sample_rate);
    }
    
    int StatsDClient::gauge(const char* key, size_t value, float sample_rate)
    {
        return m_pConnection->send(key, value, "g", sample_rate);
    }
    
    int StatsDClient::timing(const char* key, size_t ms, float sample_rate)
    {
        return m_pConnection->send(key, ms, "ms", sample_rate);
    }
};