static inline u32
fibnode_to_lsaid(struct proto_ospf *po, struct fib_node *fn)
{
  /* We have to map IP prefixes to u32 in such manner that resulting
     u32 interpreted as IP address is a member of given
     prefix. Therefore, /32 prefix have to be mapped on itself.
     All received prefixes have to be mapped on different u32s.

     We have an assumption that if there is nontrivial (non-/32)
     network prefix, then there is not /32 prefix for the first
     and the last IP address of the network (these are usually
     reserved, therefore it is not an important restriction).
     The network prefix is mapped to the first or the last
     IP address in the manner that disallow collisions - we
     use IP address that cannot be used by parent prefix.

     For example:
     192.168.0.0/24 maps to 192.168.0.255
     192.168.1.0/24 maps to 192.168.1.0
     because 192.168.0.0 and 192.168.1.255 might be used by
     192.168.0.0/23 .

     This is not compatible with older RFC 1583, so we have an option
     to the RFC 1583 compatible assignment (that always uses the first
     address) which disallows subnetting.

     Appendig E of RFC 2328 suggests different algorithm, that tries
     to maximize both compatibility and subnetting. But as it is not
     possible to have both reliably and the suggested algorithm was
     unnecessary complicated and it does crazy things like changing
     LSA ID for a network because different network appeared, we
     choose a different way. */

  u32 id = _I(fn->prefix);

  if ((po->rfc1583) || (fn->pxlen == 0) || (fn->pxlen == 32))
    return id;

  if (id & (1 << (32 - fn->pxlen)))
    return id;
  else
    return id | ~u32_mkmask(fn->pxlen);
}
