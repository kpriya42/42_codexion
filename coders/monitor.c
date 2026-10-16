/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 13:01:38 by kri-              #+#    #+#             */
/*   Updated: 2026/10/16 19:18:28 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//static bool	check_burnout(t_hub *hub)
//{
//	t_uint	num;
//	t_ulong	now;
//	t_ulong	prev_cst;

//	now = get_time_ms() - hub->start_time;
//	num = 0;
//	while (num < hub->input.num_coders)
//	{
//		pthread_mutex_lock(&hub->coders[num].mutex_coder_state);
//		prev_cst = hub->coders[num].prev_compl_start_time;
//		pthread_mutex_unlock(&hub->coders[num].mutex_coder_state);
//		if ((now - prev_cst) >= hub->input.burnout_time)
//		{
//			log_event(hub->coders[num].coder_id, "has burnt out",
//				now - hub->start_time, &hub->mutex_print);
//			log_event(hub->coders[num].coder_id, "burned out at ",
//				hub->input.burnout_time, &hub->mutex_print);
//			return (true);
//		}
//		num++;
//	}
//	return (false);
//}

static bool	check_all_finished(t_hub *hub)
{
	t_uint	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		if (!get_coder_finish_status(&hub->coders[num]))
			return (false);
		num++;
	}
	log_event(0, " All finished!", get_time_ms() - hub->start_time,
		&hub->mutex_print);
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
	t_ulong	tstamp;

	hub = (t_hub *) arg;
	pthread_mutex_lock(&hub->mutex_start);
	while (!hub->sim_start)
		pthread_cond_wait(&hub->cond_start, &hub->mutex_start);
	pthread_mutex_unlock(&hub->mutex_start);
	tstamp = get_time_ms() - hub->start_time;
	log_event(0, "Monitor routine starts", tstamp, &hub->mutex_print);
	while (get_sim_status(hub))
	{
		//if (check_all_finished(hub) || check_burnout(hub))
		if (check_all_finished(hub))
		{
			pthread_mutex_lock(&hub->mutex_start);
			hub->sim_start = false;
			pthread_mutex_unlock(&hub->mutex_start);
			break ;
		}
	}
	log_event(0, " Monitoring complete!", get_time_ms() - hub->start_time, &hub->mutex_print);
	return (NULL);
}
