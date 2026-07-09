/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 14:54:35 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/09 18:51:30 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "coder.h"

void compile_process(t_coder *coder, t_table *table, long long time)
{
  long long compile_start;

  print_process(COMPILING);
  compile_start = get_current_time_ms();
  while (get_current_time_ms() - compile_start < table.time_to_compile)
    ;
  coder->compile_count += 1; 
}

void debug_process(t_coder *coder, t_table *table, long long time)
{
  long long debug_start;

  print_process(DEBUGGING);
  compile_start = get_current_time_ms();
  while (get_current_time_ms() - compile_start < table.time_to_compile)
    ;
}

void refactor_process(t_coder *coder, t_table *table, long long time)
{
  long long refactor_start;

  print_process(REFACTORING);
  refactor_start = get_current_time_ms();
  while (get_current_time_ms() - refactor_start < table.time_to_compile)
    ;
}

void coder_routine(t_coder *coder)
{
  // affect_dongle(coder, "r");
  // affect_dongle(coder, "l");
  compile_process(coder);
  pthread_mutex_lock(table.print_mutex);
  show_the_process(coder);
  pthread_mutex_unlock(table.print_mutex);
  debug_process(coder);
  refactor_process(coder);
}
