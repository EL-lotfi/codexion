/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 17:46:04 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/11 16:45:27 by ibel-lot         ###   ########.fr       */
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
  table->time_to_compile = 101;
  table->time_to_debug = 200;
  table->time_to_refactor = 100;
  table->time_to_burnout = 100;
  table->nbr_compiles_required = 5;
  table->dongle_cooldown = 9;
  table->scheduler_type = FIFO;
  table->burnout_detected = FALSE;
  table->is_simulation_over = FALSE;
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
  if (table->is_simulation_over)
    return (0);
  if (table->burnout_detected)
    return (1);
  pthread_join(monitor, NULL);
}
