int rt_tilestack_push(rt_tilestack_t *s, const rt_tasktile_t *t) {
#if defined(THR)
  rt_mutex_lock(&s->mtx);
#endif
  s->top++;
  if (s->top >= s->size) {
    int newsize = s->size + s->growthrate;
    rt_tasktile_t *tmp = (rt_tasktile_t *) realloc(s->s, newsize * sizeof(rt_tasktile_t));
    if (tmp == NULL) {
      s->top--;
#if defined(THR)
      rt_mutex_unlock(&s->mtx);
#endif
      return -1; /* out of space! */
    }
    s->s = tmp;
    s->size = newsize;
  }

  s->s[s->top] = *t; /* push onto the stack */

#if defined(THR)
  rt_mutex_unlock(&s->mtx);
#endif

  return 0;
}
