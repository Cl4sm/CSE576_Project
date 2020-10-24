set_time(void)
{
  struct timeval newtime = { .tv_sec = 0, .tv_usec = 0 };

  if (gettimeofday(&newtime, NULL) == -1)
  {
    ilog(LOG_TYPE_IRCD, "Clock Failure (%s), TS can be corrupted",
         strerror(errno));
    sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                         "Clock Failure (%s), TS can be corrupted",
                         strerror(errno));
    server_die("Clock Failure", 1);
  }

  if (newtime.tv_sec < CurrentTime)
  {
    ilog(LOG_TYPE_IRCD, "System clock is running backwards - (%lu < %lu)",
         (unsigned long)newtime.tv_sec, (unsigned long)CurrentTime);
    sendto_realops_flags(UMODE_DEBUG, L_ALL, SEND_NOTICE,
                         "System clock is running backwards - (%lu < %lu)",
                         (unsigned long)newtime.tv_sec,
                         (unsigned long)CurrentTime);
    set_back_events(CurrentTime - newtime.tv_sec);
  }

  SystemTime.tv_sec  = newtime.tv_sec;
  SystemTime.tv_usec = newtime.tv_usec;
}
