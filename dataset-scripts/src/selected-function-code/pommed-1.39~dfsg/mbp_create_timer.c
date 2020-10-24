static int
mbp_create_timer(int timeout)
{
  int fd;
  int ret;

  struct itimerspec timing;

  fd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (fd < 0)
    {
      fprintf(stderr, "Could not create timer: %s", strerror(errno));

      return -1;
    }

  timing.it_interval.tv_sec = (timeout >= 1000) ? timeout / 1000 : 0;
  timing.it_interval.tv_nsec = (timeout - (timing.it_interval.tv_sec * 1000)) * 1000000;

  ret = clock_gettime(CLOCK_MONOTONIC, &timing.it_value);
  if (ret < 0)
    {
      fprintf(stderr, "Could not get current time: %s", strerror(errno));

      close(fd);
      return -1;
    }

  timing.it_value.tv_sec += timing.it_interval.tv_sec;
  timing.it_value.tv_nsec += timing.it_interval.tv_nsec;
  if (timing.it_value.tv_nsec > 1000000000)
    {
      timing.it_value.tv_sec++;
      timing.it_value.tv_nsec -= 1000000000;
    }

  ret = timerfd_settime(fd, TFD_TIMER_ABSTIME, &timing, NULL);
  if (ret < 0)
    {
      fprintf(stderr, "Could not setup timer: %s", strerror(errno));

      close(fd);
      return -1;
    }

  return fd;
}