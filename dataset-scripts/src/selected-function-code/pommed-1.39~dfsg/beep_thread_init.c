static int
beep_thread_init(void)
{
  pthread_attr_t attr;
  int ret;

  _dsp.sample[AUDIO_CLICK] = beep_load_sample(beep_cfg.beepfile);

  if (_dsp.sample[AUDIO_CLICK] == NULL)
    return -1;

  _dsp.thread = 0;

  pthread_mutex_init(&(_dsp.mutex), NULL);
  pthread_cond_init (&(_dsp.cond), NULL);
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  ret = pthread_create(&(_dsp.thread), &attr, beep_thread, (void *) &_dsp);
  if (ret != 0)
    {
      beep_thread_cleanup();
      ret = -1;
    }

  pthread_attr_destroy(&attr);

  return ret;
}