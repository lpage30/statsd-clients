#include <stdio.h>
#include "single-threaded-statsd-connection.hpp"
#if !defined(_MSC_VER)
    #define _snprintf_s(a1,a2,a3,...) snprintf(a1,a2,__VA_ARGS__)
    #define _TRUNCATE ((size_t)-1)
#endif
namespace statsd {
    int StatsDConnection::send(const char* key, size_t value, const char* type, const vector<pair<string, string>>& tags)
    {

        string taglist = (0 < (m_tags.length() + tags.size()) ? "#" : "";
        if (0 < m_tags.length())
        {
            taglist += m_tags;
            if (0 < tags.size())
                taglist += ",";
        }
        if (0 < tags.size())
            taglist += formatTags(tags);
        pair<char*, size_t> buf = getSendBuffer();
        int send_size = send_size = _snprintf_s(
            buf.first,
            buf.second,
            "%s%s:%zd|%s%s", m_ns.c_str(), key, value, type, taglist.c_str()
        );

        if (send_size < 0)
        {
            send_size = strlen(buf.first);
        }
        return send(make_pair(buf.first, (size_t)send_size));

    }
    string StatsDConnection::formatTags(const vector<pair<string, string>>& tags)
    {
        stringstream result;
        for (int i = 0; i < tags.size(); i++)
        {
            result << (0 == i ? "" : ",") << tags[i].first << ":" << tags[i].second;
        }
        return result.str()
    }

    std::unique_ptr<StatsDConnection> CreateStatsDConnection()
    {
        std::unique_ptr<StatsDConnection> result(new SingleThreadedStatsDConnection());
    }
};