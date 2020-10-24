static void sync_source_display(void)
{
  int source,id;
  
  if (ioctl(mixer_fd,SOUND_MIXER_READ_RECSRC,&source) == -1)
    perror("Error reading mixer recording source");
  DPRINTF(("Current recording source is %d\n",source));
  id = 0;
  if (source == 0)
    {
      id = SOUND_MIXER_MIC;
      source = 1 << id;
      if (ioctl(mixer_fd,SOUND_MIXER_WRITE_RECSRC,&source) == -1)
	perror("Error setting mixer recording source to MIC!");
    }
  else
    {
      while ((source & 1) == 0)
	{
	  source = source>>1;
	  ++id;
	}
    }
  DPRINTF(("Current recording source ID=%d\n",id));
  switch (id)
    {
    case SOUND_MIXER_MIC:
      XtVaSetValues (mic_src, XtNbitmap, circle_on_pixmap, NULL);
      XtVaSetValues (cd_src, XtNbitmap, circle_off_pixmap, NULL);
      XtVaSetValues (line_src, XtNbitmap, circle_off_pixmap, NULL);
      break;
    case SOUND_MIXER_CD:
      XtVaSetValues (mic_src, XtNbitmap, circle_off_pixmap, NULL);
      XtVaSetValues (cd_src, XtNbitmap, circle_on_pixmap, NULL);
      XtVaSetValues (line_src, XtNbitmap, circle_off_pixmap, NULL);
      break;
    case SOUND_MIXER_LINE:
      XtVaSetValues (mic_src, XtNbitmap, circle_off_pixmap, NULL);
      XtVaSetValues (cd_src, XtNbitmap, circle_off_pixmap, NULL);
      XtVaSetValues (line_src, XtNbitmap, circle_on_pixmap, NULL);
      break;
    default:
      fprintf (stderr, "Invalid recording source!\n");
    }
  return;
}