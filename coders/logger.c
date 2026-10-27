/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 16:23:11 by kri-              #+#    #+#             */
/*   Updated: 2026/10/27 19:13:21 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_uint cid, t_uint did, const char *event, t_logger *logger)
{
	t_ulong	tstamp;

	tstamp = get_time_ms() - logger->start_time;
	pthread_mutex_lock(&logger->mutex_print);
	if (cid > 0 && did > 0)
		printf("%lu coder %d %s %d\n", tstamp, cid, event, did);
	else if (cid > 0)
		printf("%lu coder %d %s\n", tstamp, cid, event);
	else
		printf("%lu %s\n", tstamp, event);
	pthread_mutex_unlock(&logger->mutex_print);
}
