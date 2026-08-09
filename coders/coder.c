/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 14:54:22 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/08 13:36:43 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void init_queue(t_coder *coder)
{
    if (!coder->f_dongle->queue)
    {
      // clean_up();
      return ;
    }
    if (coder->id_coder % 2 ==  0)
    {
      coder->f_dongle->queue[0] = coder->id_coder;
      coder->f_dongle->queue[1] = coder->nxt_coder->id_coder;
    }
    else
    {
      coder->f_dongle->queue[0] = coder->nxt_coder->id_coder;
      coder->f_dongle->queue[1] = coder->id_coder;
    }
}

void init_requests(t_coder *first_coder)
{
  t_coder   *current_coder;
  t_bool    first;

  first = TRUE;
  current_coder = first_coder;
  while (current_coder != first_coder  ||  first)
  {
    if (current_coder->id_coder % 2 ==  0)
    {
      current_coder->f_dongle->l_request = TRUE;
      current_coder->f_dongle->r_request = TRUE;
    }
    else
    {
      current_coder->f_dongle->l_request = FALSE;
      current_coder->f_dongle->r_request = FALSE;
    }
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
}

void init_queues(t_coder *first_coder)
{
  t_coder   *current_coder;
  t_bool    first;

  first = TRUE;
  current_coder = first_coder;
  while (current_coder != first_coder  ||  first)
  {
    init_queue(current_coder);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
  if (first_coder->table->scheduler_type == FIFO)
    init_requests(first_coder);
}

t_coder *coder_init(t_table *table)
{
  t_coder *coder;

  coder = malloc(sizeof(t_coder));
  if (!coder)
  {
    // clean_up()
    return (0);
  }
  coder->cmp_count = 0;
  coder->f_dongle = malloc(sizeof(t_dongle));
  coder->table = table;
  coder->table = table;
  coder->f_dongle->queue = malloc(2 * sizeof(int));
  coder->last_readiness_time = get_current_time_ms();
  if (!coder->f_dongle)
  {
    // clean_up()
    return (0);
  }
  if(pthread_mutex_init(&coder->f_dongle->mutex, NULL) != 0)
  {
    // clean_up();
    return 0;
  }
  if(pthread_cond_init(&coder->f_dongle->dongle_cond, NULL) != 0)
  {
    // clean_up();
    return 0;
  }
  return (coder);
}

t_coder *create_coders(int nbr_coders, t_table *table)
{
  t_coder *first_coder;
  t_coder *last_coder;
  t_coder *new_coder;
  int     id_coder;

  first_coder = coder_init(table);
  first_coder->id_coder = 1;
  first_coder->nxt_coder = first_coder;
  first_coder->prv_coder = first_coder;
  last_coder = first_coder;
  id_coder = 2;
  while (--nbr_coders)
  {
    while (last_coder->nxt_coder != first_coder)
      last_coder = last_coder->nxt_coder;
    new_coder = coder_init(table);
    new_coder->nxt_coder = first_coder;
    first_coder->prv_coder = new_coder;
    last_coder->nxt_coder = new_coder;
    new_coder->prv_coder = last_coder;
    new_coder->id_coder = id_coder;
    id_coder++;
  }
  init_queues(first_coder);
  return (first_coder);
}





// void edf_priority(t_coder *coder)
// {
//   if (coder->f_dongle->queue[0] == coder->id_coder &&
//       coder->last_readiness_time < coder->nxt_coder->last_readiness_time
//       && (coder->nxt_coder->cmp_count == coder->table->nbr_compiles_required
//     || coder->cmp_count != coder->table->nbr_compiles_required))
//     swap_queue(coder->f_dongle->queue);
//   if (coder->prv_coder->f_dongle->queue[0] == coder->id_coder &&
//       coder->last_readiness_time < coder->prv_coder->last_readiness_time
//       && (coder->prv_coder->cmp_count == coder->table->nbr_compiles_required
//       || coder->cmp_count != coder->table->nbr_compiles_required))
//     swap_queue(coder->prv_coder->f_dongle->queue);
// }
