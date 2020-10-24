cli_set_log_echo(cli *c, unsigned int mask, unsigned int size)
{
  if (c->ring_buf)
    {
      mb_free(c->ring_buf);
      c->ring_buf = c->ring_end = c->ring_read = c->ring_write = NULL;
      rem_node(&c->n);
    }
  c->log_mask = mask;
  if (mask && size)
    {
      c->ring_buf = mb_alloc(c->pool, size);
      c->ring_end = c->ring_buf + size;
      c->ring_read = c->ring_write = c->ring_buf;
      add_tail(&cli_log_hooks, &c->n);
      c->log_threshold = size / 8;
    }
  c->ring_overflow = 0;
}
