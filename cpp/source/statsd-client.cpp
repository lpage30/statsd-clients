// a single-threaded C++ statsd client
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include "statsd-client.hpp"
#include "single-threaded-statsd-connection.hpp"

namespace statsd {
    StatsDClient::StatsDClient(StatsDConnection* pconnection) : m_pConnection(pconnection)
    {}
    StatsDClient::~StatsDClient()
    {
        m_pConnection->close();
    }
    
    int StatsDClient::config(
        const string& host,
        const short port,
        const string& ns,
        const vector<pair<string, string>>& tags
    )
    {
        return m_pConnection->open(host, port, ns, tags);
    }
    string StatsDClient::getLastError(bool clearError)
    {
        return m_pConnection->getLastError(clearError);
    }
    int StatsDClient::count(
        const char* key,
        size_t value,
        const vector<pair<string, string>>& tags
    )
    {
        return m_pConnection->send(key, value, "c", tags);
    }
    int StatsDClient::histogram(
        const char* key,
        size_t value,
        const vector<pair<string, string>>& tags
    )
    {
        return m_pConnection->send(key, value, "h", tags);
    }
    int StatsDClient::timing(
        const char* key,
        size_t ms,
        const vector<pair<string, string>>& tags
    )
    {
        return m_pConnection->send(key, ms, "ms", tags);
    }

    StatsDClient& StatsDClient::getInstance()
    {
        static StatsDClient instance(new SingleThreadedStatsDConnection());
        return instance;
    }

};