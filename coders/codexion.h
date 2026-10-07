/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:29 by kri-              #+#    #+#             */
/*   Updated: 2026/10/07 15:50:18 by kri-             ###   ########.fr       */
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
# include <stdlib.h>
# include <string.h>

# define INPUT_ERR_MSG "Enter the correct parameters:\n\tnumber_of_coders(max 200)\n\ttime_to_burnout(ms)\n\t\
time_to_compile(ms)\n\ttime_to_debug(ms)\n\ttime_to_refactor(ms)\n\tnumber_of_compiles_required\n\t\
dongle_cooldown(ms)\n\tscheduler- fifo or edf\n"
# define INIT_ERR "Initialization failed\n"
# define MTX_INIT_ERR "Mutext init failed\n"
//# define FIFO 1
//# define EDF 0
# define MAX_INPUT 8
# define MAX_CODERS 200
# define TO_SECONDS 1000
# define TO_MS 1000

typedef unsigned long	t_ulong;
typedef unsigned int	t_uint;

typedef enum e_scheduler
{
	fifo,
	edf
}	t_scheduler;

typedef struct s_input
{
	t_uint		num_coders;
	t_ulong		burnout_time;
	t_ulong		compile_time;
	t_ulong		debug_time;
	t_ulong		num_compiles;
	t_ulong		cooldown_time;
	t_scheduler	scheduler;
}	t_input;

typedef struct s_dongle
{
	t_uint			dongle_id;
	t_ulong			cooldown_time;
	t_scheduler		scheduler;
	bool			is_being_used;
	t_ulong			released_time;
	pthread_mutex_t	mutex_dongle_state;
}	t_dongle;


typedef struct s_coder
{
	t_uint		coder_id;
	pthread_t	thread_coder;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_uint		curr_compl_count;
	t_ulong		prev_compl_start_time;
	t_ulong		burnout_time;
	t_ulong		compile_time;
	t_ulong		debug_time;
}	t_coder;

typedef struct s_log
{

}	t_log;

typedef struct s_hub
{
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		thread_monitor;
	pthread_mutex_t	mutex_print;
}	t_hub;

// initialize coders, dongles, mutexes
void	initialize_hub(t_hub *hub, t_input *input);
// create coder_threads
void	create_threads(t_hub *hub, t_input *input);
//
void	join_threads(t_hub *hub, t_input *input);
// delete threads, memory, mutexes
void	free_and_exit(t_hub *hub, t_input *input);


void	dongle_init(t_uint id, t_dongle *dongle, t_input *input);

void	coder_init(t_coder *coder, t_input *input, t_dongle *ldongle,\
													t_dongle *rdongle);

void	create_coder_threads(pthread_t *thread_coder, t_coder *coder);
void	*coder_routine(void *arg);

void	*monitor_routine(void *arg);

void	display_err(char *msg);
t_ulong	get_time_ms(void);

void	log_event(t_uint coder_id, const char *event, t_hub *hub);


#endif