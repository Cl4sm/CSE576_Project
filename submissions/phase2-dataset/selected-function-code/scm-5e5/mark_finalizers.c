     SCM *finalizers, *pending;
{
  SCM lst, elt, v;
  SCM live = EOL, undead = *finalizers;
  int more_to_do = !0;
  gc_mark(*pending);
  while NIMP(*pending) pending = &CDR(*pending);
  while (more_to_do) {
    more_to_do = 0;
    lst = undead;
    undead = EOL;
    while (NIMP(lst)) {
      elt = CAR(lst);
      v = CAR(elt);
      switch (TYP3(v)) {
      default:
	if (GCMARKP(v)) goto marked;
	goto unmarked;
      case tc3_tc7_types:
	if (GC8MARKP(v)) {
	marked:
	  gc_mark(CDR(elt));
	  more_to_do = !0;
	  v = lst;
	  lst = CDR(lst);
	  CDR(v) = live;
	  live = v;
	}
	else {
	unmarked:
	  v = lst;
	  lst = CDR(lst);
	  CDR(v) = undead;
	  undead = v;
	}
	break;
      }
    }
  }
  gc_mark(live);
  for (lst = undead; NIMP(lst); lst = CDR(lst))
    CAR(lst) = CDR(CAR(lst));
  gc_mark(undead);
  *finalizers = live;
  *pending = undead;
}
