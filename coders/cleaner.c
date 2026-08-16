/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 14:01:42 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/16 21:32:49 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void  clean_up(t_coder *first_coder)
{
  t_coder *current_coder;
  t_coder *prev_coder;
  int     nbr_coders;

  nbr_coders = first_coder->table->number_of_coders;

  current_coder = first_coder;
	while (nbr_coders--)
	{
    prev_coder = current_coder;
    current_coder = current_coder->nxt_coder;
    free(prev_coder->r_dongle->queue);
    pthread_mutex_destroy(&prev_coder->r_dongle->dongle_mutex);
    pthread_cond_destroy(&prev_coder->r_dongle->dongle_cond);
    free(prev_coder->r_dongle);
    free(prev_coder);
	}
}
