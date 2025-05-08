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

        bool open(
            const string& host,
            const short port,
            const string& ns,
            const vector<pair<string, string>>& tags = {}
        );

        bool isOpen() const;

        void close();

        bool hasError() const;
        string getLastError(bool clearError = true);
    
        bool count(
            const char* key, 
            size_t value = 1,
            const vector<pair<string, string>>& tags = {}
        );

        bool histogram(
            const char* key, 
            size_t value = 1,
            const vector<pair<string, string>>& tags = {}
        );

        bool timing(
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