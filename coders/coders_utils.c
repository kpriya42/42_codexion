/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 19:12:54 by kri-              #+#    #+#             */
/*   Updated: 2026/10/16 16:19:46 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
