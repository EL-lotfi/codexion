/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 17:46:04 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/10 14:36:34 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

// void clean_up(t_coder *first_coder)
// {
//   t_coder     *current_coder;
//   t_bool first;
//
//   current_coder = first_coder;
//   first = TRUE;
//   current_coder = first_coder;
//   while (current_coder != first_coder  ||  first)
//   { 
//     current_coder = current_coder->nxt_coder;
//     first = FALSE;
//   }
// }

t_bool is_simulation_over(t_coder *first_coder)
{
  t_coder   *current_coder;
  t_bool    first;

  first = TRUE;
  current_coder = first_coder;
  while (current_coder != first_coder  ||  first)
  { 
    if (current_coder->cmp_count < current_coder->table->nbr_compiles_required)
      return (FALSE);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
  return (TRUE);
}

t_bool is_burned_out(t_coder *coder)
{
  if (get_current_time_ms() - coder->last_readiness_time < coder->table->time_to_burnout)
    return (FALSE);
  return (TRUE);
}

void *monitor_routine(void *param)
{
  t_coder *first_coder;
  t_coder *current_coder;
  t_bool  first;

  first_coder = (t_coder *)param;
  current_coder = first_coder;
  while (TRUE)
  { 
    if (is_simulation_over(first_coder))
    {
      first_coder->table->is_simulation_over = TRUE;
      return (NULL);
      
    }
    if (is_burned_out(current_coder))
    {
      // clean_up(first_coder);
      pthread_mutex_lock(&current_coder->table->print_mutex);
      printf("%lld\t%d\tis burned out\n", count_elapsed_time(current_coder->table), current_coder->id_coder);
      pthread_mutex_unlock(&current_coder->table->print_mutex);
      first_coder->table->burnout_detected = TRUE;
      return (NULL);
    }
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
}

int main()
{
  pthread_t monitor;
  t_table   *table;
  t_coder   *first_coder;
  t_coder   *current_coder;
  t_bool    first;
  void      *monitor_result;

  table = malloc(sizeof(t_table));
  table->number_of_coders = 3;
  table->time_to_compile = 100;
  table->time_to_debug = 200;
  table->time_to_refactor = 100;
  table->time_to_burnout = 100;
  table->nbr_compiles_required = 5;
  table->dongle_cooldown = 9;
  table->scheduler_type = FIFO;
  table->start_time = get_current_time_ms();
  if(pthread_mutex_init(&table->print_mutex, NULL) != 0)
  {
    // clean_up();
    return 0;
  }
  first_coder = create_coders(table);
  current_coder = first_coder;
  first = TRUE;
  while (current_coder != first_coder  ||  first)
  { 
    pthread_create(&current_coder->coder_thread, NULL, coder_routine, current_coder);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
  pthread_create(&monitor, NULL, monitor_routine, first_coder);
  first = TRUE;
  current_coder = first_coder;
  while (current_coder != first_coder || first)
  { 
    pthread_join(current_coder->coder_thread, NULL);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
  pthread_join(monitor, NULL);
}
