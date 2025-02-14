#ifndef __FPS_H__
#define __FPS_H__

extern const double COMMON_FPS_GRANULARITY_S;
extern double common_fps_last_time_s;
extern unsigned int common_fps_nframes;
extern void common_fps_init(void);
extern void common_fps_update_and_print(void);
extern double common_get_secs(void);

#endif //__FPS_H__
