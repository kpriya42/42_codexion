/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 13:01:38 by kri-              #+#    #+#             */
/*   Updated: 2026/10/27 21:27:36 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_uint	check_burnout(t_hub *hub)
{
	t_uint	num;
	t_ulong	now;
	t_ulong	prev_cst;
	t_uint	count;

	num = 0;
	while (num < hub->input.num_coders)
	{
		pthread_mutex_lock(&hub->coders[num].mutex_coder);
		prev_cst = hub->coders[num].prev_compl_start_time;
		count = hub->coders[num].curr_compl_count;
		pthread_mutex_unlock(&hub->coders[num].mutex_coder);
		now = get_time_ms();
		if ((now - prev_cst) >= hub->input.burnout_time
			&& count < hub->coders[num].max_compiles)
		{
			return (hub->coders[num].id);
		}
		num++;
	}
	return (0);
}

static bool	check_finished(t_hub *hub)
{
	t_uint	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		if (!get_coder_finish_status(&hub->coders[num]))
			return (false);
		num++;
	}
	log_event(0, 0, " All Done!", &hub->logger);
	return (true);
}

static void	wake_up_waiting_dongles(t_hub *hub)
{
	t_uint	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		pthread_mutex_lock(&hub->dongles[num].mutex_dongle);
		pthread_cond_broadcast(&hub->dongles[num].cond_dongle);
		pthread_mutex_unlock(&hub->dongles[num].mutex_dongle);
		num++;
	}
}

void	*monitor_routine(void *arg)
{
	t_hub	*hub;
	t_uint	burnt_id;

	hub = (t_hub *) arg;
	pthread_mutex_lock(&hub->simctrl.mutex_sim);
	while (hub->simctrl.sim_status != RUN)
		pthread_cond_wait(&hub->simctrl.cond_sim, &hub->simctrl.mutex_sim);
	pthread_mutex_unlock(&hub->simctrl.mutex_sim);
	while (get_simulation_status(&hub->simctrl) != STOP)
	{
		burnt_id = check_burnout(hub);
		if (burnt_id || check_finished(hub))
		{
			pthread_mutex_lock(&hub->simctrl.mutex_sim);
			hub->simctrl.sim_status = STOP;
			pthread_cond_broadcast(&hub->simctrl.cond_sim);
			pthread_mutex_unlock(&hub->simctrl.mutex_sim);
			wake_up_waiting_dongles(hub);
			break ;
		}
		usleep(500);
	}
	if (burnt_id)
		log_event(burnt_id, 0, "has burnt out", &hub->logger);
	return (NULL);
}
