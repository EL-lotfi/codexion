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
  if (!coder->f_dongle)
  {
    // clean_up()
    return (0);
  }
  coder->f_dongle->in_use = FALSE;
  coder->table = table;
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
  return (first_coder);
}
