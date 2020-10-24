SCM ren_fil(oldname, newname)
     SCM oldname, newname;
{
  SCM ans;
  ASRTER(NIMP(oldname) && STRINGP(oldname), oldname, ARG1, s_ren_fil);
  ASRTER(NIMP(newname) && STRINGP(newname), newname, ARG2, s_ren_fil);
#if 1 /* def STDC_HEADERS */
  SYSCALL(ans = (rename(CHARS(oldname), CHARS(newname))) ? BOOL_F: BOOL_T;);
  return ans;
#else
  DEFER_INTS;
  SYSCALL(ans = link(CHARS(oldname), CHARS(newname)) ? BOOL_F : BOOL_T;);
  if (!FALSEP(ans)) {
    SYSCALL(ans = unlink(CHARS(oldname)) ? BOOL_F : BOOL_T;);
    if (FALSEP(ans))
      SYSCALL(unlink(CHARS(newname));); /* unlink failed.  remove new name */
  }
  ALLOW_INTS;
  return ans;
#endif
}
