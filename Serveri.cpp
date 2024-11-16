#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
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