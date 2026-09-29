/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 13:35:32 by kri-              #+#    #+#             */
/*   Updated: 2026/09/29 17:23:26 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "init.h"
#include "coders.h"

static void	display_err(void)
{
	write(STDERR_FILENO, INIT_ERR_MSG, strlen(INIT_ERR_MSG));
	exit(1);
}

static void	initialize_coders(t_hub *hub)
{
	unsigned int	num;

	num = 0;
	while (num < hub->input.num_coders)
	{
		coder_init(num, hub->coders[num]);
	}

}


void	initialize_hub(t_hub *hub)
{
	unsigned int	num_coders;

	num_coders = hub->input.num_coders;
	hub->coders = malloc(num_coders * sizeof(t_coder));
	hub->dongles = malloc(num_coders * sizeof(t_dongle));
	if (!hub->coders || !hub->dongles)
		display_err();

	initialize_dongles();
	initialize_coders();
}
