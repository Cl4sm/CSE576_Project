     long *a;
{
  register CONTINUATION *cont = (CONTINUATION *)(a[0]);
  long val = a[1];
# ifndef CHEAP_CONTINUATIONS
  register long j;
  register STACKITEM *src, *dst = cont->stkbse;
#  ifdef STACK_GROWS_UP
#   ifndef hpux
  if (a[2] && (a - ((long *)a[3]) < SCM_GROWTH))
    puts("grow_throw: check if long growth[]; being optimized out");
#   endif
  /* if (a[2]) fprintf(stderr, " ct = %ld, dist = %ld\n", a[2], (((long *)a[3]) - a)); */
  if (PTR_GE(dst + (cont->length), (STACKITEM *)&a)) grow_throw(a);
#  else
#   ifndef hpux
  if (a[2] && (((long *)a[3]) - a < SCM_GROWTH))
    puts("grow_throw: check if long growth[]; being optimized out");
#   endif
  /* if (a[2]) fprintf(stderr, " ct = %ld, dist = %ld\n", a[2], (((long *)a[3]) - a)); */
  dst -= cont->length;
  if (PTR_LE(dst, (STACKITEM *)&a)) grow_throw(a);
#  endif/* def STACK_GROWS_UP */
  FLUSH_REGISTER_WINDOWS;
  src = (STACKITEM *)(cont + 1);
  for (j = cont->length;0 <= --j;) *dst++ = *src++;
# endif /* ndef CHEAP_CONTINUATIONS */
# ifdef SHORT_INT
  thrown_value = val;
  longjump(cont->jmpbuf, 1);
# else
  longjump(cont->jmpbuf, val);
# endif
}
