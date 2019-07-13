#ifndef _TIMER_H
#define _TIMER_H

#define	TIMER_NUM				32
typedef void (*timer_handle_func)(void);

typedef struct timer_desc {
	const unsigned char *name;
	timer_handle_func timer_handle_fun;
}timer_desc, *ptimer_desc;

void timer_init(void);
int timer_register(const unsigned char *name, timer_handle_func timer_handle_fun);
int timer_unregister(const unsigned char *name);

#endif

