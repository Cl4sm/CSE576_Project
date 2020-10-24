void
audio_step(int dir)
{
  long vol;
  long newvol;

  if (mixer_hdl == NULL)
    return;

  if (vol_elem == NULL)
    return;

  snd_mixer_handle_events(mixer_hdl);

  if (!snd_mixer_selem_is_active(vol_elem))
    return;

  snd_mixer_selem_get_playback_volume(vol_elem, 0, &vol);

  logdebug("Mixer volume: %ld\n", vol);

  if (dir == STEP_UP)
    {
      newvol = vol + vol_step;

      if (newvol > vol_max)
	newvol = vol_max;

      logdebug("Audio stepping +%ld -> %ld\n", vol_step, newvol);
    }
  else if (dir == STEP_DOWN)
    {
      newvol = vol - vol_step;

      if (newvol < vol_min)
	newvol = vol_min;

      logdebug("Audio stepping -%ld -> %ld\n", vol_step, newvol);
    }
  else
    return;

  snd_mixer_selem_set_playback_volume(vol_elem, 0, newvol);

  if (snd_mixer_selem_is_playback_mono(vol_elem) == 0)
    snd_mixer_selem_set_playback_volume(vol_elem, 1, newvol);

  if (audio_cfg.beep)
    beep_audio();

  mbpdbus_send_audio_volume(newvol, vol);

  audio_info.level = newvol;
}