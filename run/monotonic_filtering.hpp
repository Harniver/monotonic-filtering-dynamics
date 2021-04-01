// Copyright © 2021 Giorgio Audrito. All Rights Reserved.

/**
 * @file monotonic_filtering.hpp
 * @brief Implementation of the case study comparing collection algorithms.
 */

#ifndef FCPP_MONOTONIC_FILTERING_H_
#define FCPP_MONOTONIC_FILTERING_H_

#include "lib/fcpp.hpp"

#ifdef INTERACTIVE
    #undef INTERACTIVE
    #define INTERACTIVE true
#else
    #define INTERACTIVE false
#endif


/**
 * @brief Namespace containing all the objects in the FCPP library.
 */
namespace fcpp {


//! @brief Number of randomised runs of the experiment.
constexpr int runs = 100;
//! @brief The maximum time of the simulation.
constexpr intmax_t end_time = INTERACTIVE ? std::numeric_limits<intmax_t>::max()-1 : 250;
//! @brief The communication range.
constexpr size_t comm = 100;


//! @brief Type for ABF values.
using pair_t = tuple<hops_t, device_t>;

//! @brief Infinite amount of hops away.
constexpr hops_t HOP_INF = std::numeric_limits<hops_t>::max()-1;


//! @brief Namespace containing the libraries of coordination routines.
namespace coordination {


namespace tags {
    //! @brief The number of devices.
    struct devices {};
    //! @brief The side of the square area of device deployment.
    struct side {};
    //! @brief The movement speed of nodes.
    struct speed {};

    //! @brief Distance to the source node.
    struct dist {};
    //! @brief Color representing the distance of the current node.
    struct dist_c {};
    //! @brief Estimated diameter.
    struct diam {};
    //! @brief Estimated diameter times the number of devices.
    struct diamdev {};

    //! @brief Ideal collection of values.
    struct ideal {};
    //! @brief Simple collection of values.
    struct simple {};
    //! @brief Filtered collection of values.
    struct filtered {};

    //! @brief Collected values.
    template <typename T>
    struct coll {};
    //! @brief Maximum collected values ever.
    template <typename T>
    struct coll_max {};
    //! @brief Color representing collected values.
    template <typename T>
    struct coll_c {};

    //! @brief Size of the node.
    struct node_size {};
    //! @brief Shape of the node.
    struct node_shape {};
}


//! @brief Gossips the maximum value ever.
FUN real_t max_gossip(ARGS, real_t val) { CODE
    return nbr(CALL, val, [&](field<real_t> n){
        return max(max_hood(CALL, n), val);
    });
}


//! @brief Computes the distance from a source through adaptive bellmann-ford, returning the constraining node.
FUN pair_t abf_constrain(ARGS, bool source) { CODE
    // local value for minimisation
    hops_t loc = source ? 0 : HOP_INF;
    return nbr(CALL, loc, [&](field<hops_t> n){
        n += 1; // add relative distance to distance estimates
        pair_t best = min_hood(CALL, make_tuple(n, nbr_uid(CALL)), make_tuple(loc, node.uid)); // select minimal distance
        return make_tuple(best, get<0>(best));
    });
}


//! @brief Computes the simple collection of values according to the given constraining nodes.
FUN real_t basic_collection(ARGS, device_t c, real_t val) { CODE
    return nbr(CALL, val, [&](field<real_t> n){
        return sum_hood(CALL, mux(nbr(CALL, c) == node.uid, n, real_t(0)), val);
    });
}


//! @brief Computes the collection of values according to the given constraining nodes with monotonic filtering.
FUN real_t filtered_collection(ARGS, pair_t c, real_t val) { CODE
    return nbr(CALL, val, [&](field<real_t> n){
        return sum_hood(CALL, mux(nbr(CALL, get<0>(c)) == get<0>(c)+1 and nbr(CALL, get<1>(c)) == node.uid, n, real_t(0)), val);
    });
}


//! @brief Comparison of collection dynamics.
MAIN() {
    // move device according to given speed
    real_t v = node.storage(tags::speed{});
    if (v > 0) rectangle_walk(CALL, make_vec(0,0), make_vec(1,1)*node.storage(tags::side{}), v, 1);
    // compute source
    hops_t hops = 2.5*node.storage(tags::side{})/comm;
    device_t source_id = node.current_time()/hops/2;
    bool source = node.uid == source_id;
    node.storage(tags::node_size{}) = source ? 20 : 12;
    node.storage(tags::node_shape{}) = source ? shape::cube : shape::sphere;

    // compute distances
    pair_t c = abf_constrain(CALL, source);
    // store distance and diameter for displaying purposes
    node.storage(tags::dist{}) = get<0>(c);
    node.storage(tags::dist_c{}) = get<0>(c) == HOP_INF ? BLACK : color::hsva(360.0*get<0>(c)/hops, 1, 1);
    node.storage(tags::diam{}) = max_gossip(CALL, get<0>(c) < HOP_INF ? get<0>(c) : 0);
    node.storage(tags::diamdev{}) = node.storage(tags::diam{}) * node.storage(tags::devices{});

    // compute collections
    real_t bc = basic_collection(CALL, get<1>(c), 1);
    real_t fc = filtered_collection(CALL, c, 1);
    real_t bm = max_gossip(CALL, bc);
    real_t fm = max_gossip(CALL, fc);
    // store collection for displaying purposes
    node.storage(tags::coll<tags::simple>{})       = bc;
    node.storage(tags::coll<tags::filtered>{})     = fc;
    node.storage(tags::coll_max<tags::simple>{})   = bm;
    node.storage(tags::coll_max<tags::filtered>{}) = fm;
    auto collection_color = [](device_t d, real_t v){
        if (v < d) return color::hsva(300.0*log2(v)/log2(d), 1, 1);
        else return color::hsva(300, 1, d/v);
    };
    node.storage(tags::coll_c<tags::simple>{})     = collection_color(node.storage(tags::devices{}), bc);
    node.storage(tags::coll_c<tags::filtered>{})   = collection_color(node.storage(tags::devices{}), fc);
}


} // namespace coordination


//! @brief Namespace for all FCPP components.
namespace component {

//! @brief Namespace of tags to be used for initialising components.
namespace tags {
    template <typename T>
    struct shape_tag;
    template <typename T>
    struct size_tag;
    template <typename... Ts>
    struct color_tag;
}

} // namespace component


//! @brief Namespace for component options.
namespace opt {
    using namespace coordination::tags;
    using namespace component::tags;

    using spawn_s = sequence::multiple<distribution::constant_i<size_t,devices>, distribution::constant_n<times_t, 0>>;

    using round_s = sequence::periodic_n<1, 0, 1, end_time+1>;

    using export_s = sequence::periodic_n<1, 0, 1, end_time>;

    using rectangle_d = distribution::rect<distribution::constant_n<real_t, 0>, distribution::constant_n<real_t, 0>, distribution::constant_i<real_t, side>, distribution::constant_i<real_t, side>>;

    using aggregator_t = aggregators<
        diam,               aggregator::max<real_t>,
        diamdev,            aggregator::max<real_t>,
        coll<ideal>,        aggregator::max<real_t>,
        coll<simple>,       aggregator::max<real_t>,
        coll<filtered>,     aggregator::max<real_t>,
        coll_max<ideal>,    aggregator::max<real_t>,
        coll_max<simple>,   aggregator::max<real_t>,
        coll_max<filtered>, aggregator::max<real_t>
    >;

    using time_plots = plot::split<speed, plot::split<devices, plot::plotter<aggregator_t, plot::time, coll>>>;

    using speed_plots = plot::split<devices, plot::filter<plot::time, filter::equal<end_time>, plot::plotter<aggregator_t, speed, coll_max>>>;

    using device_plots = plot::split<speed, plot::filter<plot::time, filter::equal<end_time>, plot::plotter<aggregator_t, devices, coll_max>>>;

    using hops_plots = plot::split<speed, plot::filter<plot::time, filter::equal<end_time>, plot::plotter<aggregator_t, aggregator::max<diam,true>, coll_max>>>;

    using hd_plots = plot::split<speed, plot::filter<plot::time, filter::equal<end_time>, plot::plotter<aggregator_t, aggregator::max<diamdev,true>, coll_max>>>;

    using plotter_t = plot::join<time_plots, plot::join<hd_plots, hops_plots, device_plots, speed_plots>>;

    // full option list
    DECLARE_OPTIONS(list,
        program<coordination::main>,
        synchronised<true>,
        parallel<INTERACTIVE>,
        exports<
            device_t, hops_t, real_t, vec<2>
        >,
        round_schedule<round_s>,
        log_schedule<export_s>,
        aggregator_t,
        tuple_store<
            devices,            device_t,
            side,               real_t,
            speed,              real_t,
            dist,               real_t,
            dist_c,             color,
            diam,               real_t,
            diamdev,            real_t,
            coll<ideal>,        real_t,
            coll<simple>,       real_t,
            coll<filtered>,     real_t,
            coll_max<ideal>,    real_t,
            coll_max<simple>,   real_t,
            coll_max<filtered>, real_t,
            coll_c<simple>,     color,
            coll_c<filtered>,   color,
            node_size,          real_t,
            node_shape,         shape
        >,
        spawn_schedule<spawn_s>,
        init<
            x,                  rectangle_d,
            side,               distribution::constant_i<real_t, side>,
            speed,              distribution::constant_i<real_t, speed>,
            devices,            distribution::constant_i<device_t, devices>,
            coll<ideal>,        distribution::constant_i<device_t, devices>,
            coll_max<ideal>,    distribution::constant_i<device_t, devices>
        >,
        connector<connect::fixed<comm>>,
        extra_info<devices, device_t, speed, real_t>,
        plot_type<plotter_t>,
        color_tag<coll_c<simple>, coll_c<filtered>>,
        size_tag<node_size>,
        shape_tag<node_shape>
    );

    //! @brief Area side for a given number of devices.
    inline real_t side_for(device_t d) {
        return sqrt(3000 * d);
    }

    auto make_parameters(plotter_t& p) {
        return batch::make_tagged_tuple_sequence(
            batch::arithmetic<seed>(0, runs-1, 1),
            batch::arithmetic<devices>(100, 1000, 300),
            batch::arithmetic<speed>(0, 2, 1),
            batch::stringify<output>("output/raw/batch", "txt"),
            batch::formula<side>([](auto t){
                return side_for(common::get<devices>(t));
            }),
            batch::constant<plotter>(&p)
        );
    }
} // namespace tags


}

#endif // FCPP_MONOTONIC_FILTERING_H_
