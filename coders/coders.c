/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 17:07:29 by kri-              #+#    #+#             */
/*   Updated: 2026/10/18 22:10:37 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	do_compile(t_coder *coder, t_cstate *nextstate)
{
	t_ulong	deadline;

	deadline = coder->prev_compl_start_time + coder->burnout_time;
	if (!request_dongles(coder, deadline))
		return ;
	pthread_mutex_lock(&coder->mutex_coder_state);
	coder->prev_compl_start_time = get_time_ms();
	pthread_mutex_unlock(&coder->mutex_coder_state);
	log_event(coder->coder_id, "is compiling", coder->logger);
	usleep(coder->compile_time * TO_MICROSEC);
	pthread_mutex_lock(&coder->mutex_coder_state);
	coder->curr_compl_count++;
	pthread_mutex_unlock(&coder->mutex_coder_state);
	release_dongles(coder);
	*nextstate = debug;
}

static void	do_debug(t_coder *coder, t_cstate *nextstate)
{
	log_event(coder->coder_id, "is debugging", coder->logger);
	usleep(coder->debug_time * TO_MICROSEC);
	*nextstate = refactor;
}

static void	do_refactor(t_coder *coder, t_cstate *nextstate)
{
	log_event(coder->coder_id, "is refactoring", coder->logger);
	usleep(coder->refactor_time * TO_MICROSEC);
	set_finished(coder, nextstate);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_cstate	nextstate;

	coder = (t_coder *) arg;
	nextstate = compile;
	pthread_mutex_lock(coder->mutex_start);
	while (!*coder->sim_start)
		pthread_cond_wait(coder->cond_start, coder->mutex_start);
	pthread_mutex_unlock(coder->mutex_start);
	while (get_simulation_status(coder) && !get_coder_finish_status(coder))
	{
		if (nextstate == compile)
			do_compile(coder, &nextstate);
		else if (nextstate == debug)
			do_debug(coder, &nextstate);
		else if (nextstate == refactor)
			do_refactor(coder, &nextstate);
	}
	return (NULL);
}

void	coder_init(t_coder *coder, t_input *input, t_dongle *ldongle,
	t_dongle *rdongle)
{
	coder->left_dongle = ldongle;
	coder->right_dongle = rdongle;
	coder->curr_compl_count = 0;
	coder->max_compiles = input->num_compiles;
	coder->has_finished = false;
	coder->burnout_time = input->burnout_time;
	coder->compile_time = input->compile_time;
	coder->debug_time = input->debug_time;
	coder->refactor_time = input->refactor_time;
}
