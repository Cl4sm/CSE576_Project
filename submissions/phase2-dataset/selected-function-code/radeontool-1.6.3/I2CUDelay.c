static void
I2CUDelay(I2CBusPtr b, int usec)
{
#if 0
  struct timeval begin, cur;
  long d_secs, d_usecs;
  long diff;

  if (usec > 0) {
    X_GETTIMEOFDAY(&begin);
    do {
      /* It would be nice to use {xf86}usleep, 
       * but usleep (1) takes >10000 usec !
       */
      X_GETTIMEOFDAY(&cur);
      d_secs  = (cur.tv_sec - begin.tv_sec);
      d_usecs = (cur.tv_usec - begin.tv_usec);
      diff = d_secs*1000000 + d_usecs;
    } while (diff>=0 && diff< (usec + 1));
  }
#else
  usleep(usec);
#endif
}