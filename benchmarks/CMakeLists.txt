set(GROUP_NAME benchmark)

add_compile_options(
    $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-falign-functions=64>)

find_package(benchmark REQUIRED)

set(BENCHMARKS
    delegate
    ev_queue)

foreach(BENCHMARK ${BENCHMARKS})
    add_subdirectory(${BENCHMARK})
endforeach()
