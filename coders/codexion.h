/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:29 by kri-              #+#    #+#             */
/*   Updated: 2026/09/23 20:20:04 by kri-             ###   ########.fr       */
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

# define ERROR_MSG "Enter the correct input:\nnumber_of_coders\ntime_to_burnout(ms)\ntime_to_compile(ms)\n\
time_to_debug(ms)\ntime_to_refactor(ms)\nnumber_of_compiles_required\n\
dongle_cooldown(ms)\nscheduler - fifo or edf\0"
# define FIFO 1
# define EDF 0
# define MAX_INPUT 8

typedef struct s_input
{
	unsigned int	num_coders;
	unsigned int	burnout_time;
	unsigned int	compile_time;
	unsigned int	debug_time;
	unsigned int	num_compiles;
	unsigned int	cooldown_time;
	unsigned int	scheduler;
}	t_input;


typedef struct s_coder
{
	unsigned int	coder_id;
	t_dongle		*left;
	t_dongle		*right;
	unsigned int	compilation_count;
}	t_coder;

typedef struct s_dongle
{
	unsigned int	cooldown_time;
	pthread_mutex_t	lock;
}	t_dongle;

typedef struct s_hub
{
	t_coder		*coders;
	t_dongle	*dongles;
	t_input		input;
	pthread_t	*thread_coders;
}	t_hub;

#endif