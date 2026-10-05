/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 12:53:16 by kri-              #+#    #+#             */
/*   Updated: 2026/10/05 16:18:36 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	display_err(char *msg)
{
	write(STDERR_FILENO, msg, strlen(*msg));
	exit(1);
}

unsigned long	get_time_ms(void)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
	return ((curr_time.tv_sec * 1000) + (curr_time.tv_usec / 1000));
}

//int	main(void)
//{
//	long	start_time;
//	long	curr_time;

//	start_time = get_time_ms();
//	usleep(1000000);
//	curr_time = get_time_ms();
//	printf("Time difference = %ld ms", curr_time - start_time);
//}