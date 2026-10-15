/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 17:31:19 by kri-              #+#    #+#             */
/*   Updated: 2026/10/14 19:22:50 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_threads(t_hub *hub)
{
	t_uint	num;

	num = 0;
	pthread_create(&hub->thread_monitor, NULL, monitor_routine, hub);
	while (num < hub->input.num_coders)
	{
		create_coder_threads(&hub->coders[num].thread_coder, &hub->coders[num]);
		num++;
	}
	pthread_mutex_lock(&hub->mutex_start);
	hub->sim_start = true;
	hub->start_time = get_time_ms();
	pthread_cond_broadcast(&hub->cond_start);
	pthread_mutex_unlock(&hub->mutex_start);
}

void	join_threads(t_hub *hub)
{
	t_uint	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		pthread_join(hub->coders[num].thread_coder, NULL);
		num++;
	}
	pthread_join(hub->thread_monitor, NULL);
}
