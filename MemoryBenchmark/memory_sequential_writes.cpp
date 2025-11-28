#include <cstdlib>
#include <string.h>

#include "benchmark/benchmark.h"

#define ARGS \
    ->RangeMultiplier(2)->Range(100, 2*100) \
    -> UseRealTime()

template <class Word>
void BM_write_seq(benchmark::State& state){
    void *memory;
    if (::posix_memalign(&memory, 64, state.range_x()) != 0) return;
    void* const end = static_cast<char*>(memory) + state.range_x();
    Word* const p0 = static_cast<Word*>(memory);
    Word* const p1 = static_cast<Word*>(end);
    Word fill;
    ::memset(&fill, 0xab, sizeof(fill));
    
    while (state.KeepRunning())
    {
        for (Word* p = p0; p < p1; p++){
            benchmark::DoNotOptimize(*p = fill);
        }
    }
    ::free(memory);
}

BENCHMARK_TEMPLATE(BM_write_seq, char) ARGS;
BENCHMARK_MAIN();

