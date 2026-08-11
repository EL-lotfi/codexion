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

void compile_process(t_coder *coder)
{
  long long compile_start;

  print_process(coder, COMPILING);
  compile_start = get_current_time_ms();
  while (get_current_time_ms() - compile_start < coder->table->time_to_compile)
    ;
  coder->f_dongle->last_use_time = get_current_time_ms();
  coder->prv_coder->f_dongle->last_use_time = get_current_time_ms();
  coder->cmp_count += 1; 
}

void debug_process(t_coder *coder)
{
  long long debug_start;

  print_process(coder, DEBUGGING);
  debug_start = get_current_time_ms();
  while (get_current_time_ms() - debug_start < coder->table->time_to_debug)
    ;
}

void refactor_process(t_coder *coder)
{
  long long refactor_start;

  print_process(coder, REFACTORING);
  refactor_start = get_current_time_ms();
  while (get_current_time_ms() - refactor_start < coder->table->time_to_refactor)
    ;
}

void *coder_routine(void *param)
{
  t_coder *coder;
  coder = (t_coder *)param;

  if (coder->table->is_simulation_over == TRUE || coder->table->burnout_detected == TRUE)
    return NULL);
  while (coder->cmp_count < coder->table->nbr_compiles_required)
  {
    if (coder->id_coder % 2 ==  0)
    {
      affect_dongle(coder, RIGHT);
      affect_dongle(coder, LEFT);
    }
    else
    {
      affect_dongle(coder, LEFT);
      affect_dongle(coder, RIGHT);
    }
    if (coder->table->scheduler_type == EDF)
      coder->last_readiness_time = get_current_time_ms();
    compile_process(coder);
    detach_dongle(coder, RIGHT);
    detach_dongle(coder, LEFT);
    debug_process(coder);
    refactor_process(coder);
  }
  return (NULL);
}
