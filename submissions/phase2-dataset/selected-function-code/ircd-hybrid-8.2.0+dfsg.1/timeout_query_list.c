static time_t
timeout_query_list(void)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;
  struct reslist *request = NULL;
  time_t next_time = 0;
  time_t timeout   = 0;

  DLINK_FOREACH_SAFE(ptr, ptr_next, request_list.head)
  {
    request = ptr->data;
    timeout = request->sentat + request->timeout;

    if (CurrentTime >= timeout)
    {
      if (--request->retries <= 0)
      {
        (*request->callback)(request->callback_ctx, NULL, NULL, 0);
        rem_request(request);
        continue;
      }
      else
      {
        request->sentat = CurrentTime;
        request->timeout += request->timeout;
        resend_query(request);
      }
    }

    if (next_time == 0 || timeout < next_time)
      next_time = timeout;
  }

  return (next_time > CurrentTime) ? next_time : (CurrentTime + AR_TTL);
}
