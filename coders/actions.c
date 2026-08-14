/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 14:54:30 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/08 17:39:43 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void affect_dongle(t_coder *coder, t_side side)
{
  t_dongle   *dongle;

  if (side == LEFT)
    dongle = coder->l_dongle; 
  else
    dongle = coder->r_dongle;;
  pthread_mutex_lock(&dongle->mutex);
  if (side == LEFT)
    dongle->r_request = TRUE;
  else
    dongle->l_request = TRUE;
  if (dongle->table->scheduler_type == FIFO)
    redefine_priority(dongle);
  while ( !is_dongle_available(dongle)
      || dongle->queue[0] != coder->id_coder)
    pthread_cond_wait(&dongle->dongle_cond, &dongle->mutex);
  if (!coder->table->burnout_detected)
    print_process(coder, TAKING_DONGLES);
  pthread_mutex_unlock(&dongle->mutex);
}

void detach_dongle(t_coder *coder, t_side side)
{
  t_dongle   *dongle;

  if (side == LEFT)
    dongle = coder->l_dongle;
  else 
    dongle = coder->r_dongle;
  pthread_mutex_lock(&dongle->mutex);
  if (side == LEFT)
    dongle->r_request = FALSE;
  else 
    dongle->l_request = FALSE;
  redefine_priority(dongle);
  pthread_cond_signal(&dongle->dongle_cond);
  pthread_mutex_unlock(&dongle->mutex);
}

void swap_queue(int *queue)
{
  int   tmp;

  tmp = queue[0];
  queue[0] = queue[1];
  queue[1] = tmp;
}
