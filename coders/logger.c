/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 16:23:11 by kri-              #+#    #+#             */
/*   Updated: 2026/10/08 15:47:56 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_uint id, const char *event, t_ulong tstamp, t_mutex *mutex)
{
	pthread_mutex_lock(mutex);
	if (id > 0)
		printf("%lu %d %s\n", tstamp, id, event);
	else
		printf("%lu %s\n", tstamp, event);
	pthread_mutex_unlock(mutex);
}
