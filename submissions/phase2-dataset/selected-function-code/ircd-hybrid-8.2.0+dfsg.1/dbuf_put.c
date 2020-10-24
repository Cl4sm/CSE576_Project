void
dbuf_put(struct dbuf_queue *queue, const char *buf, size_t sz)
{
  while (sz > 0)
  {
    size_t avail;
    struct dbuf_block *block = dbuf_length(queue) ? queue->blocks.tail->data : NULL;

    if (block == NULL || sizeof(block->data) - block->size == 0)
    {
      block = dbuf_alloc();
      dlinkAddTail(block, make_dlink_node(), &queue->blocks);
    }

    avail = sizeof(block->data) - block->size;
    if (avail > sz)
      avail = sz;

    memcpy(&block->data[block->size], buf, avail);
    block->size += avail;

    queue->total_size += avail;

    sz -= avail;
    buf += avail;
  }
}
