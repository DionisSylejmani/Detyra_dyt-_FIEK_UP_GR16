#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> // ka funksionalitete edhe per UDP
#include <fstream>
#include <string>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define PORT 12345
#define BUFLEN 1024 // bufferi
#define FILE_NAME "example.txt" // file per komunikim

void handleWriteRequest(const string& data) {
    ofstream outFile(FILE_NAME, ios::app);
    if (outFile.is_open()) {
        outFile << data << "\n";
        outFile.close();
        cout << "Data written to file on the server.\n";
    } else {
        cerr << "Unable to open file for writing on the server.\n";
    }
}

int main(){
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[BUFLEN];

    // Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed. Terminating.\n";
        return 1;
    }

    // Krijimi i socketave
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    // Struktura e adreses se serverit
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Bind failed. Error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server is running and waiting for commands...\n";


    while (true) {
        memset(buffer, 0, BUFLEN); // fshirje e bufferit
        int recvLen = recvfrom(serverSocket, buffer, BUFLEN - 1, 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if (recvLen == SOCKET_ERROR) {
            cerr << "Receive failed. Error: " << WSAGetLastError() << "\n";
            continue; // mban serverin on
        }

        buffer[recvLen] = '\0'; // Null-terminate received data
        string command(buffer);
        cout << "Received command: " << command << "\n";

        if (command == "read") {
            // lexon permbajtjen dhe e qon ne cmd te admin/klientit
            ifstream inFile(FILE_NAME);
            if (inFile.is_open()) {
                string fileContents((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
                inFile.close();

                // logjika e dergimit
                if (sendto(serverSocket, fileContents.c_str(), fileContents.size(), 0, (sockaddr*)&clientAddr, clientAddrLen) == SOCKET_ERROR) {
                    cerr << "Send failed. Error: " << WSAGetLastError() << "\n";
                }
            } else {
                string errorMsg = "Unable to open file for reading.";
                sendto(serverSocket, errorMsg.c_str(), errorMsg.size(), 0, (sockaddr*)&clientAddr, clientAddrLen);
            }
        } else if (command == "write") {
            // merr te dhenat prej klientit per shkrim
            char writeBuffer[BUFLEN];
            int recvWriteLen = recvfrom(serverSocket, writeBuffer, BUFLEN - 1, 0, (sockaddr*)&clientAddr, &clientAddrLen);
            if (recvWriteLen == SOCKET_ERROR) {
                cerr << "Receive failed for write data. Error: " << WSAGetLastError() << "\n";
            } else {
                writeBuffer[recvWriteLen] = '\0'; // Null-terminate received data
                handleWriteRequest(writeBuffer);
            }
        } else {
            string errorMsg = "Unsupported command received.";
            sendto(serverSocket, errorMsg.c_str(), errorMsg.size(), 0, (sockaddr*)&clientAddr, clientAddrLen);
        }
    }

    // Fshirja
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}