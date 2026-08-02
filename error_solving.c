
#include "coders/coder.h"

void *coder_routine(void *param)
{
  t_coder *coder = (t_coder *)param;
  affect_dongle(coder, RIGHT);
  affect_dongle(coder, LEFT);
  compile_process(coder);
  debug_process(coder);
  refactor_process(coder);
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
  printf("\n");
  printf("hii %d",coder->table->time_to_compile);
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

int main()
{
  t_table *table;
  t_coder *first_coder;
  t_coder *current_coder;
  t_bool  first;

  table = malloc(sizeof(t_table));
  table->time_to_compile = 300;
  table->time_to_debug = 200;
  table->time_to_refactor = 100;
  table->start_time = get_current_time_ms();
  if(pthread_mutex_init(&table->print_mutex, NULL) != 0)
  {
    // clean_up();
    return 0;
  }
  first_coder = create_coders(3, table);
  current_coder = first_coder;
  first = TRUE;
  while (current_coder != first_coder || first)
  { 
    pthread_create(&current_coder->coder_thread, NULL, coder_routine, &current_coder);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
  first = TRUE;
  while (current_coder != first_coder || first)
  { 
    pthread_join(current_coder->coder_thread, NULL);
    current_coder = current_coder->nxt_coder;
    first = FALSE;
  }
}
