#pragma once
#include <string>
#include "statsd-connection.hpp"
using namespace std;
namespace statsd 
{
    class StatsDClient
    {
        public:
            StatsDClient(std::unique_ptr<StatsDConnection> connection);
            virtual ~StatsDClient();
            int config(const string& host, const short port,const string& ns);
            string getLastError(bool clearError = true);
        public:
            int inc(const char* key, float sample_rate = 1.0);
            int dec(const char* key, float sample_rate = 1.0);
            int count(const char* key, size_t value, float sample_rate = 1.0);
            int gauge(const char* key, size_t value, float sample_rate = 1.0);
            int timing(const char* key, size_t ms, float sample_rate = 1.0);

        protected:
            std::unique_ptr<StatsDConnection> m_pConnection;
        
    };
};