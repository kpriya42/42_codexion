/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 17:07:29 by kri-              #+#    #+#             */
/*   Updated: 2026/10/24 15:57:53 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_finished(t_coder *coder, t_cstate *nextstate)
{
	if (coder->curr_compl_count == coder->max_compiles)
	{
		pthread_mutex_lock(&coder->mutex_coder_state);
		coder->has_finished = true;
		*nextstate = end;
		pthread_mutex_unlock(&coder->mutex_coder_state);
	}
	else
		*nextstate = compile;
}

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
	sleep_ms(coder, coder->compile_time);
	coder->curr_compl_count++;
	release_dongles(coder);
	*nextstate = debug;
}

static void	do_debug(t_coder *coder, t_cstate *nextstate)
{
	log_event(coder->coder_id, "is debugging", coder->logger);
	sleep_ms(coder, coder->debug_time);
	*nextstate = refactor;
}

static void	do_refactor(t_coder *coder, t_cstate *nextstate)
{
	log_event(coder->coder_id, "is refactoring", coder->logger);
	sleep_ms(coder, coder->refactor_time);
	set_finished(coder, nextstate);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_cstate	nextstate;

	coder = (t_coder *) arg;
	nextstate = compile;
	pthread_mutex_lock(coder->mutex_sim);
	while (*coder->sim_status == INIT)
		pthread_cond_wait(coder->cond_sim, coder->mutex_sim);
	pthread_mutex_unlock(coder->mutex_sim);
	while (get_simulation_status(coder) == RUN
		&& !get_coder_finish_status(coder))
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
