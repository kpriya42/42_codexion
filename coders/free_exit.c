/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 16:03:23 by kri-              #+#    #+#             */
/*   Updated: 2026/10/23 17:17:42 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_mem(t_hub *hub)
{
	if (hub->coders)
		free(hub->coders);
	if (hub->dongles)
		free(hub->dongles);
}

void	free_all(t_hub *hub, t_uint num_coders)
{
	t_uint	num;

	num = 0;
	while (num < num_coders)
	{
		printf("dongles & coders %d mtxs destroyed\n", num + 1);
		pthread_mutex_destroy(&hub->dongles[num].mutex_dongle_state);
		pthread_mutex_destroy(&hub->coders[num].mutex_coder_state);
		//pthread_cond_destroy(hub->coders[num].cond_sim);
		num++;
	}
	free_mem(hub);
	pthread_mutex_destroy(&hub->logger.mutex_print);
	pthread_cond_destroy(&hub->cond_sim);
	pthread_mutex_destroy(&hub->mutex_sim);
	printf("Hub mtxs & conds destroyed\n");
}
