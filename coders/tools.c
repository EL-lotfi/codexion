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

long long count_elapsed_time()
{
  return (get_current_time_ms() - table.start_time);
}

void print_process(t_process process)
{
  if (process == GETTING_DONGLES)
    printf("%lld/t%d/t/has taken a dongle", count_elapsed_time(), coder->id_coder);
  else if (process == COMPILING)
    printf("%lld/t%d/t/is compiling", count_elapsed_time(), coder->id_coder);
  else if (process == DEBUGGING)
    printf("%lld/t%d/t/is debigging", count_elapsed_time(), coder->id_coder);
  else if (process == REFACTORING)
    printf("%lld/t%d/t/is refactoring", count_elapsed_time(), coder->id_coder);
}

long long get_current_time_ms()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
