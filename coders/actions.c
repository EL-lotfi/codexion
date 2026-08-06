/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
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

void affect_dongle(t_coder *coder, char side)
{
  if (side == LEFT)
  {
    pthread_mutex_lock(&coder->prv_coder->f_dongle->mutex);
    while (coder->prv_coder->f_dongle->queue[0] != coder->id_coder)
      pthread_cond_wait(&coder->prv_coder->f_dongle->dongle_cond, &coder->prv_coder->f_dongle->mutex);
    // coder->prv_coder->f_dongle->in_use = TRUE;
    print_process(coder, TAKING_DONGLES);
    pthread_mutex_unlock(&coder->prv_coder->f_dongle->mutex);
  }
  else
  {
    pthread_mutex_lock(&coder->f_dongle->mutex);
    while (coder->f_dongle->queue[0] != coder->id_coder)
      pthread_cond_wait(&coder->f_dongle->dongle_cond, &coder->f_dongle->mutex);
    // coder->f_dongle->in_use = TRUE;
    print_process(coder, TAKING_DONGLES);
    pthread_mutex_unlock(&coder->f_dongle->mutex);
  }
}

void detach_dongle(t_coder *coder, char side)
{
  if (side == LEFT)
  {
    pthread_mutex_lock(&coder->prv_coder->f_dongle->mutex);
    redefine_priority(coder->prv_coder);
    pthread_cond_signal(&coder->prv_coder->f_dongle->dongle_cond);
    pthread_mutex_unlock(&coder->prv_coder->f_dongle->mutex);
  }
  else
  {
    pthread_mutex_lock(&coder->f_dongle->mutex);
    redefine_priority(coder);
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
