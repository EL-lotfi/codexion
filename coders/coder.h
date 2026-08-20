/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:49:43 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/08/19 18:51:19 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H
# define CODER_H
# include <unistd.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_dongle	t_dongle;
typedef struct s_table	t_table;

typedef enum e_attribute
{
	time_to_compile,
	time_to_debug,
	time_to_refactor,
	time_to_burnout,
  dongle_cooldown,
	nbr_compiles_required,
  burnout_detected,
  start_time,
  scheduler_type
}	t_attribute;

typedef enum e_process
{
	TAKING_DONGLES,
	COMPILING,
	DEBUGGING,
	REFACTORING
}	t_process;

typedef enum e_side
{
	LEFT,
	RIGHT
}	t_side;

typedef enum e_bool
{
	FALSE,
	TRUE
}	t_bool;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_coder
{
	pthread_mutex_t		coder_mutex;
	pthread_t		coder_thread;
	int				id_coder;
	int				cmp_count;
	long long		last_compile_start;
	t_dongle		*r_dongle;
	t_dongle		*l_dongle;
	struct s_coder	*nxt_coder;
	struct s_coder	*prv_coder;
	t_table			*table;
}	t_coder;

typedef struct s_dongle
{
	pthread_cond_t	dongle_cond;
	pthread_mutex_t	dongle_mutex;
	long long		last_use_time;
	t_coder			*r_coder;
	t_coder			*l_coder;
	t_bool			r_request;
	t_bool			l_request;
	t_table			*table;
	int				*queue;
}	t_dongle;

typedef struct s_table
{
  pthread_mutex_t get_mutex;
	pthread_mutex_t	print_mutex;
	int				number_of_coders;
	int				time_to_compile;
	int				time_to_burnout;
	int				time_to_debug;
	int				time_to_refactor;
	int				nbr_compiles_required;
	int				dongle_cooldown;
	long long		start_time;
	t_bool			burnout_detected;
	t_bool			simulation_over;
	t_scheduler		scheduler_type;
}	t_table;

void		*monitor_routine(void *param);
void		*coder_routine(void *param);
long long	count_elapsed_time(t_table *table);
long long	get_current_time_ms(void);
void		print_process(t_coder *coder, t_process process);
t_coder		*create_coders(t_table *table);
t_bool		is_dongle_available(t_dongle *dongle);
void		affect_dongle(t_coder *coder, t_side side);
void		detach_dongle(t_coder *coder, t_side side);
void		swap_queue(int *queue);
void		redefine_priority(t_dongle *dongle);
void  clean_up(t_coder *first_coder);
long long get_table_attribute(t_table *table, t_attribute attribute);

#endif
