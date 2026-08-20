/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 14:17:12 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/20 00:49:27 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	wake_everybody(t_coder *first_coder)
{
	t_coder	*current_coder;
	t_bool	first;

	current_coder = first_coder;
	first = TRUE;
	while (current_coder != first_coder || first)
	{
		pthread_cond_signal(&current_coder->l_dongle->dongle_cond);
		current_coder = current_coder->nxt_coder;
		first = FALSE;
	}
}

t_bool	check_simulation(t_coder *first_coder)
{
	t_coder	*current_coder;
	t_bool	first;

	first = TRUE;
	current_coder = first_coder;
	while (current_coder != first_coder || first)
	{
		if (current_coder->cmp_count
			< current_coder->table->nbr_compiles_required)
			return (FALSE);
		current_coder = current_coder->nxt_coder;
		first = FALSE;
	}
  pthread_mutex_lock(&current_coder->table->get_mutex);
	current_coder->table->simulation_over = TRUE;
  pthread_mutex_unlock(&current_coder->table->get_mutex);
	return (TRUE);
}

t_bool	is_burned_out(t_coder *coder)
{
  pthread_mutex_lock(&coder->coder_mutex);
	if (get_current_time_ms() - coder->last_compile_start
		< coder->table->time_to_burnout
    || coder->cmp_count == get_table_attribute(coder->table, nbr_compiles_required))
  {
    pthread_mutex_unlock(&coder->coder_mutex);
		return (FALSE);
  }
  pthread_mutex_unlock(&coder->coder_mutex);
  pthread_mutex_lock(&coder->table->get_mutex);
	coder->table->burnout_detected = TRUE;
  pthread_mutex_unlock(&coder->table->get_mutex);
	wake_everybody(coder);
	return (TRUE);
}

void	*monitor_routine(void *param)
{
	t_coder	*current_coder;

	current_coder = (t_coder *)param;
	while (TRUE)
	{
		if (check_simulation(current_coder))
			return (NULL);
		if (is_burned_out(current_coder))
		{
			pthread_mutex_lock(&current_coder->table->print_mutex);
			printf("%lld %d burned out\n",
				count_elapsed_time(current_coder->table),
				current_coder->id_coder);
			pthread_mutex_unlock(&current_coder->table->print_mutex);
			return (NULL);
		}
		current_coder = current_coder->nxt_coder;
    usleep(100);
	}
  return (NULL);
}
