#include <stdio.h>
#include <stdlib.h>
#include "io.h"

int count_rows(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return -1;
    }

    char line[512];
    int count = 0;

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        return -1;
    }

    while (fgets(line, sizeof(line), file)) {
        count++;
    }

    fclose(file);
    return count;
}

WaveformSample *load_data(const char *filename, int *sample_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    int count = count_rows(filename);
    if (count <= 0) {
        fclose(file);
        return NULL;
    }

    WaveformSample *samples = malloc(count * sizeof(WaveformSample));
    if (samples == NULL) {
        fclose(file);
        return NULL;
    }

    char line[512];

    if (fgets(line, sizeof(line), file) == NULL) {
        free(samples);
        fclose(file);
        return NULL;
    }

    int i = 0;
    while (fgets(line, sizeof(line), file) && i < count) {
        int parsed = sscanf(line, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
                            &samples[i].timestamp,
                            &samples[i].phase_A_voltage,
                            &samples[i].phase_B_voltage,
                            &samples[i].phase_C_voltage,
                            &samples[i].line_current,
                            &samples[i].frequency,
                            &samples[i].power_factor,
                            &samples[i].thd_percent);

        if (parsed == 8) {
            i++;
        }
    }

    fclose(file);

    *sample_count = i;

    if (i == 0) {
        free(samples);
        return NULL;
    }

    return samples;
}

void write_results(const char *filename,
                   double rms_a, double rms_b, double rms_c,
                   double p2p_a, double p2p_b, double p2p_c,
                   double dc_a, double dc_b, double dc_c,
                   int clip_a, int clip_b, int clip_c,
                   int comp_a, int comp_b, int comp_c,
                   double mean_freq, double mean_pf, double mean_thd,
                   double mean_current) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to write results file.\n");
        return;
    }

    fprintf(file, "POWER QUALITY ANALYSIS REPORT\n");
    fprintf(file, "=============================\n\n");

    fprintf(file, "1. Voltage Metrics\n");
    fprintf(file, "------------------\n");
    fprintf(file, "Phase A RMS           = %.2f V\n", rms_a);
    fprintf(file, "Phase B RMS           = %.2f V\n", rms_b);
    fprintf(file, "Phase C RMS           = %.2f V\n\n", rms_c);

    fprintf(file, "Phase A Peak-to-Peak  = %.2f V\n", p2p_a);
    fprintf(file, "Phase B Peak-to-Peak  = %.2f V\n", p2p_b);
    fprintf(file, "Phase C Peak-to-Peak  = %.2f V\n\n", p2p_c);

    fprintf(file, "Phase A DC Offset     = %.6f V\n", dc_a);
    fprintf(file, "Phase B DC Offset     = %.6f V\n", dc_b);
    fprintf(file, "Phase C DC Offset     = %.6f V\n\n", dc_c);

    fprintf(file, "2. Clipping Detection\n");
    fprintf(file, "---------------------\n");
    fprintf(file, "Phase A Clipped Samples = %d\n", clip_a);
    fprintf(file, "Phase B Clipped Samples = %d\n", clip_b);
    fprintf(file, "Phase C Clipped Samples = %d\n", clip_c);
    fprintf(file, "Total Clipped Samples   = %d\n\n", clip_a + clip_b + clip_c);

    fprintf(file, "3. Compliance Check\n");
    fprintf(file, "-------------------\n");
    fprintf(file, "Phase A Compliance  = %s\n", comp_a ? "COMPLIANT" : "OUT OF TOLERANCE");
    fprintf(file, "Phase B Compliance  = %s\n", comp_b ? "COMPLIANT" : "OUT OF TOLERANCE");
    fprintf(file, "Phase C Compliance  = %s\n\n", comp_c ? "COMPLIANT" : "OUT OF TOLERANCE");

    fprintf(file, "4. Additional System Metrics\n");
    fprintf(file, "----------------------------\n");
    fprintf(file, "Mean Frequency      = %.6f Hz\n", mean_freq);
    fprintf(file, "Mean Power Factor   = %.6f\n", mean_pf);
    fprintf(file, "Mean THD            = %.6f %%\n", mean_thd);
    fprintf(file, "Mean Line Current   = %.6f A\n", mean_current);

    fclose(file);
}