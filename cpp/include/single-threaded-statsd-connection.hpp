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
                if (-1 != sock)
                {
                    ::close(sock);
                    init();
    
                }
            }
            virtual int SingleThreadedStatsDConnection::open(const string& newHost, const short newPort,const string& newNS)
            {
                stringstream errorStream;
                close();
                host = newHost;
                port = newPort;
                ns = newNS;
                sock =  socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if (sock == -1 ) {
                    int err = errno;
                    errorStream << "Error creating socket, err=" << strerror(err);
                    lastError = errorStream.str();
                    return -1;
                }
                server.sin_family = AF_INET;
                server.sin_port = htons(port);
            
                int ret = inet_aton(host.c_str(), &server.sin_addr);
                if ( ret == 0 )
                {
                    // host must be a domain, get it from internet
                    struct addrinfo hints, *result = NULL;
                    memset(&hints, 0, sizeof(hints));
                    hints.ai_family = AF_INET;
                    hints.ai_socktype = SOCK_DGRAM;
            
                    ret = getaddrinfo(host.c_str(), NULL, &hints, &result);
                    if ( ret ) {
                        errorStream << "Error calling getaddrinfo, error=" << ret << ", msg=" << gai_strerror(ret);
                        close();
                        lastError = errorStream.str();
                        return -2;
                    }
                    struct sockaddr_in* host_addr = (struct sockaddr_in*)result->ai_addr;
                    memcpy(&server.sin_addr, &host_addr->sin_addr, sizeof(struct in_addr));
                    freeaddrinfo(result);
                }
                return 0;            
            }
            virtual string SingleThreadedStatsDConnection::getLastError(bool clearError = true)
            {   string result = lastError;
                if (clearError)
                {
                    lastError.clear();
                }
                return result;
            }
        protected:
            void SingleThreadedStatsDConnection::init()
            {
                sock = -1;
                memset(&server, 0x00, sizeof(server));
                memset(send_buffer,0x00, sizeof(send_buffer));
                ns = "";
                host = "";
                port = -1;
                lastError = "";
            }
            virtual pair<char*, size_t> SingleThreadedStatsDConnection::getSendBuffer()
            {
                return make_pair(send_buffer, sizeof(send_buffer));

            }
            virtual int SingleThreadedStatsDConnection::send(const pair<char*, size_t>& message)
            {
                stringstream errorStream;
                if (-1 != sock)
                {
                    lastError = "StatsDConnection not open.";
                    return -1;
                }
                int ret = sendto(sock, message.first, message.second, 0, (struct sockaddr *) &server, sizeof(server));
                if (ret == -1) 
                {
                    int err = errno;
                    errorStream << "Error sending " << message.second << " udp message to " << host << ":" << port << ", err=" << strerror(err);
                    lastError = errorStream.str();
                    return -2;
                }
                return 0;
            }            
        protected:        
            int     sock;
            struct  sockaddr_in server;
            string lastError;
            char send_buffer[MAX_STATSD_BUFFER_SIZE];
    };
};