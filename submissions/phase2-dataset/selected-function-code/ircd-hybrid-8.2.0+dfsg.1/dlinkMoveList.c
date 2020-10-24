dlinkMoveList(dlink_list *from, dlink_list *to)
{
  /* There are three cases */
  /* case one, nothing in from list */
  if (from->head == NULL)
    return;

  /* case two, nothing in to list */
  /* actually if to->head is NULL and to->tail isn't, thats a bug */
  if (to->head == NULL)
  {
    to->head = from->head;
    to->tail = from->tail;
    from->head = from->tail = NULL;
    to->length = from->length;
    from->length = 0;
    return;
  }

  /* third case play with the links */
  from->tail->next = to->head;
  from->head->prev = to->head->prev;
  to->head->prev = from->tail;
  to->head = from->head;
  from->head = from->tail = NULL;
  to->length += from->length;
  from->length = 0;
  /* I think I got that right */
}
