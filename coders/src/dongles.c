/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:41:35 by kri-              #+#    #+#             */
/*   Updated: 2026/11/04 19:57:53 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_init(t_uint id, t_dongle *dongle, t_input *input)
{
	dongle->dongle_id = id;
	dongle->usedby_coder = 0;
	dongle->cooldown_time = input->cooldown_time;
	dongle->scheduler = input->scheduler;
	dongle->released_time = 0;
	dongle->request_count = 0;
	memset(&dongle->heap, 0, sizeof(t_prioQ));
}

/*
	//check if dongle is already being used or 
	//cool_down is not passed or 
	//this coder does not match the dongle request
*/
static t_state	wait_for_dongle(t_dongle *d, t_uint coder_id)
{
	t_ulong			endtime;
	struct timespec	timeout;

	endtime = d->released_time + d->cooldown_time;
	while (get_simulation_status(d->simctrl) == RUN
		&& (d->usedby_coder
			|| (get_time_ms() < endtime)
			|| prioq_peek(&d->heap) != coder_id))
	{
		set_timeout(&timeout, endtime);
		if (!d->usedby_coder && get_time_ms() < endtime
			&& prioq_peek(&d->heap) == coder_id)
		{
			pthread_cond_timedwait(&d->cond_dongle, &d->mutex_dongle,
				&timeout);
		}
		else
			pthread_cond_wait(&d->cond_dongle, &d->mutex_dongle);
		endtime = d->released_time + d->cooldown_time;
	}
	return (get_simulation_status(d->simctrl));
}

t_uint	assign_dongle(t_uint coder_id, t_dongle *dongle, t_ulong deadline)
{
	t_slotreq	request;
	t_state		sim_status;

	pthread_mutex_lock(&dongle->mutex_dongle);
	request.coder_id = coder_id;
	if (dongle->scheduler == fifo)
		request.priority = dongle->request_count++;
	else
		request.priority = deadline;
	if (!prioq_push(&dongle->heap, &request))
		return (0);
	pthread_cond_broadcast(&dongle->cond_dongle);
	sim_status = wait_for_dongle(dongle, coder_id);
	if (sim_status == RUN)
		dongle->usedby_coder = prioq_pop(&dongle->heap);
	else
		prioq_remove(&dongle->heap, &request);
	pthread_mutex_unlock(&dongle->mutex_dongle);
	return (sim_status);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex_dongle);
	dongle->released_time = get_time_ms();
	dongle->usedby_coder = 0;
	pthread_cond_broadcast(&dongle->cond_dongle);
	pthread_mutex_unlock(&dongle->mutex_dongle);
}
