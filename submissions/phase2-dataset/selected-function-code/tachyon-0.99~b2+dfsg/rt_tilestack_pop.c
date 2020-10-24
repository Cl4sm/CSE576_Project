#if defined(THR)
  rt_mutex_lock(&s->mtx);
#endif

  if (s->top < 0) {
#if defined(THR)
    rt_mutex_unlock(&s->mtx);
#endif
    return RT_TILESTACK_EMPTY; /* empty stack */
  }

  *t = s->s[s->top];
  s->top--;

#if defined(THR)
  rt_mutex_unlock(&s->mtx);
#endif

  return 0;
}
