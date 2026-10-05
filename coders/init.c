/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:35:32 by kri-              #+#    #+#             */
/*   Updated: 2026/10/05 17:39:40 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	initialize_dongles(t_hub *hub, t_input *input)
{
	unsigned int	num;

	num = 0;
	while (num < input->num_coders)
	{
		dongle_init(&hub->dongles[num], input);
		num++;
	}
}

static void	initialize_coders(t_hub *hub, t_input *input)
{
	unsigned int	num;

	num = 0;
	while (num < input->num_coders)
	{
		hub->coders[num].coder_id = num + 1;
		coder_init(&hub->coders[num], input);
		num++;
	}
}

void	initialize_hub(t_hub *hub, t_input *input)
{
	unsigned int	num_coders;

	num_coders = input->num_coders;
	hub->coders = malloc(num_coders * sizeof(t_coder));
	hub->dongles = malloc(num_coders * sizeof(t_dongle));
	if (!hub->coders || !hub->dongles)
		display_err(INIT_ERR_MSG);

	initialize_dongles(hub, input);
	initialize_coders(hub, input);
	initialize_mutexes(hub, input);
}
