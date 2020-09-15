#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

void oct_wrapper_run(const char* funcname, float* input, float* output, int n, float param);
void oct_wrapper_init();

#ifdef __cplusplus
     }
#endif