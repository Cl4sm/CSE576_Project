static void setaudio(int *rate, int *format)
{
	static int fragset = 0;
	int frag = 0;
	int fmt;

	frag = (fragnum << 16) + fragsize;

	fmt = to_fmt(*format);
	ioctl(audio_fd, SNDCTL_DSP_SETFMT, &fmt);
	*format = from_fmt(fmt);

	fmt = !(*format & XMP_FORMAT_MONO);
	ioctl(audio_fd, SNDCTL_DSP_STEREO, &fmt);
	if (fmt) {
		*format &= ~XMP_FORMAT_MONO;
	} else {
		*format |= XMP_FORMAT_MONO;
	}

	ioctl(audio_fd, SNDCTL_DSP_SPEED, rate);

	/* Set the fragments only once */
	if (!fragset) {
		if (fragnum && fragsize)
			ioctl(audio_fd, SNDCTL_DSP_SETFRAGMENT, &frag);
		fragset++;
	}
}