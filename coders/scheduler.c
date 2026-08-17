/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/09 16:15:32 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/15 10:19:20 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

static void	edf_priority(t_dongle *dongle)
{
	if (dongle->queue[0] != dongle->l_coder->id_coder
		&& dongle->l_coder->last_compile_start
		<= dongle->r_coder->last_compile_start
		&& (dongle->r_coder->cmp_count
			== dongle->table->nbr_compiles_required
		  || dongle->l_coder->cmp_count
			!= dongle->r_coder->table->nbr_compiles_required))
		swap_queue(dongle->queue);
	else if (dongle->queue[0] != dongle->r_coder->id_coder
		&& dongle->r_coder->last_compile_start
		<= dongle->l_coder->last_compile_start
		&& (dongle->l_coder->cmp_count
			== dongle->table->nbr_compiles_required
		|| dongle->r_coder->cmp_count
			!= dongle->table->nbr_compiles_required))
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
	if (dongle->table->scheduler_type == EDF)
		edf_priority(dongle);
	else if (dongle->table->scheduler_type == FIFO)
		fifo_priority(dongle);
}

