/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:41:35 by kri-              #+#    #+#             */
/*   Updated: 2026/10/21 19:43:20 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_init(t_uint id, t_dongle *dongle, t_input *input)
{
	dongle->dongle_id = id;
	dongle->cooldown_time = input->cooldown_time;
	dongle->is_being_used = false;
	dongle->scheduler = input->scheduler;
}

bool	assign_dongle(t_dongle *dongle, t_ulong deadline)
{
	bool	dongle_occupied;
	t_ulong	rel_time;

	(void ) deadline;
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

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex_dongle_state);
	dongle->released_time = get_time_ms();
	dongle->is_being_used = false;
	pthread_mutex_unlock(&dongle->mutex_dongle_state);
}
