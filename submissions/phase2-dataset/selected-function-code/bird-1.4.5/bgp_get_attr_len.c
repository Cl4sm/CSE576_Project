static int
bgp_get_attr_len(eattr *a)
{
  int len;
  if (ATTR_KNOWN(EA_ID(a->id)))
    {
      int code = EA_ID(a->id);
      struct attr_desc *desc = &bgp_attr_table[code];
      len = desc->expected_length;
      if (len < 0)
	{
	  ASSERT(!(a->type & EAF_EMBEDDED));
	  len = a->u.ptr->length;
	}
    }
  else
    {
      ASSERT((a->type & EAF_TYPE_MASK) == EAF_TYPE_OPAQUE);
      len = a->u.ptr->length;
    }
  
  return len;
}
