SCM stringsplitutil(prog, str, vector)
     SCM prog, str;
     int vector;
{
  int anchor, match_start, match_end, num_substrings, num_elements;
  int search_base;
  SCM next_break, substrings, ret;
  SCM st_start, st_end;

  FIXUP_REGEXP(prog);
  ASRTER(NIMP(prog) && tc16_rgx==CAR(prog), prog, ARG1, s_stringsplit);
  ASRTER(NIMP(str) && STRINGP(str), str, ARG2, s_stringsplit);

  substrings = EOL;
  anchor = 0;
  search_base = 0;
  num_substrings = 0;
  next_break = lregsearchv(prog, str, cons(MAKINUM(search_base), EOL));

  while (next_break != BOOL_F) {
    match_start = INUM(VELTS(next_break)[0]);
    match_end   = INUM(VELTS(next_break)[1]);

    if (match_start < match_end) {
      substrings=cons2(MAKINUM(anchor), MAKINUM(match_start), substrings);
      anchor = match_end;
      num_substrings++;
    }

    search_base = ((match_end>search_base)?match_end:(search_base+1));
    next_break  = lregsearchv(prog, str, cons(MAKINUM(search_base), EOL));
  }

  /* get that tail bit */
  if (anchor < LENGTH(str)) {
    substrings = cons2(MAKINUM(anchor), MAKINUM(LENGTH(str)), substrings);
    num_substrings++;
  }

  num_elements = vector?(2*num_substrings):num_substrings;
  ret = make_vector(MAKINUM(num_elements), EOL);

  while (num_substrings--) {
    st_start = CAR(substrings);
    st_end	 = CAR(CDR(substrings));

    if (vector) {
      VELTS(ret)[num_substrings*2+0] = st_start;
      VELTS(ret)[num_substrings*2+1] = st_end;
    } else
      VELTS(ret)[num_substrings] = substring(str, st_start, st_end);

    substrings = CDR(CDR(substrings));
  }

  return ret;
}
