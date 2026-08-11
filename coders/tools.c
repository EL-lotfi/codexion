/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:15:32 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/11 15:31:45 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

long long count_elapsed_time(t_table *table)
{
  return (get_current_time_ms() - table->start_time);
}

long long get_current_time_ms()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

t_bool is_dongle_available(t_coder *coder)
{
  if (coder->f_dongle->last_use_time == 0)
    return (TRUE);
  while (
      !coder->table->is_simulation_over &&
      !coder->table->burnout_detected &&
      get_current_time_ms() - coder->f_dongle->last_use_time < coder->table->dongle_cooldown)
    ;
  return (TRUE);
}
