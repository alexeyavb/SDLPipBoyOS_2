#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <time.h>
#include "common_defs.h"
#include "common.h"
#include "fps.h"

const double COMMON_FPS_GRANULARITY_S = 0.5;
double common_fps_last_time_s;
unsigned int common_fps_nframes;

double common_get_secs(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ts.tv_sec + (1e-9 * ts.tv_nsec);
}

void common_fps_init(void) {
    common_fps_nframes = 0;
    common_fps_last_time_s = common_get_secs();
}

void common_fps_update_and_print(void) {
    double dt, current_time_s;
    current_time_s = common_get_secs();
    common_fps_nframes++;
    dt = current_time_s - common_fps_last_time_s;
    if (dt > COMMON_FPS_GRANULARITY_S) {
        printf("FPS = %f\n", common_fps_nframes / dt);
        common_fps_last_time_s = current_time_s;
        common_fps_nframes = 0;
    }
}
