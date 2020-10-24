static void
beep_play_sample(struct dspdata *dsp, int cmd)
{
  snd_pcm_t *pcm_handle;          
  snd_pcm_hw_params_t *hwparams;

  char *pcm_name = "default";

  struct sample *s = dsp->sample[cmd];

  snd_pcm_hw_params_alloca(&hwparams);

  if (snd_pcm_open(&pcm_handle, pcm_name, SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {
      logmsg(LOG_WARNING, "beep: error opening PCM device %s", pcm_name);
      return;
    }

  if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0)
    {
      logmsg(LOG_WARNING, "beep: cannot configure PCM device");
      return;
    }

  if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
    {
      logmsg(LOG_WARNING, "beep: error setting access");
      return;
    }

  if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, s->format) < 0)
    {
      logmsg(LOG_WARNING, "beep: error setting format");
      return;
    }

  if (snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &s->speed, 0) < 0)
    {
      logmsg(LOG_WARNING, "beep: error setting rate");
      return;
    }

  /* Set number of channels */
  if (snd_pcm_hw_params_set_channels_near(pcm_handle, hwparams, &s->channels) < 0)
    {
      logmsg(LOG_WARNING, "beep: error setting channels");
      return;
    }

  /* Set number of periods. Periods used to be called fragments. */ 
  if (snd_pcm_hw_params_set_periods_near(pcm_handle, hwparams, &s->periods, 0) < 0)
    {
      logmsg(LOG_WARNING, "beep: error setting periods");
      return;
    }

  /* Set buffer size (in frames). The resulting latency is given by */
  /* latency = periodsize * periods / (rate * bytes_per_frame)     */
  if (snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hwparams, &s->buffersize) < 0)
    {
      logmsg(LOG_WARNING, "beep: error setting buffersize");
      return;
    }

  /* Apply HW parameter settings to */
  /* PCM device and prepare device  */
  if (snd_pcm_hw_params(pcm_handle, hwparams) < 0)
    {
      logmsg(LOG_WARNING, "beep: error setting HW params");
      return;
    }

  int pcmreturn;
  /* Write num_frames frames from buffer data to    */ 
  /* the PCM device pointed to by pcm_handle.       */
  /* Returns the number of frames actually written. */
  while ((pcmreturn = snd_pcm_writei(pcm_handle, s->audiodata, s->framecount)) < 0)
    {
      snd_pcm_prepare(pcm_handle);
    }

  /* Stop PCM device and drop pending frames */
  snd_pcm_drop(pcm_handle);

  /* Stop PCM device after pending frames have been played */ 
  snd_pcm_close(pcm_handle);
}