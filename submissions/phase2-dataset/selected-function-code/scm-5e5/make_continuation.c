     CONTINUATION *parent_cont;
{
  CONTINUATION *cont;
# ifdef CHEAP_CONTINUATIONS
  cont = (CONTINUATION *)malloc(sizeof(CONTINUATION));
  if (!cont) return 0;
  cont->length = 0;
  cont->stkbse = parent_cont->stkbse;
# else
  long j;
  register STACKITEM *src, *dst;
  FLUSH_REGISTER_WINDOWS;
  j = stack_size(parent_cont->stkbse);
  cont = (CONTINUATION *)malloc((sizeof(CONTINUATION) + j*sizeof(STACKITEM)));
  if (!cont) return 0;
  cont->length = j;
  cont->stkbse = parent_cont->stkbse;
  src = cont->stkbse;
#  ifdef STACK_GROWS_UP
  src += parent_cont->length;
#  else
  src -= parent_cont->length + cont->length;
#  endif/* ndef STACK_GROWS_UP */
  dst = (STACKITEM *)(cont + 1);
  for (j = cont->length; 0 <= --j; ) *dst++ = *src++;
# endif /* ndef CHEAP_CONTINUATIONS */
  cont->parent = parent_cont;
  return cont;
}
