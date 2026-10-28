/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:35:32 by kri-              #+#    #+#             */
/*   Updated: 2026/10/28 19:28:26 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	initialize_dongles(t_hub *hub)
{
	t_uint	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		if (pthread_mutex_init(&hub->dongles[num].mutex_dongle, NULL) != 0)
			return (destroy(hub, DGL_MTX, num));
		if (pthread_cond_init(&hub->dongles[num].cond_dongle, NULL) != 0)
			return (destroy(hub, DGL_COND, num));
		dongle_init(num + 1, &hub->dongles[num], &hub->input);
		hub->dongles[num].simctrl = &hub->simctrl;
		num++;
	}
	return (NO_ERR);
}

static bool	initialize_coders(t_hub *hub)
{
	t_uint	num;
	t_uint	other;

	num = 0;
	while (num < hub->input.num_coders)
	{
		if (pthread_mutex_init(&hub->coders[num].mutex_coder, NULL) != 0)
			return (destroy(hub, CDR_MTX, num));
		hub->coders[num].id = num + 1;
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
	return (NO_ERR);
}

t_err	initialize_hub(t_hub *hub)
{
	t_uint	num_coders;
	t_err	dongle_init;

	num_coders = hub->input.num_coders;
	hub->simctrl.sim_status = INIT;
	hub->coders = malloc(num_coders * sizeof(t_coder));
	hub->dongles = malloc(num_coders * sizeof(t_dongle));
	if (!hub->coders || !hub->dongles)
		return (free_mem(hub), MALLOC_FAIL);
	if (pthread_mutex_init(&hub->logger.mutex_print, NULL) != 0)
		return (free_mem(hub), LOG_MTX);
	if (pthread_mutex_init(&hub->simctrl.mutex_sim, NULL) != 0)
		return (free_log_mtx(hub), SIM_MTX);
	if (pthread_cond_init(&hub->simctrl.cond_sim, NULL) != 0)
	{
		free_sim_mtx(hub);
		return (free_log_mtx(hub), SIM_COND);
	}
	dongle_init = initialize_dongles(hub);
	if (dongle_init == NO_ERR)
		return (initialize_coders(hub));
	return (dongle_init);
}
