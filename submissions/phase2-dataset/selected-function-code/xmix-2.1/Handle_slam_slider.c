static void Handle_slam_slider(Widget w, XtPointer client_data,
			       XtPointer call_data)
{
  int val;
  VolumeControl *vcptr = client_data;

  val = (vcptr->leftw == w) ? vcptr->volume.left : vcptr->volume.right;
  if ((int)call_data < 0)
    {
      if (val >= MAX_SOUND_VOL)
	val = 10*((MAX_SOUND_VOL-1)/10);
      else
	val -= 10;
      if (val < MIN_SOUND_VOL) val = MIN_SOUND_VOL;
    }
  else
    {
      if (val <= MIN_SOUND_VOL)
	val = 10*((MIN_SOUND_VOL+9)/10);
      else
	val += 10;
      if (val > MAX_SOUND_VOL) val = MAX_SOUND_VOL;
    }

  set_slider(vcptr,w,val);
  return;
}