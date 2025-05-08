#pragma once
#include <string>
#define MAX_STATSD_BUFFER_SIZE 65507
using namespace std;
namespace statsd 
{
    class StatsDConnection
    {
        public: 
        virtual ~StatsDConnection() { close(); }

        virtual void close() = 0;
        virtual int open(
            const string& newHost,
            const short newPort,
            const string& newNS,
            const vector<pair<string, string>>& tags = vector<pair<string, string>>()
        ) = 0;
        virtual string getLastError(bool clearError = true) = 0;
        int send(const char* key, size_t value, const char* type, const vector<pair<string, string>>& tags = {});

        protected:
        StatsDConnection() : m_port(-1) {}
        virtual pair<char*, size_t> getSendBuffer() = 0;
        virtual int send(const pair<char*, size_t>& message) = 0;
        static string formatTags(const vector<pair<string, string>>& tags);

        protected:
        string  m_tags;
        string  m_ns;
        string  m_host;
        short   m_port;

    };
}