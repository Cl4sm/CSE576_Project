list_sort (struct prelink_entry *x)
{
  int cnt, i;
  struct prelink_entry *e;
  struct prelink_entry **a;

  if (x->next == NULL)
    return;
  for (cnt = 0, e = x->next; e != NULL; e = e->next)
    ++cnt;
  a = alloca (cnt * sizeof (*a));
  for (i = 0, e = x->next; e != NULL; e = e->next)
    a[i++] = e;
  qsort (a, cnt, sizeof (*a), addr_cmp);
  x->next = NULL;
  x->prev = x;
  for (i = 0; i < cnt; ++i)
    list_append (x, a[i]);
}
