static int to_fmt(struct options *options)
{
	int fmt;

	if (options->format & XMP_FORMAT_8BIT) {
		fmt = SND_PCM_SFMT_U8 | SND_PCM_SFMT_S8;
	} else {
		fmt = SND_PCM_SFMT_S16_LE | SND_PCM_SFMT_S16_BE |
		      SND_PCM_SFMT_U16_LE | SND_PCM_SFMT_U16_BE;

		if (is_big_endian()) {
			fmt &= SND_PCM_SFMT_S16_BE | SND_PCM_SFMT_U16_BE;
		} else {
			fmt &= SND_PCM_SFMT_S16_LE | SND_PCM_SFMT_U16_LE;
		}
	}

	if (options->format & XMP_FORMAT_UNSIGNED) {
		fmt &= SND_PCM_SFMT_U8|SND_PCM_SFMT_U16_LE|SND_PCM_SFMT_U16_BE;
	} else {
		fmt &= SND_PCM_SFMT_S8|SND_PCM_SFMT_S16_LE|SND_PCM_SFMT_S16_BE;
	}

	return fmt;
}