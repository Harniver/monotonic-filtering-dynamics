// Copyright © 2021 Giorgio Audrito. All Rights Reserved.

#define INTERACTIVE

#include "run/monotonic_filtering.hpp"
#include "lib/simulation/displayer.hpp"

namespace fcpp {
namespace component {
DECLARE_COMBINE(interactive_simulator, displayer, calculus, simulated_connector, simulated_positioner, timer, scheduler, logger, storage, spawner, identifier, randomizer);
}
}


using namespace fcpp;
using namespace coordination::tags;
using namespace component::tags;

opt::plotter_t P;

int main() {
    constexpr device_t dev_num = 100; // number of devices
    for (int dev_speed = 0; dev_speed <= 1; ++dev_speed) { // movement speed of devices
        std::cout << "/*\n";
        {
            component::interactive_simulator<opt::list>::net network{common::make_tagged_tuple<name,texture,plotter,devices,side,speed>("Collection Dynamics through Monotonic Filtering", "land.jpg", &P, dev_num, opt::side_for(dev_num), dev_speed)};
            network.run();
        }
        std::cout << "*/\n";
        std::cout << plot::file("batch", P.build());
    }
    return 0;
}
