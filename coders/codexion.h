/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:29 by kri-              #+#    #+#             */
/*   Updated: 2026/09/29 18:07:46 by kri-             ###   ########.fr       */
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

# define INPUT_ERR_MSG "Enter the correct parameters:\n\tnumber_of_coders\n\ttime_to_burnout(ms)\n\ttime_to_compile(ms)\n\t\
time_to_debug(ms)\n\ttime_to_refactor(ms)\n\tnumber_of_compiles_required\n\t\
dongle_cooldown(ms)\n\tscheduler- fifo or edf\n\0"
# define FIFO 1
# define EDF 0
# define MAX_INPUT 8

typedef struct s_input
{
	unsigned int	num_coders;
	size_t			burnout_time;
	size_t			compile_time;
	size_t			debug_time;
	size_t			num_compiles;
	size_t			cooldown_time;
	unsigned int	scheduler;
}	t_input;


typedef struct s_dongle
{
	unsigned int	cooldown_time;
	pthread_mutex_t	lock;
}	t_dongle;


typedef struct s_coder
{
	unsigned int	coder_id;
	t_dongle		*left;
	t_dongle		*right;
	unsigned int	curr_compl_count;
	long			prev_compl_start_time;
	pthread_t		thread
}	t_coder;


typedef struct s_hub
{
	t_coder		*coders;
	t_dongle	*dongles;
	t_input		input;
	pthread_t	*thread_coders;
}	t_hub;

#endif