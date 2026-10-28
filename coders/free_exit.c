/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 18:28:35 by kri-              #+#    #+#             */
/*   Updated: 2026/10/28 19:45:01 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_mem(t_hub *hub)
{
	free(hub->coders);
	free(hub->dongles);
	hub->coders = NULL;
	hub->dongles = NULL;
}

void	free_log_mtx(t_hub *hub)
{
	pthread_mutex_destroy(&hub->logger.mutex_print);
	free_mem(hub);
}

t_err	destroy(t_hub *hub, t_err err_typ, t_uint num)
{
	if (err_typ == DGL_MTX)
	{
		free_dongle_mtx(hub, num);
		free_sim_cond(hub);
		free_sim_mtx(hub);
		free_log_mtx(hub);
	}
	else if (err_typ == DGL_COND)
	{
		free_dongle_cond(hub, num);
		free_dongle_mtx(hub, num + 1);
		free_sim_cond(hub);
		free_sim_mtx(hub);
		free_log_mtx(hub);
	}
	else if (err_typ == CDR_MTX)
	{
		free_coder_mtx(hub, num);
		free_dongle_cond(hub, hub->input.num_coders);
		free_dongle_mtx(hub, hub->input.num_coders);
		free_sim_cond(hub);
		free_sim_mtx(hub);
		free_log_mtx(hub);
	}
	return (err_typ);
}

void	destroy_all(t_hub *hub)
{
	free_coder_mtx(hub, hub->input.num_coders);
	free_dongle_cond(hub, hub->input.num_coders);
	free_dongle_mtx(hub, hub->input.num_coders);
	free_sim_cond(hub);
	free_sim_mtx(hub);
	free_log_mtx(hub);
}
