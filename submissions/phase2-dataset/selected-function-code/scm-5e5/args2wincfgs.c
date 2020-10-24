static int args2wincfgs(vlu, oargs)
     XWindowChanges *vlu;
     SCM oargs;
{
  SCM sval, args = oargs;
  int cfgs, len, cfgs_mask = 0;
  /* (void)memset((char *)vlu, 0, sizeof(XWindowChanges)); */
  if (!(len = ilength(args))) return 0;
  ASRTER(len > 0 && (! (len & 1)), oargs, WNA, s_window);
  while (len) {
    ASRTER(NIMP(args), oargs, WNA, s_window);
    cfgs = theint(CAR(args), s_window); args = CDR(args);
    ASRTER(NIMP(args), oargs, WNA, s_window);
    sval = CAR(args); args = CDR(args);
    cfgs_mask |= cfgs;
    switch (cfgs) {

    case CWX:		vlu->x		      = theuint(sval, s_window); break;
    case CWY:		vlu->y		      = theuint(sval, s_window); break;
    case CWWidth:	vlu->width	      = theuint(sval, s_window); break;
    case CWHeight:	vlu->height	      = theuint(sval, s_window); break;
    case CWBorderWidth:	vlu->border_width     = theuint(sval, s_window); break;
    case CWSibling:	vlu->sibling          =thepxmap(sval, s_window); break;
    case CWStackMode:	vlu->stack_mode       =  theint(sval, s_window); break;
    default: ASRTER(0, MAKINUM(cfgs), ARGn, s_window);
    }
    len -= 2;
  }
  return cfgs_mask;
}
