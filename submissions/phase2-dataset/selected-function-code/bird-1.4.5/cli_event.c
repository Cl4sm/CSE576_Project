static void
cli_event(void *data)
{
  cli *c = data;
  int err;

  while (c->ring_read != c->ring_write &&
      c->async_msg_size < CLI_MAX_ASYNC_QUEUE)
    cli_copy_message(c);

  if (c->tx_pos)
    ;
  else if (c->cont)
    c->cont(c);
  else
    {
      err = cli_get_command(c);
      if (!err)
	return;
      if (err < 0)
	cli_printf(c, 9000, "Command too long");
      else
	cli_command(c);
    }

  cli_write_trigger(c);
}
