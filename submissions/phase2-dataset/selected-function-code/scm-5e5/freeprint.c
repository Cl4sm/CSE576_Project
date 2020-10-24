     SCM exp; SCM port; int writing;
{
  if (tc_broken_heart==CAR(exp)) {
    lputs("#<GC-FORWARD->", port);
    scm_iprin1(CDR(exp), port, writing);
  }
  else {
    if (NIMP(CDR(exp)) && tc7_smob==CAR(CDR(exp))) {
      lputs("#<FREE-CELL ", port);
    }
    else {
      lputs("#<NEW-CELL . ", port);
      scm_iprin1(CDR(exp), port, writing);
    }
    lputs(" @0x", port);
    scm_intprint((long)exp, -16, port);
  }
  lputc('>', port);
  return !0;
}
