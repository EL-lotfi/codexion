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

int main()
{
  t_table *table;
  t_coder *first_coder;
  t_coder *current_coder;
  t_bool  first;

  table = malloc(sizeof(t_table));
  table->time_to_compile = 300;
  table->time_to_debug = 200;
  table->time_to_refactor = 100;
  table->start_time = get_current_time_ms();
  table->nbr_compiles_required = 5;
  table->dongle_cooldown = 400;
  table->scheduler_type = FIFO;
  if(pthread_mutex_init(&table->print_mutex, NULL) != 0)
  {
    // clean_up();
    return 0;
  }
  first_coder = create_coders(3, table);
  current_coder = first_coder;
  first = TRUE;
  while (current_coder != first_coder  ||  first)
  { 
    pthread_create(&current_coder->coder_thread, NULL, coder_routine, current_coder);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
  first = TRUE;
  current_coder = first_coder;
  while (current_coder != first_coder || first)
  { 
    pthread_join(current_coder->coder_thread, NULL);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
}
