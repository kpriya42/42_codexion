/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 13:01:38 by kri-              #+#    #+#             */
/*   Updated: 2026/10/08 15:48:29 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_hub	*hub;
	t_ulong	tstamp;

	hub = (t_hub *) arg;
	tstamp = get_time_ms() - hub->start_time;
	//printf("This is coderroutine %d\n", coder->coder_id);
	log_event(0, "Monitor routine", tstamp, &hub->mutex_print);
	return (NULL);
}
