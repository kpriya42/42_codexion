/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:06:23 by kri-              #+#    #+#             */
/*   Updated: 2026/09/23 20:20:33 by kri-             ###   ########.fr       */
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

bool	is_not_a_num(int argc, char *argv[])
{
	size_t	i;

	while (--argc)
	{
		i = 0;
		while (argv[argc][i])
		{
			if (!(argv[argc][i] >= '0' && argv[argc][i] <= '9'))
				return (true);
			i++;
		}
	}
	return (false);
}


t_input	validate_args(int argc, char *argv[])
{
	t_input	input;

	if (argc != MAX_INPUT || is_not_a_num(argc, argv)
		|| (strcmp(argv[argc], "fifo") && (strcmp(argv[argc], "edf"))))
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
	t_hub	hub;

	input = validate_args(--argc, argv);
	start_working(input, hub);

	printf("%d\n", input.num_coders);
	printf("%d\n", input.burnout_time);
	printf("%d\n", input.compile_time);
	printf("%d\n", input.debug_time);
	printf("%d\n", input.num_compiles);
	printf("%d\n", input.cooldown_time);
	printf("%d\n", input.scheduler);
	return (0);
}
