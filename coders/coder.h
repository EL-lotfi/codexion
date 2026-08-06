/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibel-lot <ibel-lot@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 16:49:43 by ibel-lot          #+#    #+#             */
/*   Updated: 2026/07/09 18:45:18 by ibel-lot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODER_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

typedef enum s_process{
  TAKING_DONGLES,
  COMPILING,
  DEBUGGING,
  REFACTORING
} t_process;

typedef enum e_side{
  LEFT,
  RIGHT
} t_side;

typedef enum e_bool{
  FALSE,
  TRUE
} t_bool;

typedef struct s_dongle{
  t_bool          in_use; 
  pthread_cond_t  dongle_cond;
  pthread_mutex_t mutex;
  int             *queue;
} t_dongle;


typedef struct s_table {
  pthread_mutex_t print_mutex;
  long long       start_time;
  int             time_to_compile;
  int             time_to_burnout;
  int             time_to_debug;
  int             time_to_refactor;
  int             nbr_compiles_required;
} t_table;

typedef struct s_coder {
  int             id_coder;
  pthread_t       coder_thread;
  int             time_to_burnout;
  int             cmp_count;
  long long       last_readiness_time;
  t_dongle        *l_dongle;
  t_dongle        *r_dongle;
  t_dongle        *f_dongle;
  struct s_coder  *nxt_coder;
  struct s_coder  *prv_coder;
  t_table         *table;
} t_coder;

void      *coder_routine(void *param);
long long count_elapsed_time();
long long get_current_time_ms();
void      print_process(t_coder *coder, t_process process);
t_coder   *create_coders(int nbr_coders, t_table *table);
void      affect_dongle(t_coder *coder, char side);
void      detach_dongle(t_coder *coder, char side);
void      swap_queue(int *queue);
void      redefine_priority(t_coder *coder);

#endif // !CODER_H
