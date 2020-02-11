#include "../recyclr-types.h"

#include <vector>
#include <utility>
#include <string>

using namespace std;
using xy_point = pair<double, double>;

#define CONFIG_FILE "config/client-configuration.json"

struct mesh_net_config {
    bool           enabled;
    vector<string> supported_messages;
};

class RecyclrClientConfig {
    public:

    double                 min_battery_percent;
    double                 max_battery_percent;
    int                    messaging_port;
    int                    streaming_port;
    int                    general_core;
    int                    networking_core;
    int                    controller_core;
    int                    image_controller_core;
    int                    default_core;
    vector<xy_point>       geo_fence_points;
    bool                   calibrate_on_startup;
    struct mesh_net_config net_config;
    bool                   sync_labels_from_server;
    bool                   sync_streams_from_server;
    unsigned int           server_sync_rate;

    RecyclrClientConfig();

    static void load_from_json(RecyclrClientConfig* config);
};