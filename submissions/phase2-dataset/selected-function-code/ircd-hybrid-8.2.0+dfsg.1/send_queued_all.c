send_queued_all(void)
{
  dlink_node *ptr;

  /* Servers are processed first, mainly because this can generate
   * a notice to opers, which is to be delivered by this function.
   */
  DLINK_FOREACH(ptr, local_server_list.head)
    send_queued_write(ptr->data);

  DLINK_FOREACH(ptr, unknown_list.head)
    send_queued_write(ptr->data);

  DLINK_FOREACH(ptr, local_client_list.head)
    send_queued_write(ptr->data);

  /* NOTE: This can still put clients on aborted_list; unfortunately,
   * exit_aborted_clients takes precedence over send_queued_all,
   * because exiting clients can generate server/oper traffic.
   * The easiest approach is dealing with aborted clients in the next I/O lap.
   * -adx
   */
}
