//
// Created by Oshane Nathanael on 27/04/2026.
//


#include <stdio.h>
#include <math.h>
#include "waveform.h"

double compute_rms(WaveformSample *samples, int count, char phase) {
    double sum_sq = 0.0;
    WaveformSample *ptr = samples;
    double voltage = 0.0;

    for (int i = 0; i < count; i++) {
        if (phase == 'A') {
            voltage = (ptr + i)->phase_A_voltage;
        } else if (phase == 'B') {
            voltage = (ptr + i)->phase_B_voltage;
        } else if (phase == 'C') {
            voltage = (ptr + i)->phase_C_voltage;
        } else {
            return -1.0;
        }

        sum_sq += voltage * voltage;
    }

    return sqrt(sum_sq / count);
}

double compute_peak_to_peak(WaveformSample *samples, int count, char phase) {
    if (count <= 0) {
        return -1.0;
    }

    WaveformSample *ptr = samples;
    double voltage = 0.0;
    double max_val = 0.0;
    double min_val = 0.0;

    if (phase == 'A') {
        max_val = min_val = ptr->phase_A_voltage;
    } else if (phase == 'B') {
        max_val = min_val = ptr->phase_B_voltage;
    } else if (phase == 'C') {
        max_val = min_val = ptr->phase_C_voltage;
    } else {
        return -1.0;
    }

    for (int i = 0; i < count; i++) {
        if (phase == 'A') {
            voltage = (ptr + i)->phase_A_voltage;
        } else if (phase == 'B') {
            voltage = (ptr + i)->phase_B_voltage;
        } else {
            voltage = (ptr + i)->phase_C_voltage;
        }

        if (voltage > max_val) {
            max_val = voltage;
        }

        if (voltage < min_val) {
            min_val = voltage;
        }
    }

    return max_val - min_val;
}

double compute_dc_offset(WaveformSample *samples, int count, char phase) {
    if (count <= 0) {
        return -1.0;
    }

    WaveformSample *ptr = samples;
    double sum = 0.0;
    double voltage = 0.0;

    for (int i = 0; i < count; i++) {
        if (phase == 'A') {
            voltage = (ptr + i)->phase_A_voltage;
        } else if (phase == 'B') {
            voltage = (ptr + i)->phase_B_voltage;
        } else if (phase == 'C') {
            voltage = (ptr + i)->phase_C_voltage;
        } else {
            return -1.0;
        }

        sum += voltage;
    }

    return sum / count;
}

int count_clipped(WaveformSample *samples, int count, char phase, double limit) {
    int clipped_count = 0;
    WaveformSample *ptr = samples;
    double voltage = 0.0;

    for (int i = 0; i < count; i++) {
        if (phase == 'A') {
            voltage = (ptr + i)->phase_A_voltage;
        } else if (phase == 'B') {
            voltage = (ptr + i)->phase_B_voltage;
        } else if (phase == 'C') {
            voltage = (ptr + i)->phase_C_voltage;
        } else {
            return -1;
        }

        if (fabs(voltage) >= limit) {
            clipped_count++;
        }
    }

    return clipped_count;
}

int check_compliance(double rms, double nominal) {
    double lower_limit = nominal * 0.90;
    double upper_limit = nominal * 1.10;

    if (rms >= lower_limit && rms <= upper_limit) {
        return 1;
    }

    return 0;
}

double compute_mean_frequency(WaveformSample *samples, int count) {
    if (count <= 0) {
        return -1.0;
    }

    double sum = 0.0;
    WaveformSample *ptr = samples;

    for (int i = 0; i < count; i++) {
        sum += (ptr + i)->frequency;
    }

    return sum / count;
}

double compute_mean_power_factor(WaveformSample *samples, int count) {
    if (count <= 0) {
        return -1.0;
    }

    double sum = 0.0;
    WaveformSample *ptr = samples;

    for (int i = 0; i < count; i++) {
        sum += (ptr + i)->power_factor;
    }

    return sum / count;
}

double compute_mean_thd(WaveformSample *samples, int count) {
    if (count <= 0) {
        return -1.0;
    }

    double sum = 0.0;
    WaveformSample *ptr = samples;

    for (int i = 0; i < count; i++) {
        sum += (ptr + i)->thd_percent;
    }

    return sum / count;
}

double compute_mean_line_current(WaveformSample *samples, int count) {
    if (count <= 0) {
        return -1.0;
    }

    double sum = 0.0;
    WaveformSample *ptr = samples;

    for (int i = 0; i < count; i++) {
        sum += (ptr + i)->line_current;
    }

    return sum / count;
}