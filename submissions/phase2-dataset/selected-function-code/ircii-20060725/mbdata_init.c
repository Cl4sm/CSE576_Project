void
mbdata_init(struct mb_data *d, const char *enc)
{
	bzero(d, sizeof(*d));

#ifdef HAVE_ICONV_OPEN
	d->enc = enc;
	if (!d->conv_in && !d->conv_out && d->enc && display_encoding)
	{
		/* New encoding, reinitialize converters */
		
		if (!d->conv_in)
			d->conv_in = iconv_open("UTF-8", d->enc);
		if (!d->conv_out)
			d->conv_out = iconv_open(CP(display_encoding), "UTF-8");
		
		if (d->conv_in == (iconv_t)(-1))
		{
			iconv_close(d->conv_in);
			d->conv_in = NULL;
		}
		if (d->conv_out == (iconv_t)(-1))
		{
			iconv_close(d->conv_out);
			d->conv_out = NULL;
		}
	}
#endif /* HAVE_ICONV_OPEN */
}
