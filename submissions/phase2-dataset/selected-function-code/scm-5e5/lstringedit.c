     SCM prog, editspec, args;
{
  int match_start, match_end, search_base, editcount;
  int total_len;
  int i, args_len, anchor, maxsubnum;
  int backslash;
  char *ptr;
  editItem editlist, substrings, edit;
  SCM str, count, next_edit;
  SCM result;
  ALLOCA_PROTECT;

  args_len = ilength(args);

  FIXUP_REGEXP(prog);
  ASRTER(NIMP(prog) && tc16_rgx==CAR(prog), prog, ARG1, s_stringedit);
  ASRTER(NIMP(editspec) && STRINGP(editspec), editspec, ARG2, s_stringedit);
  ASRTER((args_len==1)||(args_len==2), args, WNA, s_stringedit);

  str = CAR(args);
  ASRTER(NIMP(str)&&STRINGP(str), str, ARG3, s_stringedit);

  if (args_len==2) {
    count = CAR(CDR(args));
    ASRTER(INUMP(count)||(count==BOOL_T), count, ARG4, s_stringedit);
  } else
    count = MAKINUM(1);

  /* process the editspec - break it into a list of dotted pairs
   * of integers for substrings to be inserted and
   * integers representing matched subexpressions that
   * should be inserted.
   */

  maxsubnum = RGX(prog)->re_nsub;
  anchor = 0;
  backslash = 0;
  editlist = 0;
  ptr = CHARS(editspec);

  for (i=0; i<LENGTH(editspec); i++) {
    if (backslash && (ptr[i]>='0') && (ptr[i] <='9') &&
	((ptr[i]-'0')<=maxsubnum))
      {
	if ((i-1)>anchor)
	  PUSH(editlist, CHARS(editspec), anchor, i-1);

	PUSH(editlist, CHARS(editspec), ptr[i]-'0', -1);
	anchor = i+1;
      }
    backslash = (ptr[i] == '\\')?1:0;
  }

  if (anchor < LENGTH(editspec))
    PUSH(editlist, CHARS(editspec), anchor, LENGTH(editspec));

  /* now, reverse the list of edit items */
  {
    editItem prev, cur, next;

    for (prev=0, cur=editlist; cur; prev=cur, cur=next) {
      next = cur->next;
      cur->next = prev;
    }
    editlist = prev;
  }

  anchor = 0;
  search_base = 0;
  editcount = 0;
  substrings = 0;

  next_edit = lregsearchv(prog, str, cons(MAKINUM(search_base), EOL));

  while (next_edit != BOOL_F) {
    if (INUMP(count) && (editcount==INUM(count)))
      break;

    match_start = INUM(VELTS(next_edit)[0]);
    match_end   = INUM(VELTS(next_edit)[1]);

    if (match_start < match_end) {
      PUSH(substrings, CHARS(str), anchor, match_start);
      anchor = match_end;
    }

    for (edit=editlist; edit; edit=edit->next) {
      if (edit->end == -1) {
	/* A backslash number in the original editspec */
	PUSH(substrings, CHARS(str),
	     INUM(VELTS(next_edit)[edit->start*2+0]),
	     INUM(VELTS(next_edit)[edit->start*2+1]));
      } else
	/* normal string in the editspec */
	PUSH(substrings, edit->string, edit->start, edit->end);
    }

    editcount++;
    search_base = ((match_end>search_base)?match_end:(search_base+1));
    next_edit  = lregsearchv(prog, str, cons(MAKINUM(search_base), EOL));
  }

  /* get that tail bit */
  if (anchor < LENGTH(str))
    PUSH(substrings, CHARS(str), anchor, LENGTH(str));

  /* assemble the result string */
  for (edit=substrings, total_len=0; edit; edit=edit->next)
    total_len += (edit->end - edit->start);

  result = makstr(total_len);
  ptr = CHARS(result) + total_len; /* point at the null at the end */

  for (edit=substrings; edit; edit=edit->next) {
    ptr -= (edit->end - edit->start);
    bcopy(edit->string + edit->start, ptr, edit->end - edit->start);
  }
  return result;
}
