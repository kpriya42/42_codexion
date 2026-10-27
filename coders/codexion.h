/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:29 by kri-              #+#    #+#             */
/*   Updated: 2026/10/27 16:32:16 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>

# define INPUT_ERR_MSG "Enter the correct parameters:\n\tnumber_of_coders(max 200)\n\ttime_to_burnout(ms)\n\t\
time_to_compile(ms)\n\ttime_to_debug(ms)\n\ttime_to_refactor(ms)\n\tnumber_of_compiles_required\n\t\
dongle_cooldown(ms)\n\tscheduler- fifo or edf\n"
# define INIT_ERR "Initialization failed\n"
# define MTX_INIT_ERR "Mutex init failed\n"
//# define FIFO 1
//# define EDF 0
# define MAX_INPUT 8
# define MAX_CODERS 200
# define TO_MICROSEC 1000
# define HALF_MS 500
# define TO_MS 1000 
# define TO_SECONDS 1000
# define MAX_DONGLES_PER_CODER 2

typedef unsigned long	t_ulong;
typedef unsigned int	t_uint;
typedef pthread_mutex_t	t_mutex;
typedef pthread_cond_t	t_mtxcond;

typedef enum e_simstate
{
	INIT,
	RUN,
	STOP
}	t_state;

typedef enum e_scheduler
{
	fifo,
	edf
}	t_scheduler;

typedef enum e_coderstate
{
	compile,
	debug,
	refactor,
	end
}	t_cstate;

typedef struct s_input
{
	t_uint		num_coders;
	t_ulong		burnout_time;
	t_ulong		compile_time;
	t_ulong		debug_time;
	t_ulong		refactor_time;
	t_ulong		num_compiles;
	t_ulong		cooldown_time;
	t_scheduler	scheduler;
}	t_input;

typedef struct s_slotrequest
{
	t_uint	coder_id;
	//t_uint	fifo_priority;
	t_ulong	priority;
}	t_slotreq;

typedef struct s_prioQ
{
	t_slotreq	slots[MAX_DONGLES_PER_CODER];
}	t_prioQ;

typedef struct s_simcontrol
{
	t_mutex		mutex_sim;
	t_mtxcond	cond_sim;
	t_state		sim_status;
}	t_simctrl;

typedef struct s_dongle
{
	t_uint		dongle_id;
	t_uint		usedby_coder;
	t_ulong		cooldown_time;
	t_scheduler	scheduler;
	t_ulong		released_time;
	t_uint		request_count;
	t_mutex		mutex_dongle;
	t_mtxcond	cond_dongle;
	t_prioQ		heap;
	t_simctrl	*simctrl;
}	t_dongle;

typedef struct s_logger
{
	t_mutex		mutex_print;
	t_ulong		start_time;
}	t_logger;

typedef struct s_coder
{
	t_uint		id;
	pthread_t	thread_coder;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_uint		curr_compl_count;
	t_ulong		prev_compl_start_time;
	t_uint		max_compiles;
	t_mutex		mutex_coder;
	bool		has_finished;

	t_ulong		burnout_time;
	t_ulong		compile_time;
	t_ulong		debug_time;
	t_ulong		refactor_time;

	t_logger	*logger;
	t_simctrl	*simctrl;
	t_mtxcond	*cond_wait;
}	t_coder;

typedef struct s_hub
{
	t_input		input;

	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	thread_monitor;

	t_logger	logger;
	t_simctrl	simctrl;
	//t_mtxcond	cond_sleep;
}	t_hub;

// initialize coders, dongles, mutexes
void	initialize_hub(t_hub *hub);
// create coder and monitor threads and set simulation start
void	create_threads_start_sim(t_hub *hub);
// join threads
void	join_threads(t_hub *hub);
// delete threads, memory, mutexes
void	free_all(t_hub *hub, t_uint num_coders);
//free malloc
void	free_mem(t_hub *hub);

void	dongle_init(t_uint id, t_dongle *dongle, t_input *input);
bool	assign_dongle(t_uint coder_id, t_dongle *dongle, t_ulong deadline);
void	release_dongle(t_dongle *dongle);

void	coder_init(t_coder *coder, t_input *input, t_dongle *ldongle,\
													t_dongle *rdongle);
void	*coder_routine(void *arg);
bool	request_dongles(t_coder *coder, t_ulong	deadline);
void	release_dongles(t_coder *coder);
t_state	get_simulation_status(t_simctrl *simctrl);
bool	get_coder_finish_status(t_coder *coder);

void	prioq_push(t_prioQ *heap, t_slotreq *request);
t_uint	prioq_peek(t_prioQ *heap);
t_uint	prioq_pop(t_prioQ *heap);
void	prioq_remove(t_prioQ *heap, t_slotreq *request);

void	*monitor_routine(void *arg);

void	display_err(char *msg);
t_ulong	get_time_ms(void);
void	set_timeout(struct timespec *timeout, t_ulong end_time);
t_state	sleep_ms(t_simctrl *simctrl, t_ulong duration);

void	log_event(t_uint id, const char *event, t_logger *logger);

#endif