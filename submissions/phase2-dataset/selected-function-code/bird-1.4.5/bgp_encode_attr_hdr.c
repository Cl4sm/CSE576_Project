bgp_encode_attr_hdr(byte *dst, unsigned int flags, unsigned code, int len)
{
  int wlen;

  DBG("\tAttribute %02x (%d bytes, flags %02x)\n", code, len, flags);

  if (len < 256)
    {
      *dst++ = flags;
      *dst++ = code;
      *dst++ = len;
      wlen = 3;
    }
  else
    {
      *dst++ = flags | BAF_EXT_LEN;
      *dst++ = code;
      put_u16(dst, len);
      wlen = 4;
    }

  return wlen;
}
