#ifndef EVENT_H
#define EVENT_H

#include <sys/time.h>

enum event_flag {
    EVENT_READ      = 0x01,
    EVENT_WRITE     = 0x02,

    EVENT_TIMEOUT   = 0x08,
};

/*
 * Maximum stack size for event_task's.
 *
 * This is allocated by event_start using malloc(), and will never grow.
 * However, perhaps we can rely on Linux's lazy malloc() page allocation..
 */
#define EVENT_TASK_SIZE 65536


/*
 * IO reactor.
 */
struct event_main;
struct event;
struct event_task;

typedef void (event_task_func)(void *ctx);

/*
 * Prepare a new event_main for use; initially empty.
 */
int event_main_create (struct event_main **event_mainp);

/*
 * Prepare a new event for use; initially inactive.
 */
int event_create (struct event_main *event_main, struct event **eventp, int fd);

/*
 * Boot up the given event task.
 *
 * The task will be cleaned up once the func returns.
 */
int _event_start (struct event_main *event_main, const char *name, event_task_func *func, void *ctx);
#define event_start(event_main, func, ctx) _event_start(event_main, #func, func, ctx)

/*
 * Yield execution on the given event.
 *
 *  flags:          some combination of EVENT_READ|EVENT_WRITE.
 *  timeout:        relative timeout until returning 1 for timeout.
 *
 * Returns 0 on success (event happaned), 1 on timeout (event did not happen), <0 on error.
 */
int event_yield (struct event *event, int flags, const struct timeval *timeout);
   
/*
 * Deactivate and release resources.
 */
void event_destroy (struct event *event);

/*
 * Mainloop, runs while there are active events.
 */
int event_main_run (struct event_main *event_main);

#endif
