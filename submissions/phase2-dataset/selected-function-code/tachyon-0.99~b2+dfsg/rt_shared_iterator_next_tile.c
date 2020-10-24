                                 rt_tasktile_t *tile) {
  int rc=RT_SCHED_CONTINUE;

#if defined(THR)
  rt_mutex_spin_lock(&it->mtx);
#endif
  if (!it->fatalerror) {
    tile->start=it->current; /* set start to the current work unit    */
    it->current+=reqsize;    /* increment by the requested tile size  */
    tile->end=it->current;   /* set the (exclusive) endpoint          */

    /* if start is beyond the last work unit, we're done */
    if (tile->start >= it->end) {
      tile->start=0;
      tile->end=0;
      rc = RT_SCHED_DONE;
    }

    /* if the endpoint (exclusive) for the requested tile size */
    /* is beyond the last work unit, roll it back as needed     */
    if (tile->end > it->end) {
      tile->end = it->end;
    }
  } else {
    rc = RT_SCHED_DONE;
  }
#if defined(THR)
  rt_mutex_unlock(&it->mtx);
#endif

  return rc;
}
