void
dlink_move_node(dlink_node *m, dlink_list *list_del, dlink_list *list_add)
{
  /* Assumption: If m->next == NULL, then list_del->tail == m
   *      and:   If m->prev == NULL, then list_del->head == m
   */
  if (m->next)
    m->next->prev = m->prev;
  else
  {
    assert(list_del->tail == m);
    list_del->tail = m->prev;
  }

  if (m->prev)
    m->prev->next = m->next;
  else
  {
    assert(list_del->head == m);
    list_del->head = m->next;
  }

  /* Set this to NULL does matter */
  m->prev = NULL;
  m->next = list_add->head;

  /* Assumption: If list_add->tail != NULL, list_add->head != NULL */
  if (list_add->head != NULL)
    list_add->head->prev = m;
  else if (list_add->tail == NULL)
    list_add->tail = m;

  list_add->head = m;
  list_add->length++;
  list_del->length--;
}
