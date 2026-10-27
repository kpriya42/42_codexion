/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 17:07:29 by kri-              #+#    #+#             */
/*   Updated: 2026/10/27 15:58:03 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	set_finished(t_coder *coder, t_cstate *nextstate)
{
	if (coder->curr_compl_count >= coder->max_compiles)
	{
		pthread_mutex_lock(&coder->mutex_coder);
		coder->has_finished = true;
		*nextstate = end;
		pthread_mutex_unlock(&coder->mutex_coder);
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
	pthread_mutex_lock(&coder->mutex_coder);
	coder->prev_compl_start_time = get_time_ms();
	pthread_mutex_unlock(&coder->mutex_coder);
	log_event(coder->id, "is compiling", coder->logger);
	if (sleep_ms(coder->simctrl, coder->compile_time) == RUN)
	{
		*nextstate = debug;
		pthread_mutex_lock(&coder->mutex_coder);
		coder->curr_compl_count++;
		pthread_mutex_unlock(&coder->mutex_coder);
	}
	release_dongles(coder);
}

static void	do_debug(t_coder *coder, t_cstate *nextstate)
{
	log_event(coder->id, "is debugging", coder->logger);
	if (sleep_ms(coder->simctrl, coder->debug_time) == RUN)
		*nextstate = refactor;
}

static void	do_refactor(t_coder *coder, t_cstate *nextstate)
{
	log_event(coder->id, "is refactoring", coder->logger);
	if (sleep_ms(coder->simctrl, coder->refactor_time) == RUN)
		set_finished(coder, nextstate);
}

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	t_cstate	nextstate;

	coder = (t_coder *) arg;
	nextstate = compile;
	pthread_mutex_lock(&coder->simctrl->mutex_sim);
	while (coder->simctrl->sim_status == INIT)
		pthread_cond_wait(&coder->simctrl->cond_sim,
			&coder->simctrl->mutex_sim);
	pthread_mutex_unlock(&coder->simctrl->mutex_sim);
	while (get_simulation_status(coder->simctrl) == RUN
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
