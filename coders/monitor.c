/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 13:01:38 by kri-              #+#    #+#             */
/*   Updated: 2026/10/14 14:36:57 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	log_event(0, "Monitor routine", tstamp, &hub->mutex_print);
	return (NULL);
}
