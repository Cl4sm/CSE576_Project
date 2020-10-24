  int status=0;

#ifdef THR
#ifdef _MSC_VER
#if defined(THRUSENEWWIN32APIS)
  /* TryEnterCriticalSection() is only available on newer */
  /* versions of Win32: _WIN32_WINNT/WINVER >= 0x0400     */
  while (!TryEnterCriticalSection(mp));
#else
  EnterCriticalSection(mp);
#endif
#endif /* _MSC_VER */

#ifdef USEPOSIXTHREADS
  while ((status = pthread_mutex_trylock(mp)) != 0);
#endif /* USEPOSIXTHREADS */
#endif /* THR */

  return status;
}
