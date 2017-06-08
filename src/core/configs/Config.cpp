/**
 * @file
 * @brief Config description
 * @author LionZXY
 * @project Recast
 * @date 08.06.17
 *
 * Save INSTANCE config and get if you need this.
 * 
 **/

#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <configs/Config.h>


using namespace boost::property_tree::json_parser;
const int CONFIG_VERSION = 1;
const std::string DEFAULT_FOLDER = std::string("./config/");
const std::string DEFAULT_CONFIG = std::string("general.json");
static shared_ptr<Config> INSTANCE = NULL;

shared_ptr<Config> Config::instance() {
    if (INSTANCE != NULL)
        return INSTANCE;
    else {
        INSTANCE = shared_ptr<Config>(new Config(DEFAULT_CONFIG));
        return INSTANCE;
    }
}

Config::Config(std::string filename) {
    this->filename = DEFAULT_FOLDER + filename;
    try {
        read_json(this->filename, pt);
    } catch (const json_parser_error &e) {
        save();
    }
    if (pt.get("config.version", NULL) == NULL)
        pt.put("config.version", CONFIG_VERSION);
}

ptree &Config::tree() {
    return this->pt;
}

void Config::save() {
    boost::filesystem::path dir(DEFAULT_FOLDER);
    if (!boost::filesystem::exists(dir))
        if (boost::filesystem::create_directory(dir)) {
            BOOST_LOG_TRIVIAL(info) << "Папка " << DEFAULT_FOLDER << " создана успешно";
            write_json(filename, pt);
        } else {
            BOOST_LOG_TRIVIAL(info) << "Невозможно создать папку " << DEFAULT_FOLDER;
        }
    else write_json(filename, pt);
}

void Config::load() {
    read_json(filename, pt);
}

Config::~Config() {
    save();
    INSTANCE = NULL;
}