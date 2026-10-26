/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:35:32 by kri-              #+#    #+#             */
/*   Updated: 2026/10/24 15:51:58 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	initialize_dongles(t_hub *hub)
{
	t_uint	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		dongle_init(num + 1, &hub->dongles[num], &hub->input);
		hub->dongles[num].simctrl = &hub->simctrl;
		num++;
	}
}

static void	initialize_coders(t_hub *hub)
{
	t_uint	num;
	t_uint	other;

	num = 0;
	while (num < hub->input.num_coders)
	{
		hub->coders[num].coder_id = num + 1;
		hub->coders[num].logger = &hub->logger;
		hub->coders[num].simctrl = &hub->simctrl;
		other = (num - 1 + hub->input.num_coders) % hub->input.num_coders;
		if (num < other)
			coder_init(&hub->coders[num], &hub->input,
				&hub->dongles[num], &hub->dongles[other]);
		else
			coder_init(&hub->coders[num], &hub->input,
				&hub->dongles[other], &hub->dongles[num]);
		num++;
	}
}

static void	initialize_mutexes(t_hub *hub)
{
	t_uint	num;
	int		op[2];

	num = 0;
	while (num < hub->input.num_coders)
	{
		op[0] = pthread_mutex_init(&hub->dongles[num].mutex_dongle_state, NULL);
		op[1] = pthread_mutex_init(&hub->coders[num].mutex_coder_state, NULL);
		if (op[0] != 0 || op[1] != 0)
			break ;
		num++;
	}
	if (op[0] != 0 || op[1] != 0
		|| pthread_mutex_init(&hub->logger.mutex_print, NULL) != 0
		|| pthread_mutex_init(&hub->simctrl.mutex_sim, NULL) != 0
		|| pthread_cond_init(&hub->simctrl.cond_sim, NULL) != 0)
	{
		free_all(hub, hub->input.num_coders);
		display_err(MTX_INIT_ERR);
	}
}

void	initialize_hub(t_hub *hub)
{
	t_uint	num_coders;

	num_coders = hub->input.num_coders;
	hub->simctrl.sim_status = INIT;
	hub->coders = malloc(num_coders * sizeof(t_coder));
	hub->dongles = malloc(num_coders * sizeof(t_dongle));
	if (!hub->coders || !hub->dongles)
	{
		free_mem(hub);
		display_err(INIT_ERR);
	}
	initialize_mutexes(hub);
	initialize_dongles(hub);
	initialize_coders(hub);
}
