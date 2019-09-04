add_subdirectory(external)

find_package(h5cpp REQUIRED)

add_library(marlin
            src/solver.cpp
            src/solver_internals.cpp
            src/grid.cpp
            src/data.cpp
            src/io.cpp)

target_compile_features(marlin PUBLIC cxx_std_14)

target_compile_options(marlin
                       PRIVATE -Wall
                               -Wextra
                               -Werror
                               -pedantic)

target_include_directories(marlin PUBLIC include PRIVATE src)
target_link_libraries(marlin PRIVATE h5cpp PUBLIC thread_pool)