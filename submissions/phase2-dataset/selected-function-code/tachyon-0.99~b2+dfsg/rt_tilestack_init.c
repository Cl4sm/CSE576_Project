  if (s == NULL)
    return -1;

#if defined(THR)
  rt_mutex_init(&s->mtx);
#endif

  s->growthrate = 512;
  s->top = -1;

  if (size > 0) {
    s->size = size;
    s->s = (rt_tasktile_t *) malloc(s->size * sizeof(rt_tasktile_t));
  } else {
    s->size = 0;
    s->s = NULL;
  }

  return 0;
}
