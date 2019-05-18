////////////////////////////////////////////////////////////////////////////////
// MIT License
//
// Copyright (c) 2019 Miguel Aguiar
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////
// https://github.com/mcpca/fsm

#pragma once

#include <array>
#include <functional>
#include <future>
#include <memory>
#include <string>
#include <vector>

#include "defs.hpp"

#include "ThreadPool/ThreadPool.h"

namespace fsm
{
    namespace grid
    {
        struct grid_t;
    }
    namespace data
    {
        class data_t;
    }
    namespace queue
    {
        class queue_t;
    }

    namespace solver
    {
        //! Numerical parameters supplied by the user.
        struct params_t
        {
            scalar_t maxval;
            scalar_t tolerance;
        };

        static_assert(dim > 1,
                      "Number of dimensions must be greater than zero");

        class solver_t
        {
          public:
            solver_t(
                std::string const& filename,
                hamiltonian_t const& hamiltonian,
                std::array<std::pair<scalar_t, scalar_t>, dim> const& vertices,
                std::function<vector_t(input_t const&)> const& viscosity,
                params_t const& params);

            solver_t(solver_t&&) noexcept;
            solver_t& operator=(solver_t&&) noexcept;
            ~solver_t();

            void solve();

          private:
            friend solver_t make_solver(
                std::string const& filename,
                hamiltonian_t const& hamiltonian,
                std::array<std::pair<scalar_t, scalar_t>, dim> const& vertices,
                std::function<vector_t(input_t const&)> const& viscosity,
                params_t const& params);

            solver_t(std::string const& filename,
                     hamiltonian_t const& hamiltonian,
                     data::data_t cost,
                     grid::grid_t const& grid,
                     std::function<vector_t(input_t const&)> const& viscosity,
                     params_t const& params);

            void initialize();
            bool iterate();

            void work(index_t sweep_dir);
            scalar_t merge(index_t start, index_t end);

            std::string m_filename;
            hamiltonian_t m_hamiltonian;
            std::unique_ptr<grid::grid_t> m_grid;
            std::unique_ptr<data::data_t> m_soln;
            std::array<std::unique_ptr<data::data_t>, n_sweeps> m_worker;
            std::unique_ptr<data::data_t> m_cost;

            //! Numerical parameters.
            std::function<vector_t(input_t const&)> m_viscosity;
            scalar_t m_tolerance;

            std::unique_ptr<ThreadPool> m_pool;
            std::unique_ptr<queue::queue_t> m_queue;
        };
    }    // namespace solver
}    // namespace fsm