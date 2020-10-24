static void set_slider(VolumeControl *vcptr, Widget w, int value)
{
  float portion = PORTION(value);
  int update_needed;

  DPRINTF (("set_slider: val = %d, portion = %f\n",value,portion));

  if (vcptr->locked)
    {
      update_needed = (vcptr->volume.left != value);
      DPRINTF (("set_slider: locked; updating\n"));
      vcptr->volume.left = vcptr->volume.right = value;
      XawScrollbarSetThumb(vcptr->leftw, portion, SLIDER_LENGTH);
      XawScrollbarSetThumb(vcptr->rightw, portion, SLIDER_LENGTH);
    }
  else
    {
      if (vcptr->leftw == w)
	{
	  update_needed = (vcptr->volume.left != value);
	  vcptr->volume.left = value;
	}
      else
	{
	  update_needed = (vcptr->volume.right != value);
	  vcptr->volume.right = value;
	}
      XawScrollbarSetThumb(w, portion, SLIDER_LENGTH);
    }

  /*
   * If the new value is at a different notch than the current setting,
   * then inform the mixer, and adopt the setting.
   * Otherwise, don't waste time setting the mixer.
   */
  if (update_needed)
    {
       unsigned long vol = vcptr->volume.left | (vcptr->volume.right << 8);
      DPRINTF (("set_slider: updating mixer\n"));
      if (vcptr->supported
        && ioctl(mixer_fd,MIXER_WRITE(vcptr->mixer_id),&vol) == -1)
	perror("Error writing mixer in Handle_slider");
    }
  return;
}