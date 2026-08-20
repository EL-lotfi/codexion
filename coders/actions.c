/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 10:09:25 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/15 10:10:08 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "coder.h"

void	affect_dongle(t_coder *coder, t_side side)
{
	t_dongle	*dongle;

	if (side == LEFT)
		dongle = coder->l_dongle;
	else
		dongle = coder->r_dongle;
	pthread_mutex_lock(&dongle->dongle_mutex);
	if (coder->l_dongle == coder->r_dongle && side == RIGHT)
		pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
	if (side == LEFT)
		dongle->r_request = TRUE;
	else
		dongle->l_request = TRUE;
	if (dongle->table->scheduler_type == FIFO)
		redefine_priority(dongle);
	while (!is_dongle_available(dongle)
		|| dongle->queue[0] != coder->id_coder)
		pthread_cond_wait(&dongle->dongle_cond, &dongle->dongle_mutex);
	if (!get_table_attribute(coder->table, burnout_detected))
		print_process(coder, TAKING_DONGLES);
	pthread_mutex_unlock(&dongle->dongle_mutex);
}

void	detach_dongle(t_coder *coder, t_side side)
{
	t_dongle	*dongle;

	if (side == LEFT)
		dongle = coder->l_dongle;
	else
		dongle = coder->r_dongle;
	pthread_mutex_lock(&dongle->dongle_mutex);
	if (side == LEFT)
		dongle->r_request = FALSE;
	else
		dongle->l_request = FALSE;
	redefine_priority(dongle);
	pthread_cond_signal(&dongle->dongle_cond);
	pthread_mutex_unlock(&dongle->dongle_mutex);
}

void	swap_queue(int *queue)
{
	int	tmp;

	tmp = queue[0];
	queue[0] = queue[1];
	queue[1] = tmp;
}
