dbuf_delete(struct dbuf_queue *queue, size_t count)
{
  while (count > 0 && dbuf_length(queue) > 0)
  {
    dlink_node *node = queue->blocks.head;
    struct dbuf_block *block = node->data;
    size_t avail = block->size - queue->pos;

    if (count >= avail)
    {
      count -= avail;
      queue->total_size -= avail;

      dbuf_ref_free(block);

      dlinkDelete(node, &queue->blocks);
      free_dlink_node(node);

      queue->pos = 0;
    }
    else
    {
      queue->pos += count;

      queue->total_size -= count;
      count -= count;
    }
  }
}
