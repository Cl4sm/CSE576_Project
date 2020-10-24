as_path_convert_to_new(struct adata *path, byte *dst, int req_as)
{
  byte *src = path->data;
  byte *src_end = src + path->length;
  byte *dst_start = dst;
  u32 as;
  int i, t, n;


  while ((src < src_end) && (req_as > 0))
    {
      t = *src++;
      n = *src++;

      if (t == AS_PATH_SEQUENCE)
	{
	  if (n > req_as)
	    n = req_as;

	  req_as -= n;
	}
      else // t == AS_PATH_SET
	req_as--;

      *dst++ = t;
      *dst++ = n;

      for(i=0; i<n; i++)
	{
	  as = get_u16(src);
	  put_u32(dst, as);
	  src += 2;
	  dst += 4;
	}
    }

  return dst - dst_start;
}
