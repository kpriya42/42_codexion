/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 13:01:38 by kri-              #+#    #+#             */
/*   Updated: 2026/10/18 22:25:44 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	check_finished_or_burned(t_hub *hub)
{
	t_uint	num;
	t_ulong	now;
	t_ulong	prev_cst;

	num = 0;
	while (num < hub->input.num_coders)
	{
		if (!get_coder_finish_status(&hub->coders[num]))
		{
			pthread_mutex_lock(&hub->coders[num].mutex_coder_state);
			prev_cst = hub->coders[num].prev_compl_start_time;
			pthread_mutex_unlock(&hub->coders[num].mutex_coder_state);
			now = get_time_ms();
			if ((now - prev_cst) > hub->input.burnout_time)
			{
				log_event(hub->coders[num].coder_id, "has burnt out",
					&hub->logger);
				return (true);
			}
			return (false);
		}
		num++;
	}
	log_event(0, " All Done!", &hub->logger);
	return (true);
}

static bool	get_sim_status(t_hub *hub)
{
	bool	sim_status;

	pthread_mutex_lock(&hub->mutex_start);
	sim_status = hub->sim_start;
	pthread_mutex_unlock(&hub->mutex_start);

	return (sim_status);
}

void	*monitor_routine(void *arg)
{
	t_hub	*hub;

	hub = (t_hub *) arg;
	pthread_mutex_lock(&hub->mutex_start);
	while (!hub->sim_start)
		pthread_cond_wait(&hub->cond_start, &hub->mutex_start);
	pthread_mutex_unlock(&hub->mutex_start);
	// log_event(0, "Monitor routine starts", &hub->logger);
	while (get_sim_status(hub))
	{
		if (check_finished_or_burned(hub))
		{
			pthread_mutex_lock(&hub->mutex_start);
			hub->sim_start = false;
			pthread_mutex_unlock(&hub->mutex_start);
			break ;
		}
		usleep(100);
	}
	log_event(0, " Monitoring complete!", &hub->logger);
	return (NULL);
}
