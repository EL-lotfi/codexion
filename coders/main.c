/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 17:46:04 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/15 00:00:00 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"
#include <string.h>

static t_bool	parse_positive_int(const char *argument, int *result)
{
	long long	value;
	int		index;

	if (!argument || argument[0] == '\0')
		return (FALSE);
	value = 0;
	index = 0;
	while (argument[index] != '\0')
	{
		if (argument[index] < '0' || argument[index] > '9')
			return (FALSE);
		value = value * 10 + (argument[index] - '0');
		if (value > INT_MAX)
			return (FALSE);
		index++;
	}
	if (value < 0)
		return (FALSE);
	*result = (int)value;
	return (TRUE);
}

static t_bool	parse_arguments(int argc, char **argv, t_table *table)
{
	if (argc != 9)
	{
		printf("Usage: %s arguments aren't valid\n", argv[0]);
		return (FALSE);
	}
	if (!parse_positive_int(argv[1], &table->number_of_coders))
		return (FALSE);
	if (!parse_positive_int(argv[2], &table->time_to_burnout))
		return (FALSE);
	if (!parse_positive_int(argv[3], &table->time_to_compile))
		return (FALSE);
	if (!parse_positive_int(argv[4], &table->time_to_debug))
		return (FALSE);
	if (!parse_positive_int(argv[5], &table->time_to_refactor))
		return (FALSE);
	if (!parse_positive_int(argv[6], &table->nbr_compiles_required))
		return (FALSE);
	if (!parse_positive_int(argv[7], &table->dongle_cooldown))
		return (FALSE);
	if (strcmp(argv[8], "FIFO") != 0 && strcmp(argv[8], "fifo") != 0)
	{
		if (strcmp(argv[8], "EDF") != 0 && strcmp(argv[8], "edf") != 0)
			return (FALSE);
		table->scheduler_type = EDF;
	}
	else
		table->scheduler_type = FIFO;
	table->burnout_detected = FALSE;
	table->is_simulation_over = FALSE;
	return (TRUE);
}

static int	create_and_join(t_coder *first_coder, pthread_t *monitor)
{
	t_coder	*current_coder;
	t_bool	first;

	current_coder = first_coder;
	first = TRUE;
	while (current_coder != first_coder || first)
	{
		if (pthread_create(&current_coder->coder_thread, NULL,
				coder_routine, current_coder) != 0)
			return (1);
		current_coder = current_coder->nxt_coder;
		first = FALSE;
	}
	if (pthread_create(monitor, NULL, monitor_routine, first_coder) != 0)
		return (1);
	first = TRUE;
	current_coder = first_coder;
	while (current_coder != first_coder || first)
	{
		pthread_join(current_coder->coder_thread, NULL);
		current_coder = current_coder->nxt_coder;
		first = FALSE;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	pthread_t	monitor;
	t_table		table;
	t_coder		*first_coder;

	if (!parse_arguments(argc, argv, &table))
		return (1);
	table.start_time = get_current_time_ms();
	if (pthread_mutex_init(&table.print_mutex, NULL) != 0)
		return (1);
	first_coder = create_coders(&table);
	if (!first_coder)
		return (1);
	if (create_and_join(first_coder, &monitor))
		return (1);
	if (table.is_simulation_over)
		return (0);
	if (table.burnout_detected)
		return (1);
	pthread_join(monitor, NULL);
	return (0);
}

