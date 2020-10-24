static gint
possible_handler_cmp (gconstpointer a_,
                      gconstpointer b_)
{
  const PossibleHandler *a = a_;
  const PossibleHandler *b = b_;

  if (a->bypass)
    {
      if (!b->bypass)
        {
          /* BypassApproval wins, so a is better than b */
          return 1;
        }
    }
  else if (b->bypass)
    {
      /* BypassApproval wins, so b is better than a */
      return -1;
    }

  if (a->quality < b->quality)
    {
      return -1;
    }

  if (b->quality < a->quality)
    {
      return 1;
    }

  return 0;
}