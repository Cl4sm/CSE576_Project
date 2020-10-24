int main (int argc, char **argv)
{
  XtAppContext app_context;
  int scroll_sep, longway;
  Widget version;
  int supported;

  longway = (argc > 1 && strcmp(argv[1],"-l") == 0);

  topLevel = XtVaAppInitialize (&app_context,
     "XMix",			/* Application class */
     NULL, 0,			/* command line option list */
     &argc, argv,		/* command line args */
     NULL,			/* for missing app-defaults file */
     NULL);			/* terminate varargs list */

  whole = MW ("whole", formWidgetClass, topLevel, NULL);
  
  sliders = MW ("sliders", formWidgetClass, whole, NULL);
  
  master.mixer_id = SOUND_MIXER_VOLUME;
  master.formw = MW ("master_form", formWidgetClass, sliders, NULL);
  master.labelw =  MW ("master_label", labelWidgetClass, master.formw,
		       XtNlabel, "Master", NULL);
  master.leftw = MW ("master_l", scrollbarWidgetClass, master.formw,
		     SCROLLBAR_RES, XtNfromVert, master.labelw,NULL);
  master.rightw = MW ("master_r", scrollbarWidgetClass, master.formw,
		      SCROLLBAR_RES, XtNfromHoriz, master.leftw,
		      XtNfromVert, master.labelw, NULL);
  master.lockw = MW ("master_lock", commandWidgetClass, master.formw,
		     XtNfromVert, master.leftw, NULL);

  bass.mixer_id = SOUND_MIXER_BASS;
  bass.formw = MW ("bass_form", formWidgetClass, sliders,
		   XtNfromHoriz, master.formw, NULL);
  bass.labelw =  MW ("bass_label", labelWidgetClass, bass.formw,
		     XtNlabel, "Bass", NULL);
  bass.leftw = MW ("bass_l", scrollbarWidgetClass, bass.formw, SCROLLBAR_RES,
		   XtNfromVert, bass.labelw, NULL);
  bass.rightw = MW ("bass_r", scrollbarWidgetClass, bass.formw, SCROLLBAR_RES,
		    XtNfromHoriz, bass.leftw, XtNfromVert, bass.labelw, NULL);
  bass.lockw = MW ("bass_lock", commandWidgetClass, bass.formw,
		   XtNfromVert, bass.leftw, NULL);

  treble.mixer_id = SOUND_MIXER_TREBLE;
  treble.formw = MW ("treble_form", formWidgetClass, sliders,
		     XtNfromHoriz, bass.formw, NULL);
  treble.labelw =  MW ("treble_label", labelWidgetClass, treble.formw,
		       XtNlabel, "Treble", NULL);
  treble.leftw = MW ("treble_l", scrollbarWidgetClass, treble.formw,
		     SCROLLBAR_RES, XtNfromVert, treble.labelw, NULL);
  treble.rightw = MW ("treble_r", scrollbarWidgetClass, treble.formw,
		      SCROLLBAR_RES, XtNfromHoriz, treble.leftw,
		      XtNfromVert, treble.labelw, NULL);
  treble.lockw = MW ("treble_lock", commandWidgetClass, treble.formw,
		     XtNfromVert, treble.leftw, NULL);

  line.mixer_id = SOUND_MIXER_LINE;
  line.formw = MW ("line_form", formWidgetClass, sliders,
		   XtNfromHoriz, treble.formw, NULL);
  line.labelw =  MW ("line_label", labelWidgetClass, line.formw,
		     XtNlabel, "Line", NULL);
  line.leftw = MW ("line_l", scrollbarWidgetClass, line.formw, SCROLLBAR_RES,
		   XtNfromVert, line.labelw, NULL);
  line.rightw = MW ("line_r", scrollbarWidgetClass, line.formw, SCROLLBAR_RES,
		    XtNfromHoriz, line.leftw, XtNfromVert, line.labelw, NULL);
  line.lockw = MW ("line_lock", commandWidgetClass, line.formw,
		   XtNfromVert, line.leftw, NULL);

  dsp.mixer_id = SOUND_MIXER_PCM;
  dsp.formw = MW ("dsp_form", formWidgetClass, sliders,
		  longway ? XtNfromHoriz : XtNfromVert,
		  longway ? line.formw : master.formw,
		  NULL);
  dsp.labelw =  MW ("dsp_label", labelWidgetClass, dsp.formw,
		   XtNlabel, "DSP", NULL);
  dsp.leftw = MW ("dsp_l", scrollbarWidgetClass, dsp.formw, SCROLLBAR_RES,
	      XtNfromVert, dsp.labelw, NULL);
  dsp.rightw = MW ("dsp_r", scrollbarWidgetClass, dsp.formw, SCROLLBAR_RES,
	      XtNfromVert, dsp.labelw,
	      XtNfromHoriz, dsp.leftw, NULL);
  dsp.lockw = MW ("dsp_lock", commandWidgetClass, dsp.formw,
		 XtNfromVert, dsp.leftw, NULL);

  fm.mixer_id = SOUND_MIXER_SYNTH;
  fm.formw = MW ("fm_form", formWidgetClass, sliders,
		 XtNfromHoriz, dsp.formw,
		 longway ? NULL : XtNfromVert, bass.formw, NULL);
  fm.labelw =  MW ("fm_label", labelWidgetClass, fm.formw,
		  XtNlabel, "FM", NULL);
  fm.leftw = MW ("fm_l", scrollbarWidgetClass, fm.formw, SCROLLBAR_RES,
	     XtNfromVert, fm.labelw, NULL);
  fm.rightw = MW ("fm_r", scrollbarWidgetClass, fm.formw, SCROLLBAR_RES,
	     XtNfromVert, fm.labelw,
	     XtNfromHoriz, fm.leftw, NULL);
  fm.lockw = MW ("fm_lock", commandWidgetClass, fm.formw,
		XtNfromVert, fm.leftw, NULL);

  cd.mixer_id = SOUND_MIXER_CD;
  cd.formw = MW ("cd_form", formWidgetClass, sliders,
		 XtNfromHoriz, fm.formw,
		 longway ? NULL : XtNfromVert, treble.formw, NULL);
  cd.labelw =  MW ("cd_label", labelWidgetClass, cd.formw,
		  XtNlabel, "CD", NULL);
  cd.leftw = MW ("cd_l", scrollbarWidgetClass, cd.formw, SCROLLBAR_RES,
	     XtNfromVert, cd.labelw, NULL);
  cd.rightw = MW ("cd_r", scrollbarWidgetClass, cd.formw, SCROLLBAR_RES,
	     XtNfromVert, cd.labelw, XtNfromHoriz, cd.leftw, NULL);
  cd.lockw = MW ("cd_lock", commandWidgetClass, cd.formw,
		XtNfromVert, cd.leftw, NULL);

  mic.mixer_id = SOUND_MIXER_MIC;
  mic.formw = MW ("mic_form", formWidgetClass, sliders,
		  XtNfromHoriz, cd.formw,
		  longway ? NULL : XtNfromVert, line.formw, NULL);
  mic.labelw =  MW ("mic_label", labelWidgetClass, mic.formw,
		   XtNlabel, "Mic", NULL);
  mic.leftw = MW ("mic_level", scrollbarWidgetClass, mic.formw, SCROLLBAR_RES,
		  XtNfromVert, mic.labelw, NULL);
  mic.rightw = NULL;
  mic.lockw = NULL;

  reclvl.mixer_id = SOUND_MIXER_RECLEV;
  reclvl.formw = MW ("reclvl_form", formWidgetClass, sliders,
		     XtNfromHoriz, mic.formw,
		     longway ? NULL : XtNfromVert, line.formw, NULL);
  reclvl.labelw =  MW ("reclvl_label", labelWidgetClass, reclvl.formw,
		       XtNlabel, "Reclvl", NULL);
  reclvl.leftw = MW ("reclvl_level", scrollbarWidgetClass, reclvl.formw,
		     SCROLLBAR_RES, XtNfromVert, reclvl.labelw, NULL);
  reclvl.rightw = NULL;
  reclvl.lockw = NULL;

  buttons = MW ("buttons", formWidgetClass, whole,
		XtNfromHoriz, sliders, NULL);


  version = MW ("Version", labelWidgetClass, buttons,
		XtNlabel, "XMix V2.1", NULL);

  sources = MW ("sources", formWidgetClass, buttons,
		XtNfromVert, version, NULL);
  source_label = MW ("source_label", labelWidgetClass, sources,
		     XtNlabel, "Recording Source", NULL);
  line_src = MW ("line_src", commandWidgetClass, sources,
		 XtNfromVert, source_label, NULL);
  line_src_label = MW ("line_src_label", labelWidgetClass, sources,
		       XtNlabel, "Line In",
		       XtNfromVert, source_label,
		       XtNfromHoriz, line_src, NULL);
  mic_src = MW ("mic_src", commandWidgetClass, sources,
		XtNfromVert, line_src, NULL);
  mic_src_label = MW ("mic_src_label", labelWidgetClass, sources,
		      XtNlabel, "Microphone",
		      XtNfromVert, line_src,
		      XtNfromHoriz, mic_src, NULL);
  cd_src = MW ("cd_src", commandWidgetClass, sources,
	       XtNfromVert, mic_src, NULL);
  cd_src_label = MW ("cd_src_label", labelWidgetClass, sources,
		     XtNlabel, "CD",
		     XtNfromVert, mic_src,
		     XtNfromHoriz, cd_src, NULL);
  quit = MW ("quit", commandWidgetClass, buttons,
	     XtNfromVert, sources,
	     XtNlabel, "Quit",
	     XtNwidth, 50,
	     XtNheight, 20,
	     NULL);


  XtVaGetValues (mic.leftw, XtNwidth, &centering_width, NULL);
  CENTER (mic.labelw);
  XtVaGetValues (reclvl.leftw, XtNwidth, &centering_width, NULL);
  CENTER (reclvl.labelw);
  XtVaGetValues (master.formw, XtNhorizDistance, &scroll_sep, NULL);
  XtVaGetValues (master.leftw, XtNwidth, &centering_width, NULL);
  centering_width = centering_width * 2 + scroll_sep;
  CENTER (master.labelw);
  CENTER (bass.labelw);
  CENTER (treble.labelw);
  CENTER (line.labelw);
  CENTER (dsp.labelw);
  CENTER (fm.labelw);
  CENTER (cd.labelw);

  XtAddEventHandler (topLevel, EnterWindowMask, FALSE,
		     (XtEventHandler) sync_display, NULL);
  XtAddCallback (quit, XtNcallback, Quit, 0 /* client_data */ );
  XtAddCallback (master.leftw, XtNjumpProc, Handle_slider, &master);
  XtAddCallback (master.leftw, XtNscrollProc, Handle_slam_slider, &master);
  XtAddCallback (master.rightw, XtNjumpProc, Handle_slider, &master);
  XtAddCallback (master.rightw, XtNscrollProc, Handle_slam_slider, &master);
  XtAddCallback (master.lockw, XtNcallback, Handle_lock, &master);
  XtAddCallback (bass.leftw, XtNjumpProc, Handle_slider, &bass);
  XtAddCallback (bass.leftw, XtNscrollProc, Handle_slam_slider, &bass);
  XtAddCallback (bass.rightw, XtNjumpProc, Handle_slider, &bass);
  XtAddCallback (bass.rightw, XtNscrollProc, Handle_slam_slider, &bass);
  XtAddCallback (bass.lockw, XtNcallback, Handle_lock, &bass);
  XtAddCallback (treble.leftw, XtNjumpProc, Handle_slider, &treble);
  XtAddCallback (treble.leftw, XtNscrollProc, Handle_slam_slider, &treble);
  XtAddCallback (treble.rightw, XtNjumpProc, Handle_slider, &treble);
  XtAddCallback (treble.rightw, XtNscrollProc, Handle_slam_slider, &treble);
  XtAddCallback (treble.lockw, XtNcallback, Handle_lock, &treble);
  XtAddCallback (line.leftw, XtNjumpProc, Handle_slider, &line);
  XtAddCallback (line.leftw, XtNscrollProc, Handle_slam_slider, &line);
  XtAddCallback (line.rightw, XtNjumpProc, Handle_slider, &line);
  XtAddCallback (line.rightw, XtNscrollProc, Handle_slam_slider, &line);
  XtAddCallback (line.lockw, XtNcallback, Handle_lock, &line);
  XtAddCallback (dsp.leftw, XtNjumpProc, Handle_slider, &dsp);
  XtAddCallback (dsp.leftw, XtNscrollProc, Handle_slam_slider, &dsp);
  XtAddCallback (dsp.rightw, XtNjumpProc, Handle_slider, &dsp);
  XtAddCallback (dsp.rightw, XtNscrollProc, Handle_slam_slider, &dsp);
  XtAddCallback (dsp.lockw, XtNcallback, Handle_lock, &dsp);
  XtAddCallback (fm.leftw, XtNjumpProc, Handle_slider, &fm);
  XtAddCallback (fm.leftw, XtNscrollProc, Handle_slam_slider, &fm);
  XtAddCallback (fm.rightw, XtNjumpProc, Handle_slider, &fm);
  XtAddCallback (fm.rightw, XtNscrollProc, Handle_slam_slider, &fm);
  XtAddCallback (fm.lockw, XtNcallback, Handle_lock, &fm);
  XtAddCallback (cd.leftw, XtNjumpProc, Handle_slider, &cd);
  XtAddCallback (cd.leftw, XtNscrollProc, Handle_slam_slider, &cd);
  XtAddCallback (cd.rightw, XtNjumpProc, Handle_slider, &cd);
  XtAddCallback (cd.rightw, XtNscrollProc, Handle_slam_slider, &cd);
  XtAddCallback (cd.lockw, XtNcallback, Handle_lock, &cd);
  XtAddCallback (mic.leftw, XtNjumpProc, Handle_slider, &mic);
  XtAddCallback (mic.leftw, XtNscrollProc, Handle_slam_slider, &mic);
  XtAddCallback (reclvl.leftw, XtNjumpProc, Handle_slider, &reclvl);
  XtAddCallback (reclvl.leftw, XtNscrollProc, Handle_slam_slider, &reclvl);

  XtAddCallback (line_src, XtNcallback, Handle_source,
		 (XtPointer)SOUND_MIXER_LINE);
  XtAddCallback (mic_src, XtNcallback, Handle_source,
		 (XtPointer)SOUND_MIXER_MIC);
  XtAddCallback (cd_src, XtNcallback, Handle_source,
		 (XtPointer)SOUND_MIXER_CD);


  install_pixmaps ();

  /* Open the mixer device */
  mixer_fd = open ("/dev/mixer", O_RDWR, 0);
  if (mixer_fd < 0)
    perror ("Error opening mixer device"), exit (1);

  
  if (ioctl(mixer_fd, SOUND_MIXER_READ_DEVMASK, &supported) == -1)
    supported = 0xffff;	/* Assume all are supported */

  /*
   * Match the display settings to the current mixer configuration.
   */
  sync_display();

  /*
   * Pick some reasonable lock settings to start with.
   * Two equal volume levels start off with that pair linked.
   */
  
  master.locked = (master.volume.left == master.volume.right);
  bass.locked = (bass.volume.left == bass.volume.right);
  treble.locked = (treble.volume.left == treble.volume.right);
  line.locked = (line.volume.left == line.volume.right);
  dsp.locked = (dsp.volume.left == dsp.volume.right);
  fm.locked = (fm.volume.left == fm.volume.right);
  cd.locked = (cd.volume.left == cd.volume.right);
  mic.locked = 0;
  reclvl.locked = 0;

#ifdef DEBUG
  printf("supported = 0x%x\n",supported);
#endif
  set_supported(&master,(supported & SOUND_MASK_VOLUME) != 0);
  set_supported(&bass,(supported & SOUND_MASK_BASS) != 0);
  set_supported(&treble,(supported & SOUND_MASK_TREBLE) != 0);
  set_supported(&line,(supported & SOUND_MASK_LINE) != 0);
  set_supported(&dsp,(supported & SOUND_MASK_PCM) != 0);
  set_supported(&fm,(supported & SOUND_MASK_SYNTH) != 0);
  set_supported(&cd,(supported & SOUND_MASK_CD) != 0);
  set_supported(&mic,(supported & SOUND_MASK_MIC) != 0);
  set_supported(&reclvl,(supported & SOUND_MASK_RECLEV) != 0);

  /*
   * Update the lock bitmaps to reflect linking
   */
  sync_lock(&master);
  sync_lock(&bass);
  sync_lock(&treble);
  sync_lock(&line);
  sync_lock(&dsp);
  sync_lock(&fm);
  sync_lock(&cd);

  XtRealizeWidget (topLevel);	/* Action! */
  XtAppMainLoop (app_context);	/* Loop for events */
  return 0;
}