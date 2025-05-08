#include <stdio.h>
#include "single-threaded-statsd-connection.hpp"
#if !defined(_MSC_VER)
    #define _snprintf_s(a1,a2,a3,...) snprintf(a1,a2,__VA_ARGS__)
    #define _TRUNCATE ((size_t)-1)
#endif
namespace statsd {

    void StatsDConnection::configure(
        const string& host,
        const short port,
        const string& ns,
        const vector<pair<string, string>>& tags
    )
    {
        m_host = host;
        m_port = port;
        m_ns = ns;
        if (0 < m_ns.length())
            m_ns += ".";
        m_tags = formatTags(tags);
    }

    bool StatsDConnection::send(const char* key, size_t value, const char* type, const vector<pair<string, string>>& tags)
    {

        string taglist = 0 < (m_tags.length() + tags.size()) ? "|#" : "";
        if (0 < m_tags.length())
            taglist += m_tags;
        if (0 < tags.size())
            taglist += (0 < taglist.length() ? "," : "") + formatTags(tags);
        
        pair<char*, size_t> buf = getSendBuffer();
        int send_size = send_size = _snprintf_s(
            buf.first,buf.second,
            "%s%s:%zd|%s%s", m_ns.c_str(), key, value, type, taglist.c_str()
        );

        if (send_size < 0)
            send_size = strlen(buf.first);
        return send({ buf.first, (size_t)send_size });
    }

    bool StatsDConnection::hasError() const
    {
        return 0 < m_lastError.length();
    }

    string StatsDConnection::getLastError(bool clearError = true)
    {   string result = m_lastError;
        if (clearError)
            m_lastError.clear();
        return result;
    }


    std::unique_ptr<StatsDConnection> CreateStatsDConnection()
    {
        std::unique_ptr<StatsDConnection> result(new SingleThreadedStatsDConnection());
    }

    string formatTags(const vector<pair<string, string>>& tags)
    {
        static string emptyString;
        if (tags.empty())
            return emptyString;

        stringstream result;
        for (int i = 0; i < tags.size(); i++)
        {
            result << (0 == i ? "" : ",") << tags[i].first << ":" << tags[i].second;
        }
        return result.str();
    }

};