/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mutex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 16:03:23 by kri-              #+#    #+#             */
/*   Updated: 2026/10/28 18:44:43 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	free_dongle_mtx(t_hub *hub, t_uint dnum)
{
	t_uint	num;

	num = 0;
	while (num < dnum)
	{
		pthread_mutex_destroy(&hub->dongles[num].mutex_dongle);
		num++;
	}
}

void	free_dongle_cond(t_hub *hub, t_uint dnum)
{
	t_uint	num;

	num = 0;
	while (num < dnum)
	{
		pthread_cond_destroy(&hub->dongles[num].cond_dongle);
		num++;
	}
}

void	free_coder_mtx(t_hub *hub, t_uint cnum)
{
	t_uint	num;

	num = 0;
	while (num < cnum)
	{
		pthread_mutex_destroy(&hub->coders[num].mutex_coder);
		num++;
	}
}

void	free_sim_mtx(t_hub *hub)
{
	pthread_mutex_destroy(&hub->simctrl.mutex_sim);
}

void	free_sim_cond(t_hub *hub)
{
	pthread_cond_destroy(&hub->simctrl.cond_sim);
}


