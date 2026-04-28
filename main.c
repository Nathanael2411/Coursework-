#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "waveform.h"

int main(int argc, char *argv[]) {
    // Check command-line input
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Load waveform data from CSV file
    int sample_count = 0;
    WaveformSample *samples = load_data(argv[1], &sample_count);

    if (samples == NULL) {
        printf("Failed to load data.\n");
        return 1;
    }

    // Print a small preview of the loaded data
    printf("Loaded %d samples\n", sample_count);
    printf("First 3 rows:\n");

    WaveformSample *ptr = samples;
    for (int i = 0; i < 3 && i < sample_count; i++) {
        printf("t=%.4f A=%.2f B=%.2f C=%.2f\n",
               (ptr + i)->timestamp,
               (ptr + i)->phase_A_voltage,
               (ptr + i)->phase_B_voltage,
               (ptr + i)->phase_C_voltage);
    }

    // Compute RMS voltage
    double rms_a = compute_rms(samples, sample_count, 'A');
    double rms_b = compute_rms(samples, sample_count, 'B');
    double rms_c = compute_rms(samples, sample_count, 'C');

    printf("Phase A RMS = %.2f V\n", rms_a);
    printf("Phase B RMS = %.2f V\n", rms_b);
    printf("Phase C RMS = %.2f V\n", rms_c);

    // Compute peak-to-peak voltage
    double p2p_a = compute_peak_to_peak(samples, sample_count, 'A');
    double p2p_b = compute_peak_to_peak(samples, sample_count, 'B');
    double p2p_c = compute_peak_to_peak(samples, sample_count, 'C');

    printf("Phase A Peak-to-Peak = %.2f V\n", p2p_a);
    printf("Phase B Peak-to-Peak = %.2f V\n", p2p_b);
    printf("Phase C Peak-to-Peak = %.2f V\n", p2p_c);

    // Compute DC offset
    double dc_a = compute_dc_offset(samples, sample_count, 'A');
    double dc_b = compute_dc_offset(samples, sample_count, 'B');
    double dc_c = compute_dc_offset(samples, sample_count, 'C');

    printf("Phase A DC Offset = %.6f V\n", dc_a);
    printf("Phase B DC Offset = %.6f V\n", dc_b);
    printf("Phase C DC Offset = %.6f V\n", dc_c);

    // Detect clipped samples
    int clip_a = count_clipped(samples, sample_count, 'A', 324.9);
    int clip_b = count_clipped(samples, sample_count, 'B', 324.9);
    int clip_c = count_clipped(samples, sample_count, 'C', 324.9);

    printf("Phase A Clipped Samples = %d\n", clip_a);
    printf("Phase B Clipped Samples = %d\n", clip_b);
    printf("Phase C Clipped Samples = %d\n", clip_c);
    printf("Total Clipped Samples = %d\n", clip_a + clip_b + clip_c);

    // Check compliance against nominal voltage
    int comp_a = check_compliance(rms_a, 230.0);
    int comp_b = check_compliance(rms_b, 230.0);
    int comp_c = check_compliance(rms_c, 230.0);

    printf("Phase A Compliance = %s\n", comp_a ? "COMPLIANT" : "OUT OF TOLERANCE");
    printf("Phase B Compliance = %s\n", comp_b ? "COMPLIANT" : "OUT OF TOLERANCE");
    printf("Phase C Compliance = %s\n", comp_c ? "COMPLIANT" : "OUT OF TOLERANCE");

    // Compute additional system metrics
    double mean_freq = compute_mean_frequency(samples, sample_count);
    double mean_pf = compute_mean_power_factor(samples, sample_count);
    double mean_thd = compute_mean_thd(samples, sample_count);
    double mean_current = compute_mean_line_current(samples, sample_count);

    printf("Mean Frequency = %.6f Hz\n", mean_freq);
    printf("Mean Power Factor = %.6f\n", mean_pf);
    printf("Mean THD = %.6f %%\n", mean_thd);
    printf("Mean Line Current = %.6f A\n", mean_current);

    // Write final report
    write_results("results.txt",
                  rms_a, rms_b, rms_c,
                  p2p_a, p2p_b, p2p_c,
                  dc_a, dc_b, dc_c,
                  clip_a, clip_b, clip_c,
                  comp_a, comp_b, comp_c,
                  mean_freq, mean_pf, mean_thd,
                  mean_current);

    printf("Results written to results.txt\n");

    // Free allocated memory
    free(samples);
    return 0;
}