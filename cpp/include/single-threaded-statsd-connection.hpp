// a single-threaded C++ statsd connection
#pragma once
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sstream>
#include "statsd-connection.hpp"

namespace statsd {

    class SingleThreadedStatsDConnection: public StatsDConnection
    {
        public:
        SingleThreadedStatsDConnection() : StatsDConnection()
        {
            init();
        }
        virtual void SingleThreadedStatsDConnection::close()
        {
            if (-1 != m_sock)
            {
                ::close(m_sock);
                init();

            }
        }

        virtual int SingleThreadedStatsDConnection::open(
            const string& newHost,
            const short newPort,
            const string& newNS,
            const vector<pair<string, string>>& tags = {}
        )
        {
            stringstream errorStream;
            close();
            m_host = newHost;
            m_port = newPort;
            m_ns = newNS;
            m_tags = formatTags(tags);
            m_sock =  socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (m_sock == -1 ) {
                int err = errno;
                errorStream << "Error creating socket, err=" << strerror(err);
                m_lastError = errorStream.str();
                return -1;
            }
            m_server.sin_family = AF_INET;
            m_server.sin_port = htons(m_port);
        
            int ret = inet_aton(m_host.c_str(), &m_server.sin_addr);
            if ( ret == 0 )
            {
                // host must be a domain, get it from internet
                struct addrinfo hints, *result = NULL;
                memset(&hints, 0, sizeof(hints));
                hints.ai_family = AF_INET;
                hints.ai_socktype = SOCK_DGRAM;
        
                ret = getaddrinfo(m_host.c_str(), NULL, &hints, &result);
                if ( ret ) {
                    errorStream << "Error calling getaddrinfo, error=" << ret << ", msg=" << gai_strerror(ret);
                    close();
                    m_lastError = errorStream.str();
                    return -2;
                }
                struct sockaddr_in* host_addr = (struct sockaddr_in*)result->ai_addr;
                memcpy(&m_server.sin_addr, &host_addr->sin_addr, sizeof(struct in_addr));
                freeaddrinfo(result);
            }
            return 0;            
        }

        virtual string SingleThreadedStatsDConnection::getLastError(bool clearError = true)
        {   string result = m_lastError;
            if (clearError)
            {
                m_lastError.clear();
            }
            return result;
        }

        protected:
        void SingleThreadedStatsDConnection::init()
        {
            m_sock = -1;
            memset(&m_server, 0x00, sizeof(m_server));
            memset(m_send_buffer,0x00, sizeof(m_send_buffer));
            m_ns = "";
            m_host = "";
            m_port = -1;
            m_lastError = "";
            m_tags.clear();
        }
        virtual pair<char*, size_t> SingleThreadedStatsDConnection::getSendBuffer()
        {
            return make_pair(m_send_buffer, sizeof(m_send_buffer));

        }
        virtual int SingleThreadedStatsDConnection::send(const pair<char*, size_t>& message)
        {
            stringstream errorStream;
            if (-1 != m_sock)
            {
                m_lastError = "StatsDConnection not open.";
                return -1;
            }
            int ret = sendto(m_sock, message.first, message.second, 0, (struct sockaddr *) &m_server, sizeof(m_server));
            if (ret == -1) 
            {
                int err = errno;
                errorStream << "Error sending " << message.second << " udp message to " << m_host << ":" << m_port << ", err=" << strerror(err);
                m_lastError = errorStream.str();
                return -2;
            }
            return 0;
        }            

        protected:        
        int     m_sock;
        struct  sockaddr_in m_server;
        string m_lastError;
        char m_send_buffer[MAX_STATSD_BUFFER_SIZE];
    };
};