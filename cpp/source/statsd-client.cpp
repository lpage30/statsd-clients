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
        close();
    }
    
    bool StatsDClient::open(
        const string& host,
        const short port,
        const string& ns,
        const vector<pair<string, string>>& tags
    )
    {
        return m_pConnection->open(host, port, ns, tags);
    }

    bool StatsDClient::isOpen() const
    {
        return m_pConnection->isOpen();
    }

    void StatsDClient::close()
    {
        m_pConnection->close();
    }

    bool StatsDClient::hasError() const
    {
        return m_pConnection->hasError();
    }

    string StatsDClient::getLastError(bool clearError)
    {
        return m_pConnection->getLastError(clearError);
    }

    bool StatsDClient::count(
        const char* key,
        size_t value,
        const vector<pair<string, string>>& tags
    )
    {
        return m_pConnection->send(key, value, "c", tags);
    }

    bool StatsDClient::histogram(
        const char* key,
        size_t value,
        const vector<pair<string, string>>& tags
    )
    {
        return m_pConnection->send(key, value, "h", tags);
    }

    bool StatsDClient::timing(
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