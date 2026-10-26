/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:41:35 by kri-              #+#    #+#             */
/*   Updated: 2026/10/26 14:35:30 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_init(t_uint id, t_dongle *dongle, t_input *input)
{
	dongle->dongle_id = id;
	dongle->cooldown_time = input->cooldown_time;
	dongle->is_being_used = false;
	dongle->scheduler = input->scheduler;
	dongle->request_count = 0;
}

static void	wait_for_dongle_available(t_dongle *dongle, t_uint coder_id)
{
	check if dongle is held or 
	cool_down is not passed or 
	this coder can acquire this dongle
	
}
bool	assign_dongle(t_coder *coder, t_dongle *dongle, t_ulong deadline)
{
	bool		dongle_occupied;
	t_ulong		rel_time;
	t_slotreq	request;

	(void ) deadline;
	request.coder_id = coder->coder_id;
	if (dongle->scheduler == fifo)
		request.fifo_priority = dongle->request_count++;
	else
		request.edf_priority = deadline;
	
	pthread_mutex_lock(&dongle->mutex_dongle_state);
	dongle_occupied = dongle->is_being_used;
	rel_time = dongle->released_time;
	pthread_mutex_unlock(&dongle->mutex_dongle_state);
	if (dongle_occupied)
		return (false);
	else if ((get_time_ms() - rel_time) > dongle->cooldown_time)
	{
		pthread_mutex_lock(&dongle->mutex_dongle_state);
		dongle->is_being_used = true;
		pthread_mutex_unlock(&dongle->mutex_dongle_state);
		return (true);
	}
	return (false);
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
	
//	pthread_mutex_lock(&dongle->mutex_dongle_state);
//	dongle_occupied = dongle->is_being_used;
//	rel_time = dongle->released_time;
//	pthread_mutex_unlock(&dongle->mutex_dongle_state);
//	if (dongle_occupied)
//		return (false);
//	else if ((get_time_ms() - rel_time) > dongle->cooldown_time)
//	{
//		pthread_mutex_lock(&dongle->mutex_dongle_state);
//		dongle->is_being_used = true;
//		pthread_mutex_unlock(&dongle->mutex_dongle_state);
//		return (true);
//	}
//	return (false);
//}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex_dongle_state);
	dongle->released_time = get_time_ms();
	dongle->is_being_used = false;
	dongle->used_by_coder_id = 0;
	pthread_mutex_unlock(&dongle->mutex_dongle_state);
}
