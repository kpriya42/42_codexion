/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:41:35 by kri-              #+#    #+#             */
/*   Updated: 2026/10/27 21:10:00 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_init(t_uint id, t_dongle *dongle, t_input *input)
{
	int	index;

	index = 0;
	dongle->dongle_id = id;
	dongle->usedby_coder = 0;
	dongle->cooldown_time = input->cooldown_time;
	dongle->scheduler = input->scheduler;
	dongle->released_time = 0;
	dongle->request_count = 0;
	memset(&dongle->heap, 0, sizeof(t_prioQ));
	//while (index < 2)
	//{
	//	dongle->heap.slots[index].coder_id = 0;
	//	dongle->heap.slots[index].priority = 0;
	//	index++;
	//}
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
	sim_status = get_simulation_status(dongle->simctrl);
	while ((dongle->usedby_coder || (get_time_ms() < endtime)
			|| prioq_peek(&dongle->heap) != coder_id) && sim_status == RUN)
	{
		endtime = dongle->released_time + dongle->cooldown_time;
		set_timeout(&timeout, endtime);
		if (!dongle->usedby_coder && get_time_ms() < endtime
			&& sim_status == RUN)
		{
			pthread_cond_timedwait(&dongle->cond_dongle, &dongle->mutex_dongle,
				&timeout);
			sim_status = get_simulation_status(dongle->simctrl);
		}
		else if (sim_status == RUN)
		{
			pthread_cond_wait(&dongle->cond_dongle, &dongle->mutex_dongle);
			sim_status = get_simulation_status(dongle->simctrl);
		}
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
		request.priority = (t_ulong) dongle->request_count++;
	else
		request.priority = deadline;
	prioq_push(&dongle->heap, &request);
	wait_for_dongle(dongle, coder_id);
	sim_status = get_simulation_status(dongle->simctrl);
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
	if (dongle->usedby_coder != 0)
	{
		dongle->released_time = get_time_ms();
		dongle->usedby_coder = 0;
		pthread_cond_broadcast(&dongle->cond_dongle);
	}
	pthread_mutex_unlock(&dongle->mutex_dongle);
}
