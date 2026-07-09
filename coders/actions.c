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

t_bool affect_dongle(t_coder *coder, char side)
{
  if (side == LEFT)
  {
    pthread_mutex_lock(&coder->prev->f_dongle->mutex)
    coder->l_dongle = coder->prev->f_dongle;
    coder->l_dongle->in_use = True;
    pthread_mutex_unlock(&coder->prev->f_dongle->mutex)
  }
  else
  {
    pthread_mutex_lock(&coder->f_dongle->mutex)
    coder->r_dongle = coder->f_dongle;
    coder->r_dongle->in_use = True;
    pthread_mutex_unlock(&coder->f_dongle->mutex)
  }
}

t_bool detach_dongle(t_coder *coder, char side)
{
  if (side == LEFT)
  {
    pthread_mutex_lock(&coder->prev->f_dongle->mutex)
    coder->l_dongle = coder->prev->f_dongle;
    coder->l_dongle->in_use = False;
    pthread_mutex_unlock(&coder->prev->f_dongle->mutex)
  }
  else
  {
    pthread_mutex_lock(&coder->prev->f_dongle->mutex)
    coder->r_dongle = coder->prev->f_dongle;
    coder->r_dongle->in_use = True;
    pthread_mutex_unlock(&coder->prev->f_dongle->mutex)
  }
}
