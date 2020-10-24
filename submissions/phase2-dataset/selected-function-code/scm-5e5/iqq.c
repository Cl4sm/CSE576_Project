static SCM iqq(form)
     SCM form;
{
  SCM tmp;
  if (IMP(form)) return form;
  if (VECTORP(form)) {
    long i = LENGTH(form);
    SCM *data = VELTS(form);
    tmp = EOL;
    for (;--i >= 0;) tmp = cons(data[i], tmp);
    return vector(iqq(tmp));
  }
  if (NCONSP(form)) return form;
  tmp = CAR(form);
  if (IM_UNQUOTE==tmp)
    return evalcar(CDR(form));
  if (NIMP(tmp) && IM_UQ_SPLICING==CAR(tmp))
    return append(cons2(evalcar(CDR(tmp)), iqq(CDR(form)), EOL));
  return cons(iqq(CAR(form)), iqq(CDR(form)));
}
