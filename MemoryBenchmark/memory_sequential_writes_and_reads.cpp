#include <cstdlib>
#include <cstring>
#include <string.h>

#include "benchmark/benchmark.h"

constexpr size_t TOTAL_MEMORY_SIZE = 64 * 1024 * 1024;

#define CHUNK_SIZE_ARGS \
    ->RangeMultiplier(2)->Range(1024, 1024 * 1024) \
    ->UseRealTime()
// -------------------------------------------------------------------------
// SEQUENTIAL WRITE BENCHMARK (For completeness and comparison)
// -------------------------------------------------------------------------

void BM_write_chunked(benchmark::State& state) {
    const size_t chunk_size = state.range(0); 
    const size_t num_chunks = TOTAL_MEMORY_SIZE / chunk_size;

    void *memory;
    if (::posix_memalign(&memory, 64, TOTAL_MEMORY_SIZE) != 0) {
        state.SkipWithError("Memory allocation failed");
        return;
    }

    char* fill_chunk = (char*)malloc(chunk_size);
    if (!fill_chunk) { ::free(memory); state.SkipWithError("Fill buffer allocation failed"); return; }
    ::memset(fill_chunk, 0xab, chunk_size); 
    
    char* current_ptr;
    
    for (auto _ : state) {
        current_ptr = static_cast<char*>(memory);
        
        for (size_t i = 0; i < num_chunks; ++i) {
            std::memcpy(current_ptr, fill_chunk, chunk_size);
            current_ptr += chunk_size;
        }
        
        benchmark::DoNotOptimize(memory); 
    }
    
    state.SetBytesProcessed(state.iterations() * TOTAL_MEMORY_SIZE);
    
    ::free(memory);
    ::free(fill_chunk);
}


// -------------------------------------------------------------------------
// SEQUENTIAL READ BENCHMARK (NEW)
// -------------------------------------------------------------------------

void BM_read_chunked(benchmark::State& state) {
    const size_t chunk_size = state.range(0);
    const size_t num_chunks = TOTAL_MEMORY_SIZE / chunk_size;
    
    // 1. Setup: Allocate and initialize the memory block
    void *memory;
    if (::posix_memalign(&memory, 64, TOTAL_MEMORY_SIZE) != 0) {
        state.SkipWithError("Memory allocation failed");
        return;
    }
    
    // Fill the memory block once before starting the benchmark to ensure data is present
    ::memset(memory, 0xab, TOTAL_MEMORY_SIZE);

    // 2. Allocate a buffer to read the data INTO (the sink)
    char* read_buffer = (char*)malloc(chunk_size);
    if (!read_buffer) {
        ::free(memory);
        state.SkipWithError("Read buffer allocation failed");
        return;
    }

    char* current_ptr;

    // 3. Benchmark Loop
    for (auto _ : state) {
        current_ptr = static_cast<char*>(memory);
        
        for (size_t i = 0; i < num_chunks; ++i) {
            // Read: Copy data FROM the memory block INTO the local read_buffer
            std::memcpy(read_buffer, current_ptr, chunk_size);
            
            // Advance the pointer
            current_ptr += chunk_size;
        }
        
        // Ensure the compiler doesn't optimize away the entire loop by thinking 'read_buffer' is unused
        benchmark::DoNotOptimize(read_buffer);
        // Ensure the compiler doesn't assume 'memory' is unused if the loop is optimized
        benchmark::DoNotOptimize(memory); 
    }
    
    // Set throughput information
    state.SetBytesProcessed(state.iterations() * TOTAL_MEMORY_SIZE);

    // 4. Teardown
    ::free(memory);
    ::free(read_buffer);
}


// -------------------------------------------------------------------------
// BENCHMARK REGISTRATION
// -------------------------------------------------------------------------

BENCHMARK(BM_write_chunked) CHUNK_SIZE_ARGS;
BENCHMARK(BM_read_chunked) CHUNK_SIZE_ARGS;

BENCHMARK_MAIN();