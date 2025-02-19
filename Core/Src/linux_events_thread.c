#include "linux_events_thread.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <poll.h>
#include <limits.h>
#include <sys/time.h>

#include <SDL.h>
#include <SDL_thread.h>
typedef enum AXIS{
    AXIS_X = 0, AXIS_Y
} AXIS, *PAXIS, **LPAXIS;

typedef struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    int value;
} *pinput_event;
// input_event, *PINPUT_EVENT, **LPINPUT_EVENT;

static SDL_Thread *event_thread;
static bool running = false;
static struct pollfd axis[2];

int threadproc(void *data);

#define X_AXIS_DEVICE "/dev/input/event0\0"
#define Y_AXIS_DEVICE "/dev/input/event1\0"


int opendevices(void){
    axis[AXIS_X].fd = open(X_AXIS_DEVICE, O_RDONLY | O_NONBLOCK);
    axis[AXIS_Y].fd = open(Y_AXIS_DEVICE, O_RDONLY | O_NONBLOCK);

    if(axis[AXIS_X].fd < 0)
    {
        printf("error unable open for reading '%s'\n", X_AXIS_DEVICE);
        return(1);
    }

    if(axis[AXIS_Y].fd < 0)
    {
        printf("error unable open for reading '%s'\n", Y_AXIS_DEVICE);
        return(2);
    }

    axis[AXIS_X].events = POLLIN; axis[AXIS_Y].events = POLLIN;

    return(0);
}

int closedevices(void){
    if(axis[AXIS_X].fd >= 0)
    {
        printf("Close device '%s'\n", X_AXIS_DEVICE);
        close(axis[AXIS_X].fd);

    }

    if(axis[AXIS_Y].fd < 0)
    {
        printf("Close device '%s'\n", Y_AXIS_DEVICE);
        close(axis[AXIS_Y].fd);
    }

    return 0;
}


int polldevices(struct input_event* inp_data, int inp_size){

/*
	static const int inp_size = sizeof(struct input_event);
    printf("input_size=%d\n", inp_size);
	struct input_event input_data;
    struct input_event* inp_data = &input_data;
    memset(inp_data,0,inp_size);
*/

    int timeout_ms = 500;
    int retX = poll(&(axis[AXIS_X]), 1, timeout_ms);
    int retY = poll(&(axis[AXIS_Y]), 1, timeout_ms);

    if(retX > 0){
        if(axis[AXIS_X].revents){
            memset(inp_data,0,inp_size);
            ssize_t rX = read(axis[AXIS_X].fd, inp_data, inp_size);
            if(rX < 0) {
                printf(" error read x axis event data %d\n", (int)rX);                
            }
            else {
                printf("X AXIS_EVENT time=%ld.%06ld type=%hu code=%hu value=%d\n", inp_data->time.tv_sec, inp_data->time.tv_usec, inp_data->type, inp_data->code, inp_data->value);
                memset(inp_data,0,inp_size);                
            }
        }        
    }

    if(retY > 0){
        if(axis[AXIS_Y].revents){
            memset(inp_data,0,inp_size);
            ssize_t rY = read(axis[AXIS_Y].fd, inp_data, inp_size);
            if(rY < 0) {
                printf(" error read Y axis event data %d\n", (int)rY);                
            }
            else {
                printf("X AXIS_EVENT time=%ld.%06ld type=%hu code=%hu value=%d\n", inp_data->time.tv_sec, inp_data->time.tv_usec, inp_data->type, inp_data->code, inp_data->value);
                memset(inp_data,0,inp_size);                
            }
        }        
    }
    return 0;
}

#define MSG_DELAY_MSEC 2000
static int next_message = 0;

void EventThread_Init(void){
    next_message = SDL_GetTicks() + MSG_DELAY_MSEC;    
    event_thread = SDL_CreateThread(&threadproc, "eventthread", 0x00);
    if(!event_thread){
        
    }
}

int threadproc(void *data){    
    printf("Enter Thread proc callback\n");

    opendevices();


    static const int inp_size = sizeof(struct input_event);
    printf("input_size=%d\n", inp_size);
    static struct input_event input_data;
    static struct input_event* inp_data = &input_data;
    memset(inp_data,0,inp_size);

    running =true;
    while (running){
        if(SDL_GetTicks() >= next_message) {
            printf("Event Thread proc callback\n");
            next_message = SDL_GetTicks() + MSG_DELAY_MSEC;
        }
        polldevices(inp_data, inp_size);
    }
    closedevices();
    printf("Exit from Thread proc callback\n");
    return 0;
}


/*
Event pool example
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
struct input_event {
    struct timeval time;
    unsigned short type;
    unsigned short code;
    unsigned int value;
};
int main(int argc, char *argv[])
{
    const int timeout_ms = -1;
    char* input_dev = argv[1];
    int st;
    int ret;
    struct pollfd fds[1];
    fds[0].fd = open(input_dev, O_RDONLY|O_NONBLOCK);
    if(fds[0].fd<0)
    {
        printf("error unable open for reading '%s'\n",input_dev);
        return 1;
    }
    const int input_size = sizeof(struct input_event);
    printf("input_size=%d\n", input_size);
    struct input_event* input_data;
    memset(input_data,0,input_size);
    fds[0].events = POLLIN;
    while(true)
    {
        ret = poll(fds, 1, timeout_ms);
        if(ret>0)
        {
            if(fds[0].revents)
            {
                ssize_t r = read(fds[0].fd,input_data,input_size);
                if(r<0)
                {
                    printf("error %d\n",(int)r);
                    break;
                }
                else
                {
            printf("time=%ld.%06lu type=%hu code=%hu value=%u\n", input_data->time.tv_sec, input_data->time.tv_usec, input_data->type, input_data->code, input_data->value);
                    memset(input_data,0,input_size);
                }
            }
            else
            {
                printf("error\n");
            }
        }
        else
        {
            printf("timeout\n");
        }
    }
    close(fds[0].fd);
    return 0;
}

*/
