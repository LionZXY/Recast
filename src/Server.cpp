/**
 * @file
 * @brief Config file
 * @author LionZXY
 * @project Recast
 * @date 08.06.17
 *
 * Server file description
 *
 **/

#include <boost/log/trivial.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/filesystem.hpp>

#include "./headers/Server.h"

void initLogger() {
    std::string output = (std::string("recast_") + std::to_string(time(NULL)) + std::string(".log"));
    boost::filesystem::path dir("./logs/");
    if (!boost::filesystem::exists(dir))
        if (boost::filesystem::create_directory(dir)) {
            BOOST_LOG_TRIVIAL(info) << "Папка " << dir << " создана успешно";
        } else {
            BOOST_LOG_TRIVIAL(info) << "Невозможно создать папку " << dir;
        }
    boost::log::add_file_log(dir.string() + output);
    boost::log::add_console_log(std::cout);
}

Server *Server::initServer() {
    Server *server = new Server();
    initLogger();
    BOOST_LOG_TRIVIAL(info) << "Инициализация сервера...";
    server->isRunning = true;
    return server;
}

Server::Server() {
    isRunning = false;
}

void Server::mainLoop() {
    while (isRunning) {
        std::string cmd = "";
        std::cin >> cmd;
        manager.onCommand(this, cmd);
    }
}

void Server::onMessage(std::string msg) {
    BOOST_LOG_TRIVIAL(info) << msg;
}

Server::~Server() {
    //TODO
}