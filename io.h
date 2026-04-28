//
// Created by Oshane Nathanael on 27/04/2026.
//


#ifndef IO_H
#define IO_H

#include "waveform.h"

int count_rows(const char *filename);
WaveformSample *load_data(const char *filename, int *sample_count);

void write_results(const char *filename,
                   double rms_a, double rms_b, double rms_c,
                   double p2p_a, double p2p_b, double p2p_c,
                   double dc_a, double dc_b, double dc_c,
                   int clip_a, int clip_b, int clip_c,
                   int comp_a, int comp_b, int comp_c,
                   double mean_freq, double mean_pf, double mean_thd,
                   double mean_current);

#endif


