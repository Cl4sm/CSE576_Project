struct adata *
as_path_prepend(struct linpool *pool, struct adata *olda, u32 as)
{
  struct adata *newa;

  if (olda->length && olda->data[0] == AS_PATH_SEQUENCE && olda->data[1] < 255)
    /* Starting with sequence => just prepend the AS number */
    {
      int nl = olda->length + BS;
      newa = lp_alloc(pool, sizeof(struct adata) + nl);
      newa->length = nl;
      newa->data[0] = AS_PATH_SEQUENCE;
      newa->data[1] = olda->data[1] + 1;
      memcpy(newa->data + BS + 2, olda->data + 2, olda->length - 2);
    }
  else /* Create new path segment */
    {
      int nl = olda->length + BS + 2;
      newa = lp_alloc(pool, sizeof(struct adata) + nl);
      newa->length = nl;
      newa->data[0] = AS_PATH_SEQUENCE;
      newa->data[1] = 1;
      memcpy(newa->data + BS + 2, olda->data, olda->length);
    }
  put_as(newa->data + 2, as);
  return newa;
}
