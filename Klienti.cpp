#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <string>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")

#define SERVER "192.168.178.106" // ip adresa e pajisjes qe lshon serverin
#define PORT 12345
#define BUFLEN 1024 // Buffer length for receiving data
#define ADMIN_PASSWORD "admin123" // Simple password for admin
#define FILE_NAME "example.txt" // File for read/write operations
#define EXEC_FILE "example.bat" // File for execution demonstration

void createExampleFiles() {
    // Create a simple text file for demonstration
    ofstream outFile(FILE_NAME);
    if (outFile.is_open()) {
        outFile << "This is a sample text file.\nFeel free to modify this content.\n";
        outFile.close();
    }

    // Create a batch file for demonstration
    ofstream execFile(EXEC_FILE);
    if (execFile.is_open()) {
        execFile << "@echo off\n";
        execFile << "echo This is a demonstration of the execute command.\n";
        execFile << "pause\n";
        execFile.close();
    }
}

void readFromFile() {
    ifstream inFile(FILE_NAME);
    if (inFile.is_open()) {
        string line;
        cout << "Reading file contents:\n";
        while (getline(inFile, line)) {
            cout << line << "\n";
        }
        inFile.close();
    } else {
        cerr << "Unable to open file for reading.\n";
    }
}

void writeToFile() {
    ofstream outFile(FILE_NAME, ios::app);
    if (outFile.is_open()) {
        string input;
        cout << "Enter text to write: ";
        cin.ignore(); // Clear input buffer
        getline(cin, input);
        outFile << input << "\n";
        outFile.close();
        cout << "Text written to file.\n";
    } else {
        cerr << "Unable to open file for writing.\n";
    }
}

void executeFile() {
    int result = system(EXEC_FILE);
    if (result != 0) {
        cerr << "Execution failed.\n";
    }
}

int main() {
    createExampleFiles(); // Ensure example files are created on startup

    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed. Terminating.\n";
        return 1;
    }

    // Create socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    // Set up server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER);

    // Prompt for role
    string role;
    cout << "Enter role (admin/client): ";
    cin >> role;

    // Handle admin password check
    if (role == "admin") {
        string password;
        cout << "Enter admin password: ";
        cin >> password;
        if (password != ADMIN_PASSWORD) {
            cerr << "Incorrect password. Terminating.\n";
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    }

    // Prompt for command
    string command;
    cout << "Enter command (read/write/execute): ";
    cin >> command;

    // Send command to server
    if (sendto(clientSocket, command.c_str(), command.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Send failed. Error: " << WSAGetLastError() << "\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Handle commands locally
    if (command == "read") {
        char buffer[BUFLEN];
        int serverAddrLen = sizeof(serverAddr);
        int recvLen = recvfrom(clientSocket, buffer, BUFLEN - 1, 0, (sockaddr*)&serverAddr, &serverAddrLen);
        if (recvLen == SOCKET_ERROR) {
            cerr << "Receive failed. Error: " << WSAGetLastError() << "\n";
        } else {
            buffer[recvLen] = '\0'; // Null-terminate the received data
            cout << "File contents received from server:\n";
            cout << buffer << "\n";
        }
    } else if (command == "write") {
    if (role == "admin") {
        string input;
        cout << "Enter text to write: ";
        cin.ignore(); // Clear input buffer
        getline(cin, input);

        // Send data to server for writing
        if (sendto(clientSocket, input.c_str(), input.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cerr << "Send failed. Error: " << WSAGetLastError() << "\n";
        }
    } else {
        cerr << "Permission denied. Only admin can write to the file.\n";
    }
    } else if (command == "execute") {
        if (role == "admin") {
            executeFile();
        } else {
            cerr << "Permission denied. Only admin can execute the file.\n";
        }
    } else {
        cerr << "Invalid command.\n";
    }

    // Pause before exiting
    cout << "Press Enter to exit...\n";
    cin.ignore(); // Clear the input buffer
    cin.get(); // Wait for user input

    // Clean up
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
