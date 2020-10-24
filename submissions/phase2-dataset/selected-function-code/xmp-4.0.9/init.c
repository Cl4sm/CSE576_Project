static int init(struct options *options)
{
	char **parm = options->driver_parm;
	static const char *dev_audio[] = { "/dev/dsp", "/dev/sound/dsp" };
	audio_buf_info info;
	static char buf[80];
	int i;

	fragnum = 16;		/* default number of fragments */
	i = 1024;		/* default size of fragment */
	
	parm_init(parm);
	chkparm2("frag", "%d,%d", &fragnum, &i);
	chkparm1("dev", dev_audio[0] = token);
	chkparm0("nosync", do_sync = 0);
	parm_end();

	for (fragsize = 0; i >>= 1; fragsize++) ;
	if (fragsize < 4)
		fragsize = 4;

	for (i = 0; i < sizeof(dev_audio) / sizeof(dev_audio[0]); i++)
		if ((audio_fd = open(dev_audio[i], O_WRONLY)) >= 0)
			break;
	if (audio_fd < 0)
		return -1;

	setaudio(&options->rate, &options->format);

	if (ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info) == 0) {
		snprintf(buf, 80, "%s [%d fragments of %d bytes]",
			 sound_oss.description, info.fragstotal,
			 info.fragsize);
		sound_oss.description = buf;
	}

	return 0;
}