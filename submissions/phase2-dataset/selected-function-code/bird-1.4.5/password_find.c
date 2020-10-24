password_find(list *l, int first_fit)
{
  struct password_item *pi;
  struct password_item *pf = NULL;

  if (l)
  {
    WALK_LIST(pi, *l)
    {
      if ((pi->genfrom < now_real) && (pi->gento > now_real))
      {
	if (first_fit)
	  return pi;

	if (!pf || pf->genfrom < pi->genfrom)
	  pf = pi;
      }
    }
  }
  return pf;
}
