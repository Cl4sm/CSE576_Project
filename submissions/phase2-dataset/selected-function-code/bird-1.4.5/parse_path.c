static int
parse_path(struct adata *path, struct pm_pos *pos)
{
  u8 *p = path->data;
  u8 *q = p + path->length;
  struct pm_pos *opos = pos;
  int i, len;


  while (p < q)
    switch (*p++)
      {
      case AS_PATH_SET:
	pos->set = 1;
	pos->mark = 0;
	pos->val.sp = p;
	len = *p;
	p += 1 + BS * len;
	pos++;
	break;
      
      case AS_PATH_SEQUENCE:
	len = *p++;
	for (i = 0; i < len; i++)
	  {
	    pos->set = 0;
	    pos->mark = 0;
	    pos->val.asn = get_as(p);
	    p += BS;
	    pos++;
	  }
	break;

      default:
	bug("as_path_match: Invalid path component");
      }
  
  return pos - opos;
}
