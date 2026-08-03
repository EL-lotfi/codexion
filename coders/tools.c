/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:15:32 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/09 18:49:19 by ibel-lot         ###   ########.fr       */
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

void print_process(t_coder *coder, t_process process)
{
  pthread_mutex_lock(&coder->table->print_mutex);
  if (process == TAKING_DONGLES)
    printf("%lld\t%d\thas taken a dongle\n", count_elapsed_time(coder->table), coder->id_coder);
  else if (process == COMPILING)
    printf("%lld\t%d\tis compiling\n", count_elapsed_time(coder->table), coder->id_coder);
  else if (process == DEBUGGING)
    printf("%lld\t%d\tis debugging\n", count_elapsed_time(coder->table), coder->id_coder);
  else if (process == REFACTORING)
    printf("%lld\t%d\tis refactoring\n", count_elapsed_time(coder->table), coder->id_coder);
  pthread_mutex_unlock(&coder->table->print_mutex);
}
