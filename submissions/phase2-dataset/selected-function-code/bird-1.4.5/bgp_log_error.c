void
bgp_log_error(struct bgp_proto *p, u8 class, char *msg, unsigned code, unsigned subcode, byte *data, unsigned len)
{
  const byte *name;
  byte *t, argbuf[36];
  unsigned i;

  /* Don't report Cease messages generated by myself */
  if (code == 6 && class == BE_BGP_TX)
    return;

  name = bgp_error_dsc(code, subcode);
  t = argbuf;
  if (len)
    {
      *t++ = ':';
      *t++ = ' ';

      if ((code == 2) && (subcode == 2) && ((len == 2) || (len == 4)))
	{
	  /* Bad peer AS - we would like to print the AS */
	  t += bsprintf(t, "%d", (len == 2) ? get_u16(data) : get_u32(data));
	  goto done;
	}
      if (len > 16)
	len = 16;
      for (i=0; i<len; i++)
	t += bsprintf(t, "%02x", data[i]);
    }
 done:
  *t = 0;
  log(L_REMOTE "%s: %s: %s%s", p->p.name, msg, name, argbuf);
}
