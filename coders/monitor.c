/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 13:01:38 by kri-              #+#    #+#             */
/*   Updated: 2026/10/24 16:06:37 by kri-             ###   ########.fr       */
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
		//if (!get_coder_finish_status(&hub->coders[num]))
		//{
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
		//}
		num++;
	}
	log_event(0, " All Done!", &hub->logger);
	return (true);
}

static t_state	get_sim_status(t_hub *hub)
{
	t_state	sim_status;

	pthread_mutex_lock(&hub->simctrl.mutex_sim);
	sim_status = hub->simctrl.sim_status;
	pthread_mutex_unlock(&hub->simctrl.mutex_sim);

	return (sim_status);
}

void	*monitor_routine(void *arg)
{
	t_hub	*hub;

	hub = (t_hub *) arg;
	pthread_mutex_lock(&hub->simctrl.mutex_sim);
	while (hub->simctrl.sim_status != RUN)
		pthread_cond_wait(&hub->simctrl.cond_sim, &hub->simctrl.mutex_sim);
	pthread_mutex_unlock(&hub->simctrl.mutex_sim);
	// log_event(0, "Monitor routine starts", &hub->logger);
	while (get_sim_status(hub) != STOP)
	{
		if (check_finished_or_burned(hub))
		{
			pthread_mutex_lock(&hub->simctrl.mutex_sim);
			hub->simctrl.sim_status = STOP;
			pthread_cond_broadcast(&hub->simctrl.cond_sim);
			pthread_mutex_unlock(&hub->simctrl.mutex_sim);
			break ;
		}
		usleep(500);
	}
	log_event(0, " Monitoring complete!", &hub->logger);
	return (NULL);
}
