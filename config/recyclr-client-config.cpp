#include "recyclr-client-config.h"

#include "../recyclr-utils.h"
#include "json/json11.hpp"

#include <fstream>

using JSON = json11::Json;

RecyclrClientConfig::RecyclrClientConfig() :
    min_battery_percent(0),
    max_battery_percent(100),
    messaging_port(-1),
    streaming_port(-1),
    general_core(0),
    networking_core(-1),
    controller_core(-1),
    image_controller_core(-1),
    default_core(0),
    geo_fence_points(),
    calibrate_on_startup(false),
    sync_labels_from_server(false),
    sync_streams_from_server(false),
    server_sync_rate(0)
{
    net_config = {false, {}};
}

void RecyclrClientConfig::load_from_json(RecyclrClientConfig* config)
{
    ifstream conf(CONFIG_FILE);
    stringstream buffer;
    buffer << conf.rdbuf();

    string err;
    const auto config_json = JSON::parse(buffer.str(), err);

    config->min_battery_percent      = config_json["min_battery_percent"].number_value();
    config->max_battery_percent      = config_json["max_battery_percent"].number_value();
    config->messaging_port           = config_json["messaging_port"].number_value();
    config->streaming_port           = config_json["streaming_port"].number_value();
    config->general_core             = config_json["general_core"].number_value();
    config->networking_core          = config_json["networking_core"].number_value();
    config->controller_core          = config_json["controller_core"].number_value();
    config->image_controller_core    = config_json["image_controller_core"].number_value();
    config->default_core             = config_json["default_core"].number_value();
    config->calibrate_on_startup     = config_json["calibrate_on_startup"].bool_value();
    config->net_config.enabled       = config_json["horizontal_communication"]["enabled"].bool_value();
    config->sync_labels_from_server  = config_json["sync_labels_from_server"].bool_value();
    config->sync_streams_from_server = config_json["sync_streams_from_server"].bool_value();
    config->server_sync_rate         = config_json["server_sync_rate"].number_value();
    
}