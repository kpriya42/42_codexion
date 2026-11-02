/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kri- <kri-@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 13:06:23 by kri-              #+#    #+#             */
/*   Updated: 2026/11/02 17:41:17 by kri-             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	display_err(char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
	exit(1);
}

static t_ulong	ft_atol(const char *nptr)
{
	size_t	i;
	t_ulong	sum;

	i = 0;
	sum = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		sum = sum * 10 + (nptr[i] - '0');
		i++;
	}
	return (sum);
}

static bool	is_invalid_num(int argc, char *argv[])
{
	char	*str;
	t_uint	i;

	while (--argc > 0)
	{
		str = argv[argc];
		i = 0;
		while (str[i])
		{
			if (str[i] < '0' || str[i] > '9')
				return (true);
			i++;
		}
		if ((ft_atol(argv[argc]) == 0 && argc != 7)
			|| ft_atol(argv[argc]) > UINT_MAX)
			return (true);
	}
	if (ft_atol(argv[1]) > MAX_CODERS || ft_atol(argv[6]) > UINT_MAX)
		return (true);
	return (false);
}

static t_err	validate_args(int argc, char *argv[], t_input *input )
{
	if (argc != MAX_ARGS
		|| is_invalid_num(argc, argv)
		|| (strcmp(argv[argc], "fifo") && (strcmp(argv[argc], "edf"))))
		return (INVALID_INPUT);
	if (!strcmp(argv[argc], "fifo"))
		input->scheduler = fifo;
	else
		input->scheduler = edf;
	input->cooldown_time = atoi(argv[--argc]);
	input->num_compiles = atoi(argv[--argc]);
	input->refactor_time = atoi(argv[--argc]);
	input->debug_time = atoi(argv[--argc]);
	input->compile_time = atoi(argv[--argc]);
	input->burnout_time = atoi(argv[--argc]);
	input->num_coders = atoi(argv[--argc]);
	return (NO_ERR);
}

int	main(int argc, char *argv[])
{
	t_hub	hub;

	if (validate_args(--argc, argv, &hub.input) != NO_ERR)
		display_err(INPUT_ERR_MSG);
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
