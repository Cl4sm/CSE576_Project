TimerTimeout(tv)
	struct timeval *tv;
{
	struct timeval current;

	tv->tv_usec =0;
	tv->tv_sec  =0;
	
	if (!PendingTimers)
	{
		tv->tv_sec = 70; /* Just larger than the maximum of 60 */
		return;
	}
	gettimeofday(&current, NULL);
	
	if (PendingTimers->time < current.tv_sec ||
	    (PendingTimers->time == current.tv_sec &&
	    PendingTimers->microseconds < current.tv_usec))
	{
		/* No time to lose, the event is now or was */
		return;
	}
	
	tv->tv_sec = PendingTimers->time - current.tv_sec;
	if (PendingTimers->microseconds >= current.tv_usec)
		tv->tv_usec = PendingTimers->microseconds - current.tv_usec;
	else
	{
		tv->tv_usec = current.tv_usec - PendingTimers->microseconds;
		tv->tv_sec -= 1;
	}
}
