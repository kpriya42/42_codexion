/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 16:03:23 by kri-              #+#    #+#             */
/*   Updated: 2026/10/07 19:30:51 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_and_exit(t_hub *hub, t_input *input)
{
	t_uint	num;
	int		op;

	num = 0;
	while (num < input->num_coders)
	{
		pthread_mutex_destroy(&hub->dongles[num].mutex_dongle_state);
		free(&hub->dongles[num]);
		free(&hub->coders[num]);
		num++;
	}
	free(hub->coders);
	free(hub->dongles);
	pthread_mutex_destroy(&hub->mutex_print);
	//To do: delete threads
}
