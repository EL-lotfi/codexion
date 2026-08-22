/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:15:32 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/21 16:21:37 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

long long get_last_compile_start(t_coder *coder)
{
  long long last_compile_start;


  pthread_mutex_lock(&coder->coder_mutex);
  last_compile_start = coder->last_compile_start;
  pthread_mutex_unlock(&coder->coder_mutex);
  return  (last_compile_start);
}

static void	edf_priority(t_dongle *dongle, int l_coder_count, int r_coder_count)
{
  long long l_last_compile_start;
  long long r_last_compile_start;
  int       n_compile_req;

  l_last_compile_start = get_last_compile_start(dongle->l_coder);
  r_last_compile_start = get_last_compile_start(dongle->r_coder);
  n_compile_req = get_table_attribute(dongle->table, nbr_compiles_required);
	if (dongle->queue[0] != dongle->l_coder->id_coder
		&& l_last_compile_start <= r_last_compile_start
		&& (r_coder_count == n_compile_req
		  || l_coder_count != n_compile_req))
		swap_queue(dongle->queue);
	else if (dongle->queue[0] != dongle->r_coder->id_coder
		&& r_last_compile_start <= l_last_compile_start
    && (l_coder_count == n_compile_req
		|| r_coder_count != n_compile_req))
		swap_queue(dongle->queue);
}

static void	fifo_priority(t_dongle *dongle)
{
	if (dongle->queue[0] != dongle->l_coder->id_coder
		&& dongle->r_request == FALSE
		&& dongle->l_request == TRUE)
		swap_queue(dongle->queue);
	else if (dongle->queue[0] != dongle->r_coder->id_coder
		&& dongle->l_request == FALSE
		&& dongle->r_request == TRUE)
		swap_queue(dongle->queue);
}

void	redefine_priority(t_dongle *dongle)
{
  int   l_coder_count;
  int   r_coder_count;

  pthread_mutex_lock(&dongle->l_coder->coder_mutex);
  l_coder_count = dongle->l_coder->cmp_count;
  pthread_mutex_unlock(&dongle->l_coder->coder_mutex);
  pthread_mutex_lock(&dongle->r_coder->coder_mutex);
  r_coder_count = dongle->r_coder->cmp_count;
  pthread_mutex_unlock(&dongle->r_coder->coder_mutex);
	if (get_table_attribute(dongle->table, scheduler_type) == EDF)
		edf_priority(dongle, l_coder_count, r_coder_count);
	else if (get_table_attribute(dongle->table, scheduler_type) == FIFO)
		fifo_priority(dongle);
}
