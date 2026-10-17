/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 17:31:19 by kri-              #+#    #+#             */
/*   Updated: 2026/10/17 17:46:49 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_threads_start_sim(t_hub *hub)
{
	t_uint	num;

	num = 0;
	pthread_create(&hub->thread_monitor, NULL, monitor_routine, hub);
	while (num < hub->input.num_coders)
	{
		pthread_create(
			&hub->coders[num].thread_coder,
			NULL,
			coder_routine,
			&hub->coders[num]);
		num++;
	}
	pthread_mutex_lock(&hub->mutex_start);
	hub->start_time = get_time_ms();
	while (num--)
		hub->coders[num].prev_compl_start_time = hub->start_time;
	hub->sim_start = true;
	pthread_cond_broadcast(&hub->cond_start);
	pthread_mutex_unlock(&hub->mutex_start);
}

void	join_threads(t_hub *hub)
{
	t_uint	num;

	num = 0;
	printf("\nThreads join starting..\n");
	while (num < hub->input.num_coders)
	{
		pthread_join(hub->coders[num].thread_coder, NULL);
		num++;
	}
	pthread_join(hub->thread_monitor, NULL);
	printf("\nThreads join complete..\n");
}
