/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:06:23 by kri-              #+#    #+#             */
/*   Updated: 2026/09/07 17:48:26 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
number_of_coders 
time_to_burnout(ms)
time_to_compile(ms)
time_to_debug(ms)
time_to_refactor(ms)
number_of_compiles_required
dongle_cooldown(ms)
scheduler - fifo or edf
*/
t_input	validate_input(int argc, char *argv[])
{
	t_input	input;

	if (argc != MAX_INPUT || (strcmp(argv[argc], "fifo")
			&& (strcmp(argv[argc], "edf"))))
	{
		write(STDOUT_FILENO, ERROR_MSG, strlen(ERROR_MSG));
		exit(1);
	}
	input.scheduler = EDF;
	if (!strcmp(argv[argc], "fifo"))
		input.scheduler = FIFO;
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
	t_input	input;

	input = validate_input(--argc, argv);

	printf("%d\n", input.num_coders);
	printf("%d\n", input.burnout_time);
	printf("%d\n", input.compile_time);
	printf("%d\n", input.debug_time);
	printf("%d\n", input.num_compiles);
	printf("%d\n", input.cooldown_time);
	printf("%d\n", input.scheduler);
	return (0);
}
