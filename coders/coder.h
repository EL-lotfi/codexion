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
  GETTING_DONGLES,
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
  pthread_mutex_t mutex;
} t_dongle;

typedef struct s_coder {
  int             id_coder;
  pthread_t       coder_thread;
  int             time_to_burnout;
  int             cmp_count;
  t_dongle        *l_dongle;
  t_dongle        *r_dongle;
  t_dongle        *f_dongle;
  struct s_coder  *nxt_coder;
  struct s_coder  *prv_coder;
} t_coder;

typedef struct table {
  pthread_mutex_t print_mutex;
  long long start_time;
  int time_to_compile;
  int time_to_burnout;
  int time_to_debug;
  int time_to_refactor;
  int nbr_compiles_required;
}

void coder_routine(t_coder *coder);
long long count_elapsed_time();
void compile_process(t_coder *coder, t_table *table, long long time);
void debug_process(t_coder *coder, t_table *table, long long time);
void refactor_process(t_coder *coder, t_table *table, long long time);
long long get_current_time_ms();
void print_process(t_process process);
t_coder *create_coders(int nbr_coders);

#endif // !CODER_H
