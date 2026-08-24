/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:26:17 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/24 11:18:49 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	print_process(t_coder *coder, t_process process)
{
	pthread_mutex_lock(&coder->table->print_mutex);
	if (coder->table->burnout_detected == TRUE)
	{
		pthread_mutex_unlock(&coder->table->print_mutex);
		return ;
	}
	if (process == TAKING_DONGLES)
		printf("%lld %d has taken a dongle\n",
			count_elapsed_time(coder->table),
			coder->id_coder);
	else if (process == COMPILING)
		printf("%lld %d is compiling\n",
			count_elapsed_time(coder->table),
			coder->id_coder);
	else if (process == DEBUGGING)
		printf("%lld %d is debugging\n",
			count_elapsed_time(coder->table),
			coder->id_coder);
	else if (process == REFACTORING)
		printf("%lld %d is refactoring\n",
			count_elapsed_time(coder->table),
			coder->id_coder);
	pthread_mutex_unlock(&coder->table->print_mutex);
}

void	compile_process(t_coder *coder)
{
	long long	compile_start;
	long long	tm_to_compile;
	t_bool		burnout_dt;

	tm_to_compile = get_table_attribute(coder->table, time_to_compile);
	burnout_dt = get_table_attribute(coder->table, burnout_detected);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->last_compile_start = get_current_time_ms();
	pthread_mutex_unlock(&coder->coder_mutex);
	print_process(coder, COMPILING);
	compile_start = get_current_time_ms();
	while (!burnout_dt && get_current_time_ms() - compile_start < tm_to_compile)
		usleep(200);
	pthread_mutex_lock(&coder->l_dongle->dongle_mutex);
	coder->l_dongle->last_use_time = get_current_time_ms();
	pthread_mutex_unlock(&coder->l_dongle->dongle_mutex);
	pthread_mutex_lock(&coder->r_dongle->dongle_mutex);
	coder->r_dongle->last_use_time = get_current_time_ms();
	pthread_mutex_unlock(&coder->r_dongle->dongle_mutex);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->cmp_count += 1;
	pthread_mutex_unlock(&coder->coder_mutex);
}

void	debug_process(t_coder *coder)
{
	long long	debug_start;
	long long	burnout_dt;
	long long	tm_to_debug;

	tm_to_debug = get_table_attribute(coder->table, time_to_debug);
	burnout_dt = get_table_attribute(coder->table, burnout_detected);
	print_process(coder, DEBUGGING);
	debug_start = get_current_time_ms();
	while (!burnout_dt && get_current_time_ms() - debug_start < tm_to_debug)
		usleep(100);
	return ;
}

void	refactor_process(t_coder *coder)
{
	long long	refactor_start;
	long long	tm_refactor;
	t_bool		burnout_dt;

	tm_refactor = get_table_attribute(coder->table, time_to_refactor);
	burnout_dt = get_table_attribute(coder->table, burnout_detected);
	print_process(coder, REFACTORING);
	refactor_start = get_current_time_ms();
	while (!burnout_dt && get_current_time_ms() - refactor_start < tm_refactor)
		usleep(100);
	return ;
}

void	*coder_routine(void *param)
{
	int			n_compile_req;
	t_coder		*coder;

	coder = (t_coder *)param;
	n_compile_req = get_table_attribute(coder->table, nbr_compiles_required);
	while (!get_table_attribute(coder->table, burnout_detected)
		&& coder->cmp_count < n_compile_req)
	{
		if (coder->id_coder % 2 == 0)
		{
			affect_dongle(coder, RIGHT);
			affect_dongle(coder, LEFT);
		}
		else
		{
			affect_dongle(coder, LEFT);
			affect_dongle(coder, RIGHT);
		}
		compile_process(coder);
		detach_dongle(coder, RIGHT);
		detach_dongle(coder, LEFT);
		debug_process(coder);
		refactor_process(coder);
	}
	return (NULL);
}
