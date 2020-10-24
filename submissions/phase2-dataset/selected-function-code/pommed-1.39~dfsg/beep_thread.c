void *
beep_thread (void *arg)
{
  struct dspdata *dsp = (struct dspdata *) arg;
  for (;;)
    {
      pthread_mutex_lock(&dsp->mutex);
      pthread_cond_wait(&dsp->cond, &dsp->mutex);
      pthread_mutex_unlock(&dsp->mutex);

      switch (dsp->command)
	{
	  case AUDIO_CLICK:
	    dsp->command = AUDIO_COMMAND_NONE;

	    beep_play_sample(dsp, AUDIO_CLICK);
	    break;
	  case AUDIO_COMMAND_QUIT:
	    pthread_exit(NULL);
	    break;
	  case AUDIO_COMMAND_NONE:
	    break;
	}
    }

  return NULL;
}