dlink_node *
dlinkFindDelete(dlink_list *list, void *data)
{
  dlink_node *m = NULL;

  DLINK_FOREACH(m, list->head)
  {
    if (m->data != data)
      continue;

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

    return m;
  }

  return NULL;
}
