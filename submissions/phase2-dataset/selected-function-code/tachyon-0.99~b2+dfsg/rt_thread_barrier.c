#ifdef THR
  int my_phase;
  int my_result;

  rt_mutex_lock(&barrier->lock);
  my_phase = barrier->phase;
  barrier->sum += increment;
  barrier->n_waiting++;

  if (barrier->n_waiting == barrier->n_clients) {
    barrier->result = barrier->sum;
    barrier->sum = 0;
    barrier->n_waiting = 0;
    barrier->phase = 1 - my_phase;
    rt_cond_broadcast(&barrier->wait_cv);
  }

  while (barrier->phase == my_phase) {
    rt_cond_wait(&barrier->wait_cv, &barrier->lock);
  }

  my_result = barrier->result;

  rt_mutex_unlock(&barrier->lock);

  return my_result; 
#else 
  return 0;
#endif
}
