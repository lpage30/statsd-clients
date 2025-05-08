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

        virtual bool open(
            const string& host,
            const short port,
            const string& ns,
            const vector<pair<string, string>>& tags = {}
        ) = 0;
        
        virtual bool isOpen() const = 0;
        
        virtual void close() = 0;

        bool send(
            const char* key,
            size_t value,
            const char* type,
            const vector<pair<string, string>>& tags = {}
        );

        bool hasError() const;
        string getLastError(bool clearError = true);

        protected:
        StatsDConnection() : m_port(-1) {}

        void configure(
            const string& host = "",
            const short port = -1,
            const string& ns = "",
            const vector<pair<string, string>>& tags = {}
        );

        virtual pair<char*, size_t> getSendBuffer() = 0;
        virtual bool send(const pair<char*, size_t>& message) = 0;

        protected:
        string  m_tags;
        string  m_ns;
        string  m_host;
        short   m_port;
        string  m_lastError;
    };
}