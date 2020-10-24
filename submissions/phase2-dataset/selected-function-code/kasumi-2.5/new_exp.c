static struct expression *
new_exp (nargs, op, args)
     int nargs;
     enum operator op;
     struct expression * const *args;
{
  int i;
  struct expression *newp;

  /* If any of the argument could not be malloc'ed, just return NULL.  */
  for (i = nargs - 1; i >= 0; i--)
    if (args[i] == NULL)
      goto fail;

  /* Allocate a new expression.  */
  newp = (struct expression *) malloc (sizeof (*newp));
  if (newp != NULL)
    {
      newp->nargs = nargs;
      newp->operation = op;
      for (i = nargs - 1; i >= 0; i--)
	newp->val.args[i] = args[i];
      return newp;
    }

 fail:
  for (i = nargs - 1; i >= 0; i--)
    FREE_EXPRESSION (args[i]);

  return NULL;
}
