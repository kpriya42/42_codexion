/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:06:23 by kri-              #+#    #+#             */
/*   Updated: 2026/09/29 14:57:21 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
		write(STDOUT_FILENO, INPUT_ERR_MSG, strlen(INPUT_ERR_MSG));
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
	t_hub	hub;

	hub.input = validate_args(--argc, argv);
	initialize_hub(&hub);

	printf("%d\n", hub.input.num_coders);
	printf("%d\n", hub.input.burnout_time);
	printf("%d\n", hub.input.compile_time);
	printf("%d\n", hub.input.debug_time);
	printf("%d\n", hub.input.num_compiles);
	printf("%d\n", hub.input.cooldown_time);
	printf("%d\n", hub.input.scheduler);
	return (0);
}
