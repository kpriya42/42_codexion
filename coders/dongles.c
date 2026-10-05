/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:41:35 by kri-              #+#    #+#             */
/*   Updated: 2026/10/05 17:40:46 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	dongle_init(t_dongle *dongle, t_input *input)
{
	dongle->cooldown_time = input->cooldown_time;
	dongle->is_being_used = false;
	dongle->scheduler = input->scheduler;
}
