/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 19:12:54 by kri-              #+#    #+#             */
/*   Updated: 2026/10/28 20:53:38 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_init(t_coder *coder, t_input *input, t_dongle *ldongle,
	t_dongle *rdongle)
{
	if (ldongle < rdongle)
	{
		coder->left_dongle = ldongle;
		coder->right_dongle = rdongle;
	}
	else
	{
		coder->right_dongle = ldongle;
		coder->left_dongle = rdongle;
	}
	coder->curr_compl_count = 0;
	coder->max_compiles = input->num_compiles;
	coder->has_finished = false;
	coder->burnout_time = input->burnout_time;
	coder->compile_time = input->compile_time;
	coder->debug_time = input->debug_time;
	coder->refactor_time = input->refactor_time;
	coder->prev_compl_start_time = get_time_ms();
}

bool	request_dongles(t_coder *coder, t_ulong	deadline)
{
	t_uint	ldid;
	t_uint	rdid;

	ldid = coder->left_dongle->dongle_id;
	rdid = coder->right_dongle->dongle_id;
	if (assign_dongle(coder->id, coder->left_dongle, deadline) != RUN)
		return (false);
	if (rdid == ldid)
	{
		log_event(coder->id, ldid, "has taken a dongle", coder->logger);
		return (false);
	}
	if (assign_dongle(coder->id, coder->right_dongle, deadline) != RUN)
	{
		release_dongle(coder->left_dongle);
		return (false);
	}
	if (get_simulation_status(coder->simctrl) == RUN)
	{
		log_event(coder->id, ldid, "has taken a dongle", coder->logger);
		log_event(coder->id, rdid, "has taken another dongle", coder->logger);
		return (true);
	}
	release_dongles(coder);
	return (false);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left_dongle);
	if (coder->right_dongle != coder->left_dongle)
		release_dongle(coder->right_dongle);
}

bool	get_coder_finish_status(t_coder *coder)
{
	bool	result;

	pthread_mutex_lock(&coder->mutex_coder);
	result = coder->has_finished;
	pthread_mutex_unlock(&coder->mutex_coder);
	return (result);
}
