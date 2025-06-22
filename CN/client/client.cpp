#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include<ws2tcpip.h>
#include <vector>

#define MAX_BUFFER_SIZE 1024

using namespace std;

SOCKET client_socket;
SOCKADDR_IN server_addr;
char recv_buffer[MAX_BUFFER_SIZE];
HANDLE hThread;

bool is_connected = false;

#pragma comment(lib, "Ws2_32.lib")

// 子线程用于反复接收消息
DWORD WINAPI recv_message_thread(LPVOID lpParam) {
    SOCKET* recv_socket = (SOCKET*)lpParam;
    memset(recv_buffer, 0, MAX_BUFFER_SIZE);
    while (1) {
        if (recv(*recv_socket, recv_buffer, MAX_BUFFER_SIZE, 0) > 0) {
            cout << "-------------------------------------" << endl;
            cout << "[RECV] message received from server: " << endl;
            cout << recv_buffer << endl;
            cout << "-------------------------------------" << endl;
            memset(recv_buffer, 0, MAX_BUFFER_SIZE);
        }
    }
}

bool connect_to_server() {
    // 如果已经连接，则不必再进行连接
    if (is_connected) {
        cout << "[CONNECT] You are already connected to the server." << endl;
        return true;
    }
    char server_ip[18];
    int server_port = -1;

    WSAData wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    // 创建客户端的socket
    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    cout << "[CONNECT] Please input server's IP: ";
    cin >> server_ip;
    cout << "[CONNECT] Please input server's port: ";
    cin >> server_port;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.S_un.S_addr = inet_addr(server_ip);
    if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cout << "[CONNECT] Failed to connect to server." << endl;
        return false;
    }
    // 启动子线程用于接收消息
    hThread = CreateThread(NULL, 0, recv_message_thread, &client_socket, 0, NULL);
    if (hThread == NULL) {
        cout << "[CONNECT] Failed to create thread." << endl;
        return false;
    }
    is_connected = true;
    cout << "[CONNECT] Connected to server." << endl;
    return true;
}

bool get_current_time() {
    if (is_connected) {
        char send_buffer[] = "t";
        if (send(client_socket, send_buffer, strlen(send_buffer), 0) == SOCKET_ERROR) {
            cout << "[TIME] Failed to send message to server." << endl;
            return false;
        }
        cout << "[TIME] Waiting for server's response..." << endl;
        Sleep(100);
        return true;
    } 
    cout << "[TIME] You are not connected to the server." << endl;
    return false;
}

bool get_computer_name() {
    if (is_connected) {
        char send_buffer[] = "n";
        if (send(client_socket, send_buffer, strlen(send_buffer), 0) == SOCKET_ERROR) {
            cout << "[NAME] Failed to send message to server." << endl;
            return false;
        }
        cout << "[NAME] Waiting for server's response..." << endl;
        Sleep(100);
        return true;
    } 
    cout << "[NAME] You are not connected to the server." << endl;
    return false;
}

bool get_client_list() {
    if (is_connected) {
        char send_buffer[] = "l";
        if (send(client_socket, send_buffer, strlen(send_buffer), 0) == SOCKET_ERROR) {
            cout << "[LIST] Failed to send message to server." << endl;
            return false;
        }
        cout << "[LIST] Waiting for server's response..." << endl;
        Sleep(100);
        return true;
    } 
    cout << "[LIST] You are not connected to the server." << endl;
    return false;
}

bool send_message() {
    if (is_connected) {
        string msg;
        string target_ip;
        string target_port;
        cout << "[SEND] Please input target's IP: ";
        cin >> target_ip;
        cout << "[SEND] Please input target's port: ";
        cin >> target_port;
        cout << "[SEND] Please input message: ";
        getchar();
        getline(cin, msg);
        msg += "\0";
        string send_buffer = "s " + target_ip + ":" + target_port + "-" + msg;
        if (send(client_socket, send_buffer.c_str(), send_buffer.length(), 0) == SOCKET_ERROR) {
            cout << "[SEND] Failed to send message to server." << endl;
            return false;
        }
        cout << "[SEND] Waiting for server's response..." << endl;
        Sleep(100);
        return true;
    } 
    cout << "[SEND] You are not connected to the server." << endl;
    return false;
}

bool disconnect_from_server() {
    if (is_connected) {
        char send_buffer[] = "d";
        if (send(client_socket, send_buffer, strlen(send_buffer), 0) == SOCKET_ERROR) {
            cout << "[DISCONNECT] Failed to send message to server." << endl;
            return false;
        }
        cout << "[DISCONNECT] Waiting for server's response..." << endl;
        // Sleep(100);
        is_connected = false;
        CloseHandle(hThread);
        closesocket(client_socket);
        WSACleanup();
        return true;
    } 
    cout << "[DISCONNECT] You are not connected to the server." << endl;
    return false;
}



int main() {
    cout << "THIS IS A CLIENT TERMINAL" << endl;
    cout << "Welcome to use the client terminal, you can use following commands:" << endl;
    cout << "+--------------------------------------------+" << endl;
    cout << "|      Function List              |  opcode  |" << endl;
    cout << "| Connect to the server           |     c    |" << endl;
    cout << "| Disconnect from the server      |     d    |" << endl;
    cout << "| Get current time                |     t    |" << endl;
    cout << "| Get server computer name        |     n    |" << endl;
    cout << "| Get client list                 |     l    |" << endl;
    cout << "| Send message to another client  |     s    |" << endl;
    cout << "| Exit the client terminal        |     e    |" << endl;
    cout << "+--------------------------------------------+" << endl;
    // 进行输入的循环读取，直到输入exit退出
    while (1) {
        cout << "[SHELL]> ";
        char opcode;
        cin >> opcode;
        switch (opcode) {
            case 'c': {
                is_connected = connect_to_server();
                if (is_connected)
                    cout << "[CONNECT] Connect to server successfully." << endl;
                else cout << "[CONNECT] Connect to server failed." << endl;
                break;
            }
            case 'd': {
                if (disconnect_from_server())
                    cout << "[DISCONNECT] Disconnect from server successfully." << endl;
                else cout << "[DISCONNECT] Disconnect from server failed." << endl;
                break;
            }
            case 't': {
                if (get_current_time())
                    cout << "[TIME] Get current time successfully." << endl;
                else cout << "[TIME] Get current time failed." << endl;
                break;
            }
            case 'n': {
                if (get_computer_name())
                    cout << "[NAME] Get server computer name successfully." << endl;
                else cout << "[NAME] Get server computer name failed." << endl;
                break;
            }
            case 'l': {
                if (get_client_list())
                    cout << "[LIST] Get client list successfully." << endl;
                else cout << "[LIST] Get client list failed." << endl;
                break;
            }
            case 's': {
                if (send_message())
                    cout << "[SEND] Send message successfully." << endl;
                else cout << "[SEND] Send message failed." << endl;
                break;
            }
            case 'e': {
                if (disconnect_from_server()) {
                    cout << "[EXIT] Disconnect from server successfully." << endl;
                    cout << "[EXIT] Exit the client terminal." << endl;
                    system("pause");
                    return 0;
                }
                else cout << "[EXIT] Disconnect from server failed." << endl;
                break;
            }
            default: {
                cout << "[ERROR] Invalid command." << endl;
                break;
            }
        }
    }
}