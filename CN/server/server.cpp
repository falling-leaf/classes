#include <iostream>
#include <sstream>
#include <string>
#include <WinSock2.h>
#include<ws2tcpip.h>
#include <vector>
#include <time.h>

#pragma comment(lib,"ws2_32.lib")
#define MAX_BUF_SIZE 1024
#define TEST_PORT 5114

using namespace std;

struct CLIENT_INFO {
    SOCKET* socket; // 客户端的套接字
    int port; // 客户端的端口号
    char ip[INET_ADDRSTRLEN]; // 客户端的IP地址
};

std::vector<CLIENT_INFO> client_list; // 客户端列表

// 设置进行client处理的线程
DWORD WINAPI client_handler(LPVOID arg) {
    // 这里的arg就是客户端的套接字
    SOCKET* client_socket = (SOCKET*)arg;
    // 在队列中寻找并获取客户端的相关信息
    int client_port = 0;
    char client_ip[INET_ADDRSTRLEN];
    for (int i = 0; i < client_list.size(); i++) {
        if (client_list[i].socket == client_socket) {
            client_port = client_list[i].port;
            strcpy(client_ip, client_list[i].ip);
            break;
        }
    }
    cout << "New client connected!" << endl;
    cout << "Client info: [IP: " << client_ip << ", PORT: " << client_port << "]" << endl;
    // 循环接收客户端发送的消息
    char recv_buf[MAX_BUF_SIZE];
    char send_buf[MAX_BUF_SIZE];
    bool is_closed = false;
    while (1) {
        // 持续监听客户端的消息
        int ret = recv(*client_socket, recv_buf, MAX_BUF_SIZE, 0);
        // 如果收到的字节数大于0，那么意味着发送了一个请求
        if (ret > 0) {
            switch (recv_buf[0]) {
                // t: 获取时间
                case 't': {
                    time_t current_time;
                    time(&current_time);
                    struct tm *time_info;
                    time_info = localtime(&current_time);  // 将时间转换为当地时间
                    char time_str[100];
                    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
                    strcpy(send_buf, "[TIME] current time is: ");
                    strcpy(send_buf + 24, time_str);
                    if (send(*client_socket, send_buf, strlen(send_buf), 0) == SOCKET_ERROR) {
                        cout << "ERROR on [TIME] service" << endl;
                    } else {
                        cout << "SUCCESS on [TIME] service to client [IP: " << client_ip << ", PORT: " << client_port << "]"  << endl;
                        cout << "MESSAGE: " << send_buf << endl;
                    }
                    memset(send_buf, 0, MAX_BUF_SIZE);
                    break;
                }
                // n: 获取名字
                case 'n': {
                    DWORD name_length = MAX_COMPUTERNAME_LENGTH + 1;
                    TCHAR name_buf[MAX_COMPUTERNAME_LENGTH + 1];
                    GetComputerName(name_buf, &name_length);
                    char computer_name[MAX_COMPUTERNAME_LENGTH + 1];
                    // 根据是否定义UNICODE，将name_buf转换为字符串
                    #ifdef UNICODE
                        wcstombs(computer_name, name_buf, MAX_BUF_SIZE);
                    #else
                        strcpy(computer_name, name_buf);
                    #endif
                    strcpy(send_buf, "[NAME] server computer name is: ");
                    strcpy(send_buf + 32, computer_name);
                    if (send(*client_socket, send_buf, strlen(send_buf), 0) == SOCKET_ERROR) {
                        cout << "ERROR on [NAME] service" << endl;
                    } else {
                        cout << "SUCCESS on [NAME] service to client [IP: " << client_ip << ", PORT: " << client_port << "]"  << endl;
                        cout << "MESSAGE: " << send_buf << endl;
                    }
                    memset(send_buf, 0, MAX_BUF_SIZE);
                    break;
                }
                // l: 获取当前连接的所有客户端信息
                case 'l': {
                    // 用stringstream来拼接字符串
                    stringstream output_list;
                    for (int i = 0; i < client_list.size(); i++) {
                        output_list << "Client " << i << ": [IP: " << client_list[i].ip << ", PORT: " << client_list[i].port << "]" << endl;
                    }
                    string str = output_list.str();
                    str = str.substr(0, str.length() - 1);
                    strcpy(send_buf, "[LIST] current client list is: \n");
                    strcpy(send_buf + 32, str.c_str());
                    if (send(*client_socket, send_buf, strlen(send_buf), 0) == SOCKET_ERROR) {
                        cout << "ERROR on [LIST] service" << endl;
                    } else {
                        cout << "SUCCESS on [LIST] service to client [IP: " << client_ip << ", PORT: " << client_port << "]"  << endl;
                        cout << "MESSAGE: " << send_buf << endl;
                    }
                    memset(send_buf, 0, MAX_BUF_SIZE);
                    break;
                }
                // s: 发送消息转给其他客户端
                case 's': {
                    bool send_success = false;
                    // 约定传输格式为：s 0.0.0.0:1234 hello
                    string msg_decoder(recv_buf);
                    string send_addr = msg_decoder.substr(2, msg_decoder.find(':') - 2);
                    string send_port = msg_decoder.substr(msg_decoder.find(':') + 1, msg_decoder.find(' ') - msg_decoder.find(':') - 1);
                    string send_msg = msg_decoder.substr(msg_decoder.find('-') + 1);
                    // 寻找对应的套接字
                    SOCKET send_socket = INVALID_SOCKET;
                    for (int i = 0; i < client_list.size(); i++) {
                        if (strcmp(client_list[i].ip, send_addr.c_str()) == 0 && client_list[i].port == atoi(send_port.c_str())) {
                            send_socket = *client_list[i].socket;
                        }
                    }

                    // 对目标端口的信息发送
                    if (send_socket == INVALID_SOCKET) {
                        cout << "ERROR on [SEND] service: no such client" << endl;
                    } else {
                        // 准备发送消息
                        string client_msg = "[SEND] you have received a message from Client [IP: " + string(client_ip) + ", PORT: " + to_string(client_port) + "]:\n";
                        strcpy(send_buf, client_msg.c_str());
                        strcpy(send_buf + client_msg.length(), send_msg.c_str());
                        if (send(send_socket, send_buf, strlen(send_buf), 0) == SOCKET_ERROR) {
                            cout << "ERROR on [SEND] service" << endl;
                        } else {
                            send_success = true;
                            cout << "SUCCESS on [SEND] service to client [IP: " << send_addr << ", PORT: " << send_port << "]"  << endl;
                            cout << "MESSAGE: " << send_buf << endl;
                        }
                        memset(send_buf, 0, MAX_BUF_SIZE);
                    }

                    string src_msg = "[SEND]";

                    // 对源端口的信息发送
                    if (send_socket == INVALID_SOCKET) {
                        src_msg += " ERROR: no such client";
                    } else if (!send_success) {
                        src_msg += " ERROR: failed to send message to target client";
                    } else {
                        src_msg += " SUCCESS: message sent to target client";
                    }
                    strcpy(send_buf, src_msg.c_str());
                    if (send(*client_socket, send_buf, strlen(send_buf), 0) == SOCKET_ERROR) {
                        cout << "ERROR on [SEND] service" << endl;
                    } else {
                        cout << "SUCCESS on [SEND] service to client [IP: " << client_ip << ", PORT: " << client_port << "]"  << endl;
                        cout << "MESSAGE: " << send_buf << endl;
                    }
                    memset(send_buf, 0, MAX_BUF_SIZE);
                    break;
                }
                // d: 退出程序
                case 'd': {
                    is_closed = true;
                    break;
                }
                default: {
                    cout << "ERROR on handler: invalid command: " << recv_buf[0] << endl;
                    break;
                }
            }
            // 处理完毕后，需要刷新缓冲区
            memset(recv_buf, 0, MAX_BUF_SIZE);
            if (is_closed)
                break;
        }
    }
    // 处理完毕，break到这里
    // 关闭客户端的套接字
    for (int i = 0; i < client_list.size(); i++) {
        if (client_list[i].socket == client_socket) {
            client_list.erase(client_list.begin() + i);
            break;
        }
    }
    closesocket(*client_socket);
    delete client_socket;
    return 0;
}

int main() {
    cout << "THIS IS A SERVER TERMINAL" << endl;
    // 这是winsocket相关的内容，需要通过它进行树池话后才能使用
    WSAData wsadata;
    // 初始化winsocket，MAK#EWORDd表示版本号，&wsadata表示数据结构
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    // 创建监听的套接字，AF_INET表示IPv4，SOCK_STREAM表示TCP，0表示默认协议
    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == INVALID_SOCKET) {
        cout << "ERROR on creating socket" << endl;
        return 1;
    }
    // 配置套接字的地址信息
    SOCKADDR_IN listen_addr;
    listen_addr.sin_family = AF_INET; // IPv4
    listen_addr.sin_port = htons(TEST_PORT); // 监听的端口号
    listen_addr.sin_addr.S_un.S_addr = INADDR_ANY; // 监听的IP地址，INADDR_ANY表示监听所有IP地址
    // 使用bind()，将listen_socket绑定到listen_addr上
    bind(listen_socket, (struct sockaddr*)(&listen_addr), sizeof(listen_addr));
    // 开始监听，数字代表监听的最大队列
    if (listen(listen_socket, 10)) {
        cout << "ERROR on listening" << endl;
        return 1;
    } else {
        cout << "SUCCESS on listening, waiting for connection..." << endl;
    }
    // 开始准备处理每次client的连接请求
    while (1) {
        // 新建一个套接字，将建立连接的对应客户端放到其他线程处理
        // 同时由于采用多线程，这里如果直接定义局部变量会导致内存被删除，因此采用动态分配内存的方式
        SOCKET* client_socket = new SOCKET;
        // 接受客户端的连接
        *client_socket = accept(listen_socket, NULL, NULL);
        // 获取客户端的地址信息
        SOCKADDR_IN client_addr;
        int client_addr_len = sizeof(client_addr);
        struct CLIENT_INFO client_info;
        // 获取客户端的相关信息，将其从client_socket中获取
        if (getpeername(*client_socket, (struct sockaddr*)(&client_addr), &client_addr_len) == 0) {
            // TODO: 将IP地址转化为字符串
            // char client_ip[INET_ADDRSTRLEN];
            char* client_ip = inet_ntoa(client_addr.sin_addr);
            strcpy(client_info.ip, client_ip);
            // 端口
            client_info.port = ntohs(client_addr.sin_port);
            // socket
            client_info.socket = client_socket;
        } else {
            cout << "ERROR on getting client address" << endl;
            return 1;
        }
        // 将客户端信息进行存储
        client_list.push_back(client_info);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)client_handler, (LPVOID)client_socket, 0, NULL);
    }
    // 关闭套接字
    closesocket(listen_socket);
    // 释放winsocket资源
    WSACleanup();
    return 0;
}