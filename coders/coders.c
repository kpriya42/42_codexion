/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/22 17:07:29 by kri-              #+#    #+#             */
/*   Updated: 2026/10/11 20:32:13 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_init(t_coder *coder, t_input *input, t_dongle *ldongle,
	t_dongle *rdongle)
{
	coder->left_dongle = ldongle;
	coder->right_dongle = rdongle;
	coder->curr_compl_count = 0;
	coder->prev_compl_start_time = *coder->start_time;
	coder->burnout_time = input->burnout_time;
	coder->compile_time = input->compile_time;
	coder->debug_time = input->debug_time;
	coder->refactor_time = input->refactor_time;
}

void	create_coder_threads(pthread_t *thread_coder, t_coder *coder)
{
	pthread_create(thread_coder, NULL, coder_routine, coder);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_ulong	tstamp;

	coder = (t_coder *) arg;
	pthread_mutex_lock(coder->mutex_start);
	while (*coder->sim_start == false)
		pthread_cond_wait(coder->cond_start, coder->mutex_start);
	pthread_mutex_unlock(coder->mutex_start);
	tstamp = get_time_ms() - *coder->start_time;
	log_event(coder->coder_id, "Coder routine", tstamp, coder->mutex_print);
	return (NULL);
}
