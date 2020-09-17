#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

void oct_wrapper_run(const char* funcname, float** input, unsigned int ninput, float** output, unsigned int noutput, unsigned int nsamples, float param);
void oct_wrapper_init();

#ifdef __cplusplus
     }
#endif