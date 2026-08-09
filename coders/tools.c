/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:15:32 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/09 18:49:19 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

long long count_elapsed_time(t_table *table)
{
  return (get_current_time_ms() - table->start_time);
}

long long get_current_time_ms()
{
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void edf_priority(t_coder *coder)
{
  if (coder->prv_coder->f_dongle->queue[0] != coder->id_coder &&
      coder->last_readiness_time < coder->prv_coder->last_readiness_time
      && (coder->prv_coder->cmp_count == coder->table->nbr_compiles_required
      || coder->cmp_count != coder->table->nbr_compiles_required))
    swap_queue(coder->prv_coder->f_dongle->queue);
  else if (coder->f_dongle->queue[0] != coder->id_coder &&
      coder->last_readiness_time < coder->nxt_coder->last_readiness_time
      && (coder->nxt_coder->cmp_count == coder->table->nbr_compiles_required
    || coder->cmp_count != coder->table->nbr_compiles_required))
    swap_queue(coder->f_dongle->queue);
}

void fifo_priority(t_coder *coder)
{
  if (coder->f_dongle->r_request == FALSE && 
      coder->f_dongle->l_request ==  TRUE &&
      coder->f_dongle->queue[0] != coder->id_coder)
    swap_queue(coder->f_dongle->queue);
  if (coder->f_dongle->r_request == TRUE && 
      coder->f_dongle->l_request ==  FALSE &&
      coder->f_dongle->queue[0] == coder->id_coder)
    swap_queue(coder->f_dongle->queue);
}

void redefine_priority(t_coder *coder)
{
  if (coder->table->scheduler_type == EDF)
    edf_priority(coder);
  else if (coder->table->scheduler_type == FIFO)
    fifo_priority(coder);
}
