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

#define FSM_IS_PARALLEL

#ifndef FSM_IS_PARALLEL
#    define FSM_N_WORKERS 1
#endif

#ifndef FSM_N_WORKERS
#    include <thread>
#endif

#ifndef FSM_N_DIMS
#    define FSM_N_DIMS 2
#endif

namespace fsm
{
    constexpr auto dim = FSM_N_DIMS;

    static_assert(dim > 1, "The number of dimensions must be at least two.");

    constexpr auto n_sweeps = 1 << dim;
    constexpr auto n_boundaries = 2 * dim;

    using index_t = size_t;
    using point_t = std::array<index_t, dim>;

    using scalar_t = float;
    using vector_t = std::array<scalar_t, dim>;

#ifdef FSM_USE_ROWMAJOR
    using input_t = index_t;
#else
    using input_t = point_t;
#endif

    using hamiltonian_t = std::function<double(input_t, vector_t)>;

    namespace parallel
    {
#ifdef FSM_N_WORKERS
        constexpr auto n_workers = FSM_N_WORKERS;

        static_assert(n_workers > 0,
                      "The number of threads must be greater than zero.");
#else
        const auto n_workers = std::thread::hardware_concurrency() >= n_sweeps
                                   ? n_sweeps
                                   : std::thread::hardware_concurrency();
#endif
    }    // namespace parallel
}    // namespace fsm
