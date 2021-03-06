#include <cmath>
#include <cstdio>
#include <iostream>
#include <numeric>

#include "marlin/solver.hpp"

#include "hdf5.hpp"
#include "timer.hpp"

int main(int argc, char** argv)
{
#if MARLIN_N_DIMS == 2
    constexpr char const* filename = "../data/constant_field.h5";

    std::array<std::pair<marlin::scalar_t, marlin::scalar_t>, marlin::dim>
        vertices = { { { -1.0, 1.0 }, { -1.0, 1.0 } } };

    marlin::scalar_t v[] = { -0.5, 0.5 };

    if(argc >= 3)
    {
        sscanf(argv[1], "%lf", v);
        sscanf(argv[2], "%lf", v + 1);
    }

    auto h = [&v](auto, auto&& p) -> marlin::scalar_t {
        return std::sqrt(std::inner_product(
                   std::begin(p), std::end(p), std::begin(p), 0.0)) -
               p[0] * v[0] - p[1] * v[1];
    };

    auto viscosity = [&v](auto const&) {
        return marlin::vector_t{
            static_cast<marlin::scalar_t>(1.0 + std::abs(v[0])),
            static_cast<marlin::scalar_t>(1.0 + std::abs(v[1]))
        };
    };

    auto ds = h5io::read(filename, "cost_function");

    marlin::solver::params_t params;
    params.tolerance = 1.0e-4;
    params.maxval = 5.0;

    marlin::solver::solver_t s(std::move(ds.data), ds.size, vertices, params);

    timer::timer_t t;

    s.solve(h, viscosity);

    std::cout << "Took " << t.get_elapsed_sec<double>() << " seconds."
              << std::endl;

    h5io::write(filename, "value_function", s.steal(), ds.size);

#else
    (void)argc;
    (void)argv;
#endif
    return 0;
}
