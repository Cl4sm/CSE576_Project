#if defined(THR)
  rt_mutex_lock(&s->mtx);
#endif
  if (s->size > (s->top + 1)) {
    int newsize = s->top + 1;
    rt_tasktile_t *tmp = (rt_tasktile_t *) realloc(s->s, newsize * sizeof(rt_tasktile_t));
    if (tmp == NULL) {
#if defined(THR)
      rt_mutex_unlock(&s->mtx);
#endif
      return -1; /* out of space! */
    }
    s->s = tmp;
    s->size = newsize;
  }
#if defined(THR)
  rt_mutex_unlock(&s->mtx);
#endif

  return 0;
}
