                               void * fctn(void*),
                               void * parms,
                               void **rsltparms))(void *) {
#if defined(THR)
  int my_phase;
  void * (*my_result)(void*);

  rt_mutex_lock(&barrier->lock);
  my_phase = barrier->phase;
  if (fctn != NULL)
    barrier->fctn = fctn;
  if (parms != NULL)
    barrier->parms = parms;
  barrier->n_waiting++;

  if (barrier->n_waiting == barrier->n_clients) {
    barrier->rslt = barrier->fctn;
    barrier->rsltparms = barrier->parms;
    barrier->fctn = NULL;
    barrier->parms = NULL;
    barrier->n_waiting = 0;
    barrier->phase = 1 - my_phase;
    rt_cond_broadcast(&barrier->wait_cv);
  }

  while (barrier->phase == my_phase) {
    rt_cond_wait(&barrier->wait_cv, &barrier->lock);
  }

  my_result = barrier->rslt;
  if (rsltparms != NULL)
    *rsltparms = barrier->rsltparms;

  rt_mutex_unlock(&barrier->lock);
#else
  void * (*my_result)(void*) = fctn;
  if (rsltparms != NULL)
    *rsltparms = parms;
#endif

  return my_result;
}
