/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 17:31:19 by kri-              #+#    #+#             */
/*   Updated: 2026/10/11 20:31:49 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_threads(t_hub *hub, t_input *input)
{
	t_uint	num;

	num = 0;
	pthread_create(&hub->thread_monitor, NULL, monitor_routine, hub);
	while (num < input->num_coders)
	{
		create_coder_threads(&hub->coders[num].thread_coder, &hub->coders[num]);
		num++;
	}
	pthread_mutex_lock(&hub->mutex_start);
	hub->start_time = get_time_ms();
	hub->sim_start = true;
	pthread_cond_broadcast(&hub->cond_start);
	pthread_mutex_unlock(&hub->mutex_start);
}

void	join_threads(t_hub *hub, t_input *input)
{
	t_uint	num;

	num = 0;
	while (num < input->num_coders)
	{
		pthread_join(hub->coders[num].thread_coder, NULL);
		num++;
	}
	pthread_join(hub->thread_monitor, NULL);
}
