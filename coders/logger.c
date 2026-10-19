/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 16:23:11 by kri-              #+#    #+#             */
/*   Updated: 2026/10/16 19:01:43 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_uint id, const char *event, t_logger *logger)
{
	t_ulong	tstamp;

	tstamp = get_time_ms() - logger->start_time;
	pthread_mutex_lock(&logger->mutex_print);
	// if (!strcmp(event, "nextstate = "))
	// 	printf("%s %d\n", event, (t_cstate)id);
	// else if (!strcmp(event, "burned out at "))
	// 	printf("coder %d %s %lu\n", id, event, tstamp);
	// else
	if (id > 0)
		printf("%lu coder %d %s\n", tstamp, id, event);
	else
		printf("%lu %s\n", tstamp, event);
	pthread_mutex_unlock(&logger->mutex_print);
}
