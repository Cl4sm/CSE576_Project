int
audio_init(void)
{
  snd_mixer_elem_t *elem;
  snd_mixer_selem_id_t *sid;

  double dvol;
  long vol;

  int ret;

  vol_elem = NULL;
  spkr_elem = NULL;
  head_elem = NULL;

  if (audio_cfg.disabled)
    {
      audio_info.level = 0;
      audio_info.max = 0;
      audio_info.muted = 1;

      return 0;
    }

  play = 1;

  ret = snd_mixer_open(&mixer_hdl, 0);
  if (ret < 0)
    {
      logdebug("Failed to open mixer: %s\n", snd_strerror(ret));

      mixer_hdl = NULL;

      return -1;
    }

  ret = snd_mixer_attach(mixer_hdl, audio_cfg.card);
  if (ret < 0)
    {
      logdebug("Failed to attach mixer: %s\n", snd_strerror(ret));

      snd_mixer_close(mixer_hdl);

      return -1;
    }

  ret = snd_mixer_selem_register(mixer_hdl, NULL, NULL);
  if (ret < 0)
    {
      logdebug("Failed to register mixer: %s\n", snd_strerror(ret));

      snd_mixer_detach(mixer_hdl, audio_cfg.card);
      snd_mixer_close(mixer_hdl);

      return -1;
    }

  ret = snd_mixer_load(mixer_hdl);
  if (ret < 0)
    {
      logdebug("Failed to load mixer: %s\n", snd_strerror(ret));

      snd_mixer_detach(mixer_hdl, audio_cfg.card);
      snd_mixer_close(mixer_hdl);

      return -1;
    }


  /* Grab interesting elements */
  snd_mixer_selem_id_alloca(&sid);

  for (elem = snd_mixer_first_elem(mixer_hdl); elem; elem = snd_mixer_elem_next(elem)) 
    {
      snd_mixer_selem_get_id(elem, sid);

      if (strcmp(snd_mixer_selem_id_get_name(sid), audio_cfg.vol) == 0)
	vol_elem = elem;

      if (strcmp(snd_mixer_selem_id_get_name(sid), audio_cfg.spkr) == 0)
	spkr_elem = elem;

      if (strcmp(snd_mixer_selem_id_get_name(sid), audio_cfg.head) == 0)
	head_elem = elem;
    }

  logdebug("Audio init: volume %s, speakers %s, headphones %s\n",
	   (vol_elem == NULL) ? "NOK" : "OK",
	   (spkr_elem == NULL) ? "NOK" : "OK",
	   (head_elem == NULL) ? "NOK" : "OK");

  if ((vol_elem == NULL) || ((spkr_elem == NULL) && (head_elem == NULL)))
    {
      logdebug("Failed to open required mixer elements\n");

      audio_cleanup();

      return -1;
    }

  /* Get min & max volume */
  snd_mixer_selem_get_playback_volume_range(vol_elem, &vol_min, &vol_max);

  dvol = (double)(vol_max - vol_min) / 100.0;
  vol_step = (long)(dvol * (double)audio_cfg.step);

  logdebug("Audio init: min %ld, max %ld, step %ld\n", vol_min, vol_max, vol_step);

  /* Set initial volume if enabled */
  if (audio_cfg.init > -1)
    {
      dvol *= (double)audio_cfg.init;
      vol = (long)dvol;

      if (vol > vol_max)
	vol = vol_max;

      snd_mixer_selem_set_playback_volume(vol_elem, 0, vol);

      if (snd_mixer_selem_is_playback_mono(vol_elem) == 0)
	snd_mixer_selem_set_playback_volume(vol_elem, 1, vol);      
    }

  snd_mixer_handle_events(mixer_hdl);
  snd_mixer_selem_get_playback_volume(vol_elem, 0, &vol);

  audio_info.level = vol;
  audio_info.max = vol_max;
  audio_info.muted = !play;

  return 0;
}