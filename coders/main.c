/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:06:23 by kri-              #+#    #+#             */
/*   Updated: 2026/10/28 19:33:37 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	display_err(char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
	exit(1);
}

bool	is_invalid_num(int argc, char *argv[])
{
	while (--argc > 0)
	{
		if (atoi(argv[argc]) < 0)
			return (true);
	}
	if (atoi(argv[1]) >= MAX_CODERS || atoi(argv[1]) <= 0)
		return (true);
	return (false);
}

t_input	validate_args(int argc, char *argv[])
{
	t_input	input;

	if (argc != MAX_INPUT || is_invalid_num(argc, argv)
		|| (strcmp(argv[argc], "fifo") && (strcmp(argv[argc], "edf"))))
		display_err(INPUT_ERR_MSG);
	if (!strcmp(argv[argc], "fifo"))
		input.scheduler = fifo;
	else
		input.scheduler = edf;
	input.cooldown_time = atoi(argv[--argc]);
	input.num_compiles = atoi(argv[--argc]);
	input.refactor_time = atoi(argv[--argc]);
	input.debug_time = atoi(argv[--argc]);
	input.compile_time = atoi(argv[--argc]);
	input.burnout_time = atoi(argv[--argc]);
	input.num_coders = atoi(argv[--argc]);
	return (input);
}

int	main(int argc, char *argv[])
{
	t_hub	hub;

	hub.input = validate_args(--argc, argv);
	if (initialize_hub(&hub) != NO_ERR)
		display_err(INIT_ERR);
	if (create_threads_start_sim(&hub) != NO_ERR)
	{
		destroy_all(&hub);
		display_err(INIT_ERR);
	}
	join_threads(&hub);
	destroy_all(&hub);
	return (0);
}
