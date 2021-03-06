/**
 * @file server.cpp
 * @brief Networking server source file
 * @author StealthTech
 * @project Recast-server
 * @date 17.06.17
 * @email st3althtech@mail.ru
 *
 **/

#include <iostream>
#include <thread>
#include <netinet/in.h> // struct sockaddr_in
#include <boost/log/trivial.hpp>

#include "io/network/Networking.hpp"
#include "io/network/NetworkServer.hpp"

NetworkServer::NetworkServer(uint32_t port, ICommandSender *sender, bool isTCP) : port(port), isTCP(isTCP),
                                                                                  sender(sender) {};

void NetworkServer::run() {
    try {
        if (isTCP) {
            SocketTCP sock(port, 25);
            BOOST_LOG_TRIVIAL(info) << "Server is running on port " << port << " for TCP connections";
            fork();

            isRunning = true;
            while (isRunning) {
                shared_ptr<SocketTCP> client = sock.accept();
                listenFor(client);
            }
        } else {
            SocketUDP sock(port);
            BOOST_LOG_TRIVIAL(info) << "Server is running on port " << port << " for UDP connections";
            fork();

            isRunning = true;
            while (isRunning) {
                shared_ptr<SocketUDP> client = make_shared<SocketUDP>(sock);
                listenForBytes(client);
            }
        }
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}

void NetworkServer::listenFor(shared_ptr<SocketTCP> client) {
    client->setRecvTimeout(30, 0); // s, ms
    while (true)
        try {
            string request = client->recv();
            string response = exchange(request);
            client->send(response);
        } catch (const exception &e) {
            cerr << "[ERR] An exception occurred: " << e.what() << endl;
            return;
        }
}

void NetworkServer::listenForBytes(shared_ptr<SocketTCP> client) {
    client->setRecvTimeout(30, 0); // s, ms
    while (true)
        try {
            char *request = client->recvBytes(1024);
            char *response = exchange(request);
            client->send(response);
        } catch (const exception &e) {
            cerr << "[ERR] An exception occurred: " << e.what() << endl;
            return;
        }
}

void NetworkServer::listenFor(shared_ptr<SocketUDP> client) {
    while (true)
        try {
            struct sockaddr_in senderAddr;
            string request = client->recvFrom(senderAddr);
            string response = exchange(request);
            client->sendTo(senderAddr, response);
        } catch (const exception &e) {
            cerr << "[ERR] An exception occurred: " << e.what() << endl;
            return;
        }
}

void NetworkServer::listenForBytes(shared_ptr<SocketUDP> client) {
    while (true)
        try {
            struct sockaddr_in senderAddr;
            char *request = client->recvBytesFrom(senderAddr);
            char *response = exchange(request);
            client->sendTo(senderAddr, response);
        } catch (const exception &e) {
            cerr << "[ERR] An exception occurred: " << e.what() << endl;
            return;
        }
}

void NetworkServer::shutdown() {
    isRunning = false;
}

string NetworkServer::exchange(const string request) {
    nofityListener(const_cast<char *>(request.data()));

    return request;
}

char *NetworkServer::exchange(char *request) {
    nofityListener(request);

    return request;
}

bool NetworkServer::registerListener(NetworkListener *listener) {
    auto temp = listeners.find(listener->getId());
    if (temp != listeners.end()) {
        BOOST_LOG_TRIVIAL(info) << "[ERR] Listener id " << listener->getId() << " is already registered";
        return false;
    }
    listeners[listener->getId()] = listener;
    BOOST_LOG_TRIVIAL(info) << "[INFO] Registered listener id " << listener->getId();
    return true;
}

bool NetworkServer::removeListener(NetworkListener *listener) {
    auto temp = listeners.find(listener->getId());
    if (temp == listeners.end()) {
        BOOST_LOG_TRIVIAL(info) << "[ERR] Listener id " << listener->getId() << " is not registered";
        return false;
    } else {
        listeners.erase(listener->getId());
        BOOST_LOG_TRIVIAL(info) << "[INFO] Unregistered listener id " << listener->getId();
        return true;
    }
}

bool NetworkServer::nofityListener(char *request) {
    int id = request[0];
//    int id = 0; // DEBUG
    auto temp = listeners.find(id);

    if (temp == listeners.end()) {
        return false;
    } else {
        (*temp).second->onPacket(request, sender);
    }
}
