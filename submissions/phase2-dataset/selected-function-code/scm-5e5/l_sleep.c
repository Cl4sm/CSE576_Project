SCM l_sleep(i)
     SCM i;
{
  unsigned int j = 0;
  ASRTER(INUMP(i) && (INUM(i) >= 0), i, ARG1, s_sleep);
#  ifdef __HIGHC__
  SYSCALL(sleep(INUM(i)););
#  else
#   ifdef _WIN32
  Sleep(INUM(i) * 1000);
#   else
#    ifdef __IBMC__
  DosSleep(INUM(i) * 1000);
#    else
  SYSCALL(j = sleep(INUM(i)););
#    endif
#   endif
#  endif
  return MAKINUM(j);
}
