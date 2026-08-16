/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 14:54:22 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/16 21:31:14 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

static void	init_queue(t_dongle *dongle)
{
	dongle->queue = malloc(2 * sizeof(int));
	if (!dongle->queue)
	{
		return ;
	}
	if (dongle->r_coder->id_coder % 2 == 0)
	{
		dongle->queue[0] = dongle->r_coder->id_coder;
		dongle->queue[1] = dongle->l_coder->id_coder;
	}
	else
	{
		dongle->queue[0] = dongle->l_coder->id_coder;
		dongle->queue[1] = dongle->r_coder->id_coder;
	}
}

static void	init_dongle(t_coder *coder)
{
	t_dongle	*dongle;

	coder->l_dongle = coder->prv_coder->r_dongle;
	dongle = coder->r_dongle;
	dongle->l_coder = coder;
	dongle->r_coder = coder->nxt_coder;
	dongle->l_request = FALSE;
	dongle->r_request = FALSE;
	dongle->last_use_time = 0;
	init_queue(dongle);
	if (pthread_cond_init(&dongle->dongle_cond, NULL) != 0)
	{
		return ;
	}
	if (pthread_mutex_init(&dongle->dongle_mutex, NULL) != 0)
	{
		return ;
	}
}

static void	init_dongles(t_coder *coder)
{
	int		i;

	i = coder->table->number_of_coders;
	while (i--)
	{
		init_dongle(coder);
		coder = coder->nxt_coder;
	}
}

static t_coder	*coder_init(int id_coder, t_table *table)
{
	t_coder		*coder;
	t_dongle	*dongle;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (0);
	dongle = malloc(sizeof(t_dongle));
	if (!dongle)
		return (0);
	coder->id_coder = id_coder;
	coder->cmp_count = 0;
	coder->table = table;
	coder->r_dongle = dongle;
	coder->r_dongle->table = table;
	coder->last_compile_start = get_current_time_ms();
	return (coder);
}

t_coder	*create_coders(t_table *table)
{
	t_coder	*first_coder;
	t_coder	*last_coder;
	t_coder	*new_coder;
	int		i;
	int		id_coder;

	id_coder = 1;
	first_coder = coder_init(id_coder, table);
	first_coder->id_coder = id_coder;
	first_coder->nxt_coder = first_coder;
	first_coder->prv_coder = first_coder;
	i = table->number_of_coders;
	while (--i)
	{
		last_coder = first_coder->prv_coder;
		id_coder++;
		new_coder = coder_init(id_coder, table);
		new_coder->nxt_coder = first_coder;
		first_coder->prv_coder = new_coder;
		last_coder->nxt_coder = new_coder;
		new_coder->prv_coder = last_coder;
	}
	init_dongles(first_coder);
	return (first_coder);
}
