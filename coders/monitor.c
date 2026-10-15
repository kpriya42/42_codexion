/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 13:01:38 by kri-              #+#    #+#             */
/*   Updated: 2026/10/14 19:10:20 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	check_burnout(t_hub *hub)
{
	t_uint	num;
	t_ulong	now;
	t_ulong	prev_compl_start;

	now = get_time_ms();
	num = 0;
	while (num < hub->input.num_coders)
	{
		prev_compl_start = hub->coders[num].prev_compl_start_time;
		if (now - prev_compl_start >= hub->input.burnout_time)
		{
			log_event(hub->coders[num].coder_id, " has burnt out",
				now, &hub->mutex_print);
			return (true);
		}
		num++;
	}
	return (false);
}

static bool	check_all_finished(t_hub *hub)
{
	t_uint	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		if (hub->coders[num].has_finished == false)
			return (false);
		num++;
	}
	log_event(0, " All Done!", get_time_ms(), &hub->mutex_print);
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_hub	*hub;
	t_ulong	tstamp;

	hub = (t_hub *) arg;
	pthread_mutex_lock(&hub->mutex_start);
	while (!hub->sim_start)
		pthread_cond_wait(&hub->cond_start, &hub->mutex_start);
	pthread_mutex_unlock(&hub->mutex_start);
	tstamp = get_time_ms() - hub->start_time;
	log_event(0, "Monitor routine starts", tstamp, &hub->mutex_print);
	while (hub->sim_start)
	{
		if (check_burnout(hub) || check_all_finished(hub))
		{
			hub->sim_start = false;
			break ;
		}
	}
	return (NULL);
}
