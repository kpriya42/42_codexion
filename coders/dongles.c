/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:41:35 by kri-              #+#    #+#             */
/*   Updated: 2026/10/26 19:35:15 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_init(t_uint id, t_dongle *dongle, t_input *input)
{
	dongle->dongle_id = id;
	dongle->cooldown_time = input->cooldown_time;
	dongle->scheduler = input->scheduler;
	dongle->request_count = 0;
}

/*
	//check if dongle is already being used or 
	//cool_down is not passed or 
	//this coder does not match the dongle request
*/
static t_state	wait_for_dongle(t_dongle *dongle, t_uint coder_id)
{
	t_state			sim_status;
	t_ulong			endtime;
	struct timespec	timeout;

	endtime = dongle->released_time + dongle->cooldown_time;
	set_timeout(&timeout, endtime);
	sim_status = get_simulation_status(dongle->simctrl);
	while ((dongle->used_by_coder_id || (get_time_ms() < endtime)
			|| prioq_peek(&dongle->heap) != coder_id) && sim_status == RUN)
	{
		if (!dongle->used_by_coder_id && get_time_ms() < endtime)
			pthread_cond_timedwait(&dongle->cond_dongle, &dongle->mutex_dongle,
				&timeout);
		else
			pthread_cond_wait(&dongle->cond_dongle, &dongle->mutex_dongle);
		sim_status = get_simulation_status(dongle->simctrl);
	}
	return (sim_status);
}

bool	assign_dongle(t_uint coder_id, t_dongle *dongle, t_ulong deadline)
{
	t_slotreq	request;
	t_state		sim_status;

	pthread_mutex_lock(&dongle->mutex_dongle);
	request.coder_id = coder_id;
	if (dongle->scheduler == fifo)
		request.fifo_priority = dongle->request_count++;
	else
		request.edf_priority = deadline;
	prioq_push(&dongle->heap, &request);
	sim_status = wait_for_dongle(dongle, coder_id);
	if (sim_status == RUN)
		dongle->used_by_coder_id = prioq_pop(&dongle->heap);
	pthread_mutex_unlock(&dongle->mutex_dongle);
	return (sim_status);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex_dongle);
	dongle->released_time = get_time_ms();
	dongle->used_by_coder_id = 0;
	pthread_cond_broadcast(&dongle->cond_dongle);
	pthread_mutex_unlock(&dongle->mutex_dongle);
}
//bool	assign_dongle(t_coder *coder, t_dongle *dongle, t_ulong deadline)
//{
//	bool		dongle_occupied;
//	t_ulong		rel_time;
//	t_slotreq	request;

//	(void ) deadline;
//	request.coder_id = coder->coder_id;
//	if (dongle->scheduler == fifo)
//		request.fifo_priority = dongle->request_count++;
//	else
//		request.edf_priority = deadline;
//	pthread_mutex_lock(&dongle->mutex_dongle);
//	dongle_occupied = dongle->is_being_used;
//	rel_time = dongle->released_time;
//	pthread_mutex_unlock(&dongle->mutex_dongle);
//	if (dongle_occupied)
//		return (false);
//	else if ((get_time_ms() - rel_time) > dongle->cooldown_time)
//	{
//		pthread_mutex_lock(&dongle->mutex_dongle);
//		dongle->is_being_used = true;
//		pthread_mutex_unlock(&dongle->mutex_dongle);
//		return (true);
//	}
//	return (false);
//}

