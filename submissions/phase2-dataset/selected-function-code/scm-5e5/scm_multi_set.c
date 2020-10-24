     SCM syms, vals;
{
  SCM res = EOL, *pres = &res;
  SCM *loc;
  do {
    ASRTER(NIMP(vals) && CONSP(vals), vals, WNA, s_set);
    switch (7 & (int)(CAR(syms))) {
    case 0:
      loc = lookupcar(syms);
      break;
    case 1:
      loc = &(I_VAL(CAR(syms)));
      break;
    case 4:
      loc = ilookup(CAR(syms));
      break;
    }
    *pres = cons(*loc, EOL);
    pres = &CDR(*pres);
    *loc = CAR(vals);
    syms = CDR(syms);
    vals = CDR(vals);
  } while (NIMP(syms));
  ASRTER(NULLP(vals) && NULLP(syms), vals, WNA, s_set);
  return res;
}
