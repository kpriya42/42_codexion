/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:06:23 by kri-              #+#    #+#             */
/*   Updated: 2026/10/27 21:53:43 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	printf(" num_coders = %d\n", input.num_coders);
	printf(" burnout_time = %lu ms\n", input.burnout_time);
	printf(" compile_time = %lu ms\n", input.compile_time);
	printf(" debug_time = %lu ms\n", input.debug_time);
	printf(" refactor time = %lu ms\n", input.refactor_time);
	printf(" num_compiles = %lu\n", input.num_compiles);
	printf(" cooldown_time = %lu ms\n", input.cooldown_time);
	printf(" scheduler = %d\n", input.scheduler);
	return (input);
}

int	main(int argc, char *argv[])
{
	t_hub	hub;

	hub.input = validate_args(--argc, argv);
	initialize_hub(&hub);
	create_threads_start_sim(&hub);
	join_threads(&hub);
	free_all(&hub, hub.input.num_coders);
	return (0);
}
