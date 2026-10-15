/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 19:12:54 by kri-              #+#    #+#             */
/*   Updated: 2026/10/14 19:13:20 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

void	create_coder_threads(pthread_t *thread_coder, t_coder *coder)
{
	pthread_create(thread_coder, NULL, coder_routine, coder);
}

