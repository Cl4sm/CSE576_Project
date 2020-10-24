internal_function
FREE_EXPRESSION (exp)
     struct expression *exp;
{
  if (exp == NULL)
    return;

  /* Handle the recursive case.  */
  switch (exp->nargs)
    {
    case 3:
      FREE_EXPRESSION (exp->val.args[2]);
      /* FALLTHROUGH */
    case 2:
      FREE_EXPRESSION (exp->val.args[1]);
      /* FALLTHROUGH */
    case 1:
      FREE_EXPRESSION (exp->val.args[0]);
      /* FALLTHROUGH */
    default:
      break;
    }

  free (exp);
}
