/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:06:23 by kri-              #+#    #+#             */
/*   Updated: 2026/10/05 16:41:26 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_invalid_num(int argc, char *argv[])
{
	while (--argc)
	{
		if (atoi(argv[argc]) < 1)
			return (true);
	}
	if (atoi(argv[1]) >= MAX_CODERS)
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
	input.debug_time = atoi(argv[--argc]);
	input.compile_time = atoi(argv[--argc]);
	input.burnout_time = atoi(argv[--argc]);
	input.num_coders = atoi(argv[--argc]);
	return (input);
}

int	main(int argc, char *argv[])
{
	t_hub	hub;
	t_input	input;

	input = validate_args(--argc, argv);
	initialize_hub(&hub, &input); // initialize coders, dongles, mutexes
	create_threads(); // create coder_threads
	join_threads();  // actual program simulation starts here
	free_and_exit();
	printf("%d\n", input.num_coders);
	printf("%d\n", input.burnout_time);
	printf("%d\n", input.compile_time);
	printf("%d\n", input.debug_time);
	printf("%d\n", input.num_compiles);
	printf("%d\n", input.cooldown_time);
	printf("%d\n", input.scheduler);
	return (0);
}
