/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/29 12:53:16 by kri-              #+#    #+#             */
/*   Updated: 2026/10/26 13:19:52 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	display_err(char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
	exit(1);
}

t_ulong	get_time_ms(void)
{
	struct timeval	curr_time;

	gettimeofday(&curr_time, NULL);
	return ((curr_time.tv_sec * TO_MS) + (curr_time.tv_usec / TO_MS));
}

//static void	set_timeout(struct timespec *timeout, t_ulong end_time)
//{
//	timeout->tv_sec = end_time / 1000;
//	timeout->tv_nsec = (end_time % 1000) * 1000000;
//}

static t_state	get_simulation_status_locked(t_coder *coder)
{
	return (*coder->sim_status);
}

t_state	sleep_ms(t_coder *coder, t_ulong duration)
{
	struct timespec	timeout;
	t_ulong			end_time;
	t_state			sim_status;

	end_time = get_time_ms() + duration;
	timeout.tv_sec = end_time / 1000;
	timeout.tv_nsec = (end_time % 1000) * 1000000;
	pthread_mutex_lock(coder->mutex_sim);
	sim_status = get_simulation_status_locked(coder);
	while (sim_status == RUN && get_time_ms() < end_time)
	{
		pthread_cond_timedwait(coder->cond_sim,
			coder->mutex_sim, &timeout);
		sim_status = get_simulation_status_locked(coder);
	}
	pthread_mutex_unlock(coder->mutex_sim);
	return (sim_status);
}

//int	main(void)
//{
//	long	begin_time;
//	long	curr_time;

//	begin_time = get_time_ms();
//	usleep(1000000);
//	curr_time = get_time_ms();
//	printf("Time difference = %ld ms", curr_time - begin_time);
//}