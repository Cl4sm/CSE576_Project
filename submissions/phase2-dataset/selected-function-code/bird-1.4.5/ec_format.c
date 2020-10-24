int
ec_format(byte *buf, u64 ec)
{
  u32 type, key, val;
  char tbuf[16], *kind;

  type = ec >> 48;
  switch (type & 0xf0ff)
    {
    case EC_RT: kind = "rt"; break;
    case EC_RO: kind = "ro"; break;

    default:
      kind = tbuf;
      bsprintf(kind, "unknown 0x%x", type);
    }

  switch (ec >> 56)
    {
      /* RFC 4360 3.1.  Two-Octet AS Specific Extended Community */
    case 0x00:
    case 0x40:
      key = (ec >> 32) & 0xFFFF;
      val = ec;
      return bsprintf(buf, "(%s, %u, %u)", kind, key, val);

      /* RFC 4360 3.2.  IPv4 Address Specific Extended Community */
    case 0x01:
    case 0x41:
      key = ec >> 16;
      val = ec & 0xFFFF;
      return bsprintf(buf, "(%s, %R, %u)", kind, key, val);

      /* RFC 5668  4-Octet AS Specific BGP Extended Community */
    case 0x02:
    case 0x42:
      key = ec >> 16;
      val = ec & 0xFFFF;
      return bsprintf(buf, "(%s, %u, %u)", kind, key, val);

      /* Generic format for unknown kinds of extended communities */
    default:
      key = ec >> 32;
      val = ec;
      return bsprintf(buf, "(generic, 0x%x, 0x%x)", key, val);
    }

}
