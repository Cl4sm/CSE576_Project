{
  long tmp;
  struct timeb time_buffer1;
  struct timeb time_buffer2;
  int cnt = 0;
 tryagain:
  cnt++;
  scm_ftime(&time_buffer1);
  scm_ftime(&time_buffer2);
  if (time_buffer1.time==time_buffer2.time) {
    if (time_buffer1.millitm > time_buffer2.millitm)
      time_buffer2.time = time_buffer2.time + 1;
  }
  else if ((1 + time_buffer1.time)==time_buffer2.time) ;
  else if (cnt < TIMETRIES) goto tryagain;
  else { /* could not read two ftime()s within one second in 10 tries */
    scm_warn("ftime()s too fast", "", MAKINUM(TIMETRIES));
    return MAKINUM(-1);
  }
  tmp = CLKTCK*(time_buffer2.millitm - your_base.millitm);
  tmp = CLKTCK*(time_buffer2.time - your_base.time) + tmp/1000;
  return MAKINUM(tmp);
}
