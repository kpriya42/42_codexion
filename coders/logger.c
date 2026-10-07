/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 16:23:11 by kri-              #+#    #+#             */
/*   Updated: 2026/10/06 17:06:39 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_uint coder_id, const char *event, t_hub *hub)
{
	t_ulong	timestamp;

	timestamp = get_time_ms();
	pthread_mutex_lock(&hub->mutex_print);
	printf("%lu %d %s", timestamp, coder_id, event);
	pthread_mutex_unlock(&hub->mutex_print);
}
