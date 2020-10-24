ipsum_calc_block(u32 *buf, unsigned len, u16 isum)
{
  /*
   *  A few simple facts about the IP checksum (see RFC 1071 for detailed
   *  discussion):
   *
   *	o  It's associative and commutative.
   *	o  It's byte order independent.
   *	o  It's word size independent.
   *
   *  This gives us a neat 32-bits-at-a-time algorithm which respects
   *  usual alignment requirements and is reasonably fast.
   */

  ASSERT(!(len % 4));
  if (!len)
    return isum;

  u32 *end = buf + (len >> 2);
  u32 sum = isum;
  while (buf < end)
    sum = add32(sum, *buf++);

  sum = (sum >> 16) + (sum & 0xffff);    /* add high-16 to low-16 */
  sum += (sum >> 16); /* add carry */
  return sum;
}
