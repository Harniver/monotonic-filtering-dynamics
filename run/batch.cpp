// Copyright © 2021 Giorgio Audrito. All Rights Reserved.

#include "run/monotonic_filtering.hpp"

using namespace fcpp;

opt::plotter_t P;

int main() {
    auto param = opt::make_parameters(P);
    std::cerr << "\033[A\033[64C(total " << param.size()+1 << " files " << param.size()*(end_time+1) << " lines)\n" << std::flush;
    batch::run(component::batch_simulator<opt::list>{}, param);
    std::cout << plot::file("batch", P.build());
    return 0;
}
