void
dlinkDelete(dlink_node *m, dlink_list *list)
{
  /* Assumption: If m->next == NULL, then list->tail == m
   *      and:   If m->prev == NULL, then list->head == m
   */
  if (m->next)
    m->next->prev = m->prev;
  else
  {
    assert(list->tail == m);
    list->tail = m->prev;
  }

  if (m->prev)
    m->prev->next = m->next;
  else
  {
    assert(list->head == m);
    list->head = m->next;
  }

  /* Set this to NULL does matter */
  m->next = m->prev = NULL;
  list->length--;
}
