/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 14:54:30 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/08 17:39:43 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

// t_bool coder_is_ready(t_coder *coder)
// {
//   if (code->dongle->in_use)
// }

void affect_dongle(t_coder *coder, t_side side)
{
  t_coder   *coder_holder;

  if (side == LEFT)
    coder_holder = coder->prv_coder; 
  else
    coder_holder = coder;
  pthread_mutex_lock(&coder_holder->f_dongle->mutex);
  if (coder_holder->table->scheduler_type == FIFO)
  {
    if (side == LEFT)
    {
      coder_holder->f_dongle->r_request = TRUE;
      redefine_priority(coder_holder);
    }
    else
    {
      coder_holder->f_dongle->l_request = TRUE;
      redefine_priority(coder_holder);
    }
  }
  while (coder_holder->f_dongle->queue[0] != coder->id_coder)
    pthread_cond_wait(&coder_holder->f_dongle->dongle_cond, &coder_holder->f_dongle->mutex);
  print_process(coder, TAKING_DONGLES);
  pthread_mutex_unlock(&coder_holder->f_dongle->mutex);
}

void detach_dongle(t_coder *coder, t_side side)
{
  long long   cooldown_tick;

  cooldown_tick = get_current_time_ms();
  if (side == LEFT)
  {
    pthread_mutex_lock(&coder->prv_coder->f_dongle->mutex);
    coder->prv_coder->f_dongle->r_request = FALSE;
    if (coder->table->scheduler_type == EDF)
      redefine_priority(coder->nxt_coder);
    while (get_current_time_ms() - cooldown_tick  < coder->table->dongle_cooldown)
      ;
    pthread_cond_signal(&coder->prv_coder->f_dongle->dongle_cond);
    pthread_mutex_unlock(&coder->prv_coder->f_dongle->mutex);
  }
  else
  {
    pthread_mutex_lock(&coder->f_dongle->mutex);
    coder->f_dongle->l_request = FALSE;
    if (coder->table->scheduler_type == EDF)
      redefine_priority(coder->nxt_coder);
    while (get_current_time_ms() - cooldown_tick  < coder->table->dongle_cooldown)
      ;
    pthread_cond_signal(&coder->f_dongle->dongle_cond);
    pthread_mutex_unlock(&coder->f_dongle->mutex);
  }
}

void swap_queue(int *queue)
{
  int   tmp;

  tmp = queue[0];
  queue[0] = queue[1];
  queue[1] = tmp;
}
