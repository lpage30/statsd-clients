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
            virtual int open(const string& newHost, const short newPort,const string& newNS) = 0;
            virtual string getLastError(bool clearError = true) = 0;
            int send(const char* key, size_t value, const char* type, float sample_rate);
        protected:
            StatsDConnection() : port(-1) {}
            virtual pair<char*, size_t> getSendBuffer() = 0;
            virtual int send(const pair<char*, size_t>& message) = 0;
        protected:
            string  ns;
            string  host;
            short   port;

    };
    std::unique_ptr<StatsDConnection> CreateStatsDConnection();
}