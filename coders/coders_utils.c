/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 19:12:54 by kri-              #+#    #+#             */
/*   Updated: 2026/10/26 12:50:50 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_init(t_coder *coder, t_input *input, t_dongle *ldongle,
	t_dongle *rdongle)
{
	coder->left_dongle = ldongle;
	coder->right_dongle = rdongle;
	coder->curr_compl_count = 0;
	coder->max_compiles = input->num_compiles;
	coder->has_finished = false;
	coder->burnout_time = input->burnout_time;
	coder->compile_time = input->compile_time;
	coder->debug_time = input->debug_time;
	coder->refactor_time = input->refactor_time;
}

bool	request_dongles(t_coder *coder, t_ulong	deadline)
{
	if (!assign_dongle(coder->left_dongle, deadline))
		return (false);
	log_event(coder->coder_id, "has taken a dongle", coder->logger);
	//if (!assign_dongle(coder->right_dongle, deadline))
	//{
	//	release_dongle(coder->left_dongle);
	//	return (false);
	//}
	//log_event(coder->coder_id, "has taken another dongle", coder->logger);
	return (true);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left_dongle);
	//release_dongle(coder->right_dongle);
}

t_state	get_simulation_status(t_coder *coder)
{
	t_state	result;

	pthread_mutex_lock(&coder->simctrl->mutex_sim);
	result = coder->simctrl->sim_status;
	pthread_mutex_unlock(&coder->simctrl->mutex_sim);
	return (result);
}

bool	get_coder_finish_status(t_coder *coder)
{
	bool	result;

	pthread_mutex_lock(&coder->mutex_coder_state);
	result = coder->has_finished;
	pthread_mutex_unlock(&coder->mutex_coder_state);
	return (result);
}
