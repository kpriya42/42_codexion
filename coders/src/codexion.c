/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 17:31:19 by kri-              #+#    #+#             */
/*   Updated: 2026/10/29 14:05:20 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	stop_join_sim(t_hub *hub, t_uint created)
{
	t_uint	num;

	pthread_mutex_lock(&hub->simctrl.mutex_sim);
	hub->simctrl.sim_status = STOP;
	pthread_cond_broadcast(&hub->simctrl.cond_sim);
	pthread_mutex_unlock(&hub->simctrl.mutex_sim);
	pthread_join(hub->thread_monitor, NULL);
	num = 0;
	while (num < created)
	{
		pthread_join(hub->coders[num].thread_coder, NULL);
		num++;
	}
}

t_err	create_threads_start_sim(t_hub *hub)
{
	t_uint	num;

	num = 0;
	if (pthread_create(&hub->thread_monitor, NULL, monitor_routine, hub) != 0)
		return (THREAD_FAIL);
	while (num < hub->input.num_coders)
	{
		if (pthread_create(
				&hub->coders[num].thread_coder,
				NULL, coder_routine,
				&hub->coders[num]) != 0)
		{
			stop_join_sim(hub, num);
			return (THREAD_FAIL);
		}
		num++;
	}
	pthread_mutex_lock(&hub->simctrl.mutex_sim);
	hub->logger.start_time = get_time_ms();
	while (num--)
		hub->coders[num].prev_compl_start_time = get_time_ms();
	hub->simctrl.sim_status = RUN;
	pthread_cond_broadcast(&hub->simctrl.cond_sim);
	pthread_mutex_unlock(&hub->simctrl.mutex_sim);
	return (NO_ERR);
}

void	join_threads(t_hub *hub)
{
	t_uint	num;

	num = 0;
	pthread_join(hub->thread_monitor, NULL);
	while (num < hub->input.num_coders)
	{
		pthread_join(hub->coders[num].thread_coder, NULL);
		num++;
	}
}
