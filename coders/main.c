/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 17:46:04 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/09 17:40:13 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "coder.h"

int main()
{
  t_table table;
  t_coder *first_coder;
  t_coder *current_coder;
  t_bool  first;

  first = TRUE;
  table.time_to_compile = 300;
  table.time_to_debug = 200;
  table.time_to_refactor = 100;
  table.start_time = get_current_time_ms();
  first_coder = create_coder(5);
  current_coder = first_coder;
  while (current_coder != first_coder || first)
  { 
    pthread_create(current_coder->coder_thread, NULL, coder_routine, current_coder);
    current_coder = current_coder->next;
    first = FALSE;
  }
}
