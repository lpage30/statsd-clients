#pragma once
#include <string>
#include "statsd-connection.hpp"
using namespace std;
namespace statsd 
{
    class StatsDClient
    {
        public:
        StatsDClient(StatsDConnection* pconnection);
        virtual ~StatsDClient();
        int config(
            const string& host,
            const short port,
            const string& ns,
            const vector<pair<string, string>>& tags = {}
        );

        string getLastError(bool clearError = true);
    
        int count(
            const char* key, 
            size_t value = 1,
            const vector<pair<string, string>>& tags = {}
        );
        int histogram(
            const char* key, 
            size_t value = 1,
            const vector<pair<string, string>>& tags = {}
        );
        int timing(
            const char* key,
            size_t ms,
            const vector<pair<string, string>>& tags = {}
        );

        protected:
        std::unique_ptr<StatsDConnection> m_pConnection;

        public:
        static StatsDClient& getInstance();
    };
    
};