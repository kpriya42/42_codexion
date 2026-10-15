/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 17:07:29 by kri-              #+#    #+#             */
/*   Updated: 2026/10/14 19:18:17 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_init(t_coder *coder, t_input *input, t_dongle *ldongle,
	t_dongle *rdongle)
{
	coder->left_dongle = ldongle;
	coder->right_dongle = rdongle;
	coder->curr_compl_count = 0;
	coder->has_finished = false;
	coder->prev_compl_start_time = *coder->start_time;
	coder->burnout_time = input->burnout_time;
	coder->compile_time = input->compile_time;
	coder->debug_time = input->debug_time;
	coder->refactor_time = input->refactor_time;
}

static void	start_routine(t_coder *coder)
{
	pthread_mutex_lock(coder->mutex_start);
	while (!*coder->sim_start)
		pthread_cond_wait(coder->cond_start, coder->mutex_start);
	pthread_mutex_unlock(coder->mutex_start);
}

static void	compile(t_coder *coder, t_cstate *nextstate)
{
	nextstate = debug;
}

void	*coder_routine(void *arg)
{
	t_coder			*coder;
	t_ulong			tstamp;
	static t_cstate	nextstate;

	coder = (t_coder *) arg;
	nextstate = compile;
	start_routine(coder);
	while (*coder->sim_start && !coder->has_finished)
	{
		if (nextstate == compile)
			compile(coder, &nextstate);
		else if (nextstate == debug)
		{
			sleep(coder->debug_time);
			nextstate = refactor;
		}
		else if (nextstate == refactor)
		{
			sleep(coder->refactor_time);
			set_finished();
		}
	}
	return (NULL);
}
