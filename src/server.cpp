#include "server.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <string>
#include <ctime>
#include <unistd.h>
#include <csignal>
#include <iostream>
#include <cstdlib>

std::string month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
std::string week[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
int serverSocket;

std::string parseRequest(char buffer[]){
    std::string requestEndpoint;
    for (int i = 5; buffer[i] != ' '; i++){
        requestEndpoint += buffer[i];
    }
    return requestEndpoint;
}
std::string parseDate(){
    time_t now;
    tm* date;
    time(&now);
    date = gmtime(&now);
    std::string formatDate = week[date->tm_wday] + ", " + std::to_string(date->tm_mday) + " " + month[date->tm_mon] + " " + std::to_string(1900+date->tm_year)+ " " + std::to_string(date->tm_hour) + ":" + std::to_string(date->tm_min) + ":" + std::to_string(date->tm_sec) + " " + date->tm_zone;
    return formatDate;
}
void sigintHandler(int sig){
    std::_Exit(SIGINT);
}
void startServer(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    ::serverSocket = serverSocket;
    signal(SIGINT, sigintHandler);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 5);

    while (true){
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::string request = parseRequest(buffer);
        std::string response = "HTTP/1.1 200 OK\r\nDate: "+
        parseDate()+
        "\r\nContent-Length: "+
        std::to_string(8+request.size())+
        "\r\n\r\nHello "+
        request+
        "!!";
        send(clientSocket, response.c_str(), response.size(), 0);
        close(clientSocket);
    }
}