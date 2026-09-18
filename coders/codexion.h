/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 15:01:29 by kri-              #+#    #+#             */
/*   Updated: 2026/09/07 17:49:01 by kri-             ###   ########.fr       */
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
# define MAX_INPUT 7

typedef struct s_input
{
	unsigned int		num_coders;
	unsigned int		burnout_time;
	unsigned int		compile_time;
	unsigned int		debug_time;
	unsigned int		num_compiles;
	unsigned int		cooldown_time;
	unsigned int		scheduler;
}	t_input;

#endif