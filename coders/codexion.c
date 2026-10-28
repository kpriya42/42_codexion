/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 17:31:19 by kri-              #+#    #+#             */
/*   Updated: 2026/10/28 19:14:43 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	create_threads_start_sim(t_hub *hub)
{
	t_uint	num;

	num = 0;
	pthread_create(&hub->thread_monitor, NULL, monitor_routine, hub);
	while (num < hub->input.num_coders)
	{
		if (pthread_create(
				&hub->coders[num].thread_coder,
				NULL,
				coder_routine,
				&hub->coders[num]) != 0)
			return (false);
		num++;
	}
	pthread_mutex_lock(&hub->simctrl.mutex_sim);
	hub->logger.start_time = get_time_ms();
	while (num--)
		hub->coders[num].prev_compl_start_time = get_time_ms();
	hub->simctrl.sim_status = RUN;
	pthread_cond_broadcast(&hub->simctrl.cond_sim);
	pthread_mutex_unlock(&hub->simctrl.mutex_sim);
	return (true);
}

void	join_threads(t_hub *hub)
{
	t_uint	num;

	num = 0;
	printf("\nThreads starting to join..\n\n");
	pthread_join(hub->thread_monitor, NULL);
	while (num < hub->input.num_coders)
	{
		pthread_join(hub->coders[num].thread_coder, NULL);
		num++;
	}
	printf("\nThreads join complete..\n");
}
