slab *
sl_new(pool *p, unsigned size)
{
  slab *s = ralloc(p, &sl_class);
  unsigned int align = sizeof(struct sl_alignment);
  if (align < sizeof(int))
    align = sizeof(int);
  s->data_size = size;
  size += OFFSETOF(struct sl_obj, u.data);
  if (size < sizeof(struct sl_obj))
    size = sizeof(struct sl_obj);
  size = (size + align - 1) / align * align;
  s->obj_size = size;
  s->head_size = (sizeof(struct sl_head) + align - 1) / align * align;
  s->objs_per_slab = (SLAB_SIZE - s->head_size) / size;
  if (!s->objs_per_slab)
    bug("Slab: object too large");
  s->num_empty_heads = 0;
  init_list(&s->empty_heads);
  init_list(&s->partial_heads);
  init_list(&s->full_heads);
  return s;
}
