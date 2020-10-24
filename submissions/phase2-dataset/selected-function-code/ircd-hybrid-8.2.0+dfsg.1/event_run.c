void
event_run(void)
{
  static time_t last = 0;
  unsigned int len = 0;

  if (last == CurrentTime)
    return;
  last = CurrentTime;

  len = dlink_list_length(&events);
  while (len-- && dlink_list_length(&events))
  {
    struct event *e = events.head->data;

    if (e->next > CurrentTime)
      break;

    event_delete(e);

    e->handler(e->data);

    if (!e->oneshot)
      event_add(e, e->data);
  }
}
