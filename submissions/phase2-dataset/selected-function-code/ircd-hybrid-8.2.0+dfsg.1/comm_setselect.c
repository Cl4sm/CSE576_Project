void
comm_setselect(fde_t *F, unsigned int type, void (*handler)(fde_t *, void *),
               void *client_data, time_t timeout)
{
  int new_events;

  if ((type & COMM_SELECT_READ))
  {
    F->read_handler = handler;
    F->read_data = client_data;
  }

  if ((type & COMM_SELECT_WRITE))
  {
    F->write_handler = handler;
    F->write_data = client_data;
  }

  new_events = (F->read_handler ? POLLIN : 0) |
    (F->write_handler ? POLLOUT : 0);

  if (timeout != 0)
  {
    F->timeout = CurrentTime + (timeout / 1000);
    F->timeout_handler = handler;
    F->timeout_data = client_data;
  }

  if (new_events != F->evcache)
  {
    devpoll_write_update(F->fd, POLLREMOVE);
    if ((F->evcache = new_events))
      devpoll_write_update(F->fd, new_events);
  }
}
