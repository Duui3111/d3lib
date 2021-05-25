#ifndef __SOCET_H__
#define __SOCET_H__

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#define closesocket close
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <iostream>
#include <string>
#include <functional>

#ifdef _WIN32
#include <urlmon.h>             
#include <WinInet.h>
#pragma comment(lib, "urlmon.lib")  
#pragma comment(lib, "wininet.lib")

namespace d3lib
{
    class Fetch
    {
    public:
        Fetch() = default;
        static std::string fetch(std::string url, LPCWSTR lpszAgent = L"FetchAgent") { return DownloadBytes(url, lpszAgent); }

    private:
        static std::string DownloadBytes(std::string szUrl, LPCWSTR lpszAgent)
        {
            DWORD dwBytesRead = 0;
            std::string output;

            HINTERNET hOpen = InternetOpenW(lpszAgent, NULL, NULL, NULL, NULL);
            HINTERNET hFile = InternetOpenUrlA(hOpen, szUrl.c_str(), NULL, NULL, INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE, NULL);

            while (true)
            {
                char buffer[2000] = {};
                InternetReadFile(hFile, static_cast<PVOID>(buffer), _countof(buffer), &dwBytesRead);
                if (dwBytesRead == 0) break;
                output.append(static_cast<char*>(buffer), dwBytesRead);
            }

            InternetCloseHandle(hFile);
            InternetCloseHandle(hOpen);
            return output;
        }
    };
}
#endif

namespace d3lib
{
    struct Socket
    {
        static int Reverse(int* csock, char* str, int len) { return recv(*csock, str, len, NULL); }
        static int Send(int* csock, const char* str, int len) { return send(*csock, str, len, NULL); }
    };

    class Server
    {
    private:
        SOCKET sListen;
        SOCKET sConnect;
        SOCKADDR_IN addr;
        int* csock;

    public:
        Server(const char* addrs, int port)
        {
#ifdef _WIN32          
            WSAData wsaData;
            WORD DLLVERSION;
            DLLVERSION = MAKEWORD(2, 1);
            long ret = WSAStartup(DLLVERSION, &wsaData);
#endif

            sConnect = socket(AF_INET, SOCK_STREAM, NULL);

            addr.sin_addr.s_addr = inet_addr(addrs);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            sListen = socket(AF_INET, SOCK_STREAM, NULL);

            bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
            listen(sListen, 10);
        }

        ~Server()
        {
#ifdef _WIN32
            WSACleanup();
#endif
            closesocket(sListen);
            closesocket(sConnect);
        }

        void Handle(void(func(void*)))
        {
            int addrlen = sizeof(addr);

            while (true)
            {
                csock = (int*)malloc(sizeof(int));
                *csock = accept(sListen, (SOCKADDR*)&addr, &addrlen);
    
                std::cout << "A Connection was found with : " << inet_ntoa(addr.sin_addr) << std::endl;

                CreateThread(0, 0, (LPTHREAD_START_ROUTINE)func, (void*)csock, 0, 0);
            }
        }
    };

    class Client
    {
    public:
        long ret;
        SOCKET sConnect;

    public:
        Client(const char* addrs, int port) 
        {      
            WSAData wsaData;
            WORD DLLVERSION;
            DLLVERSION = MAKEWORD(2,1);
            ret = WSAStartup(DLLVERSION, &wsaData);

            SOCKADDR_IN addr;

            sConnect = socket(AF_INET, SOCK_STREAM, NULL);
            
            addr.sin_addr.s_addr = inet_addr(addrs);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
        }

        ~Client()
        {
#ifdef _WIN32
            WSACleanup();
#endif

            closesocket(sConnect);
        }

        void Handle(std::function<void(void)> func)
        {
            while(true)
            {
                if(ret > 1)
                {
                    func();
                }
            }
        }
    };
}

#endif
