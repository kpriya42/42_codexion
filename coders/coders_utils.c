/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 19:12:54 by kri-              #+#    #+#             */
/*   Updated: 2026/10/18 22:11:45 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	request_dongles(t_coder *coder, t_ulong	deadline)
{
	if (!assign_dongle(coder->left_dongle, deadline))
		return (false);
	log_event(coder->coder_id, " has taken a dongle", coder->logger);
	return (true);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left_dongle);
}

void	set_finished(t_coder *coder, t_cstate *nextstate)
{
	if (coder->curr_compl_count == coder->max_compiles)
	{
		pthread_mutex_lock(&coder->mutex_coder_state);
		coder->has_finished = true;
		*nextstate = end;
		pthread_mutex_unlock(&coder->mutex_coder_state);
	}
	else
		*nextstate = compile;
}

bool	get_simulation_status(t_coder *coder)
{
	bool	result;

	pthread_mutex_lock(coder->mutex_start);
	result = *coder->sim_start;
	pthread_mutex_unlock(coder->mutex_start);

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
