/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 10:25:57 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/24 10:30:44 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

long long	get_table_attribute(t_table *table, t_attribute attribute)
{
	long long	result;

	result = 0;
	pthread_mutex_lock(&table->get_mutex);
	if (attribute == time_to_compile)
		result = table->time_to_compile;
	if (attribute == time_to_burnout)
		result = table->time_to_burnout;
	if (attribute == time_to_debug)
		result = table->time_to_debug;
	if (attribute == time_to_refactor)
		result = table->time_to_refactor;
	if (attribute == nbr_compiles_required)
		result = table->nbr_compiles_required;
	if (attribute == dongle_cooldown)
		result = table->dongle_cooldown;
	if (attribute == burnout_detected)
		result = table->burnout_detected;
	if (attribute == start_time)
		result = table->start_time;
	if (attribute == scheduler_type)
		result = table->scheduler_type;
	pthread_mutex_unlock(&table->get_mutex);
	return (result);
}

long long	count_elapsed_time(t_table *table)
{
	return (get_current_time_ms() - get_table_attribute(table, start_time));
}

long long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

t_bool	is_dongle_available(t_dongle *dongle)
{
	int			d_cooldown;
	t_bool		burnout_dt;

	burnout_dt = get_table_attribute(dongle->table, burnout_detected);
	d_cooldown = get_table_attribute(dongle->table, dongle_cooldown);
	if (dongle->last_use_time == 0)
		return (TRUE);
	while (!burnout_dt && get_current_time_ms() - dongle->last_use_time
		< d_cooldown)
		usleep(100);
	return (TRUE);
}
