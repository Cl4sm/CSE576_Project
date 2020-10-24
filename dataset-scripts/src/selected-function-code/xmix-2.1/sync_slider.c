static void sync_slider(VolumeControl *vcptr)
{
  float portion;
  unsigned long vol;

  if (!vcptr->supported)
    return;
  if (ioctl(mixer_fd,MIXER_READ(vcptr->mixer_id),&vol) == -1)
    perror("Error reading volumes in sync_slider");

  vcptr->volume.left = vol&0xff;
  vcptr->volume.right = (vol>>8) & 0xff;

  portion = PORTION(vcptr->volume.left);
  XawScrollbarSetThumb (vcptr->leftw, portion, SLIDER_LENGTH);
  
  if (vcptr->rightw != NULL)
    {
      portion = PORTION(vcptr->volume.right);
      XawScrollbarSetThumb (vcptr->rightw, portion, SLIDER_LENGTH);
    }
  return;
}