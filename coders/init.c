/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:35:32 by kri-              #+#    #+#             */
/*   Updated: 2026/10/06 17:37:01 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	initialize_dongles(t_hub *hub, t_input *input)
{
	t_uint	num;

	num = 0;
	while (num < input->num_coders)
	{
		dongle_init(num + 1, &hub->dongles[num], input);
		num++;
	}
}

static void	initialize_coders(t_hub *hub, t_input *input)
{
	t_uint	num;
	t_uint	other;

	num = 0;
	while (num < input->num_coders)
	{
		hub->coders[num].coder_id = num + 1;
		other = (num - 1 + input->num_coders) % input->num_coders;
		if (num < other)
			coder_init(&hub->coders[num], input,
				&hub->dongles[num], &hub->dongles[other]);
		else
			coder_init(&hub->coders[num], input,
				&hub->dongles[other], &hub->dongles[num]);
		num++;
	}
}

static void	initialize_mutexes(t_hub *hub, t_input *input)
{
	t_uint	num;
	int				op;

	num = 0;
	while (num < input->num_coders)
	{
		op = pthread_mutex_init(&hub->dongles[num].mutex_dongle_state, NULL);
		if (op != 0)
			display_err(MTX_INIT_ERR);
		num++;
	}
	if (pthread_mutex_init(&hub->mutex_print, NULL) != 0)
		display_err(MTX_INIT_ERR);
}

void	initialize_hub(t_hub *hub, t_input *input)
{
	t_uint	num_coders;

	num_coders = input->num_coders;
	hub->coders = malloc(num_coders * sizeof(t_coder));
	hub->dongles = malloc(num_coders * sizeof(t_dongle));
	if (!hub->coders || !hub->dongles)
		display_err(INIT_ERR);
	initialize_mutexes(hub, input);
	initialize_dongles(hub, input);
	initialize_coders(hub, input);
}
