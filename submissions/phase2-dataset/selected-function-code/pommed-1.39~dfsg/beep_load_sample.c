static struct sample *
beep_load_sample(char *filename)
{
  AFfilehandle affd;     /* filehandle for soundfile from libaudiofile */
  AFframecount framecount;
  int dummy, channels, byteorder, framesize, precision;
  struct sample *sample;

  int ret;

  sample = (struct sample *) malloc(sizeof(struct sample));
  if (sample == NULL)
    return NULL;

  affd = afOpenFile(filename, "r", 0);
  if (!affd)
    {
      free(sample);
      return NULL;
    }

  afGetSampleFormat(affd, AF_DEFAULT_TRACK, &dummy, &precision);
  channels = afGetChannels(affd, AF_DEFAULT_TRACK);
  byteorder = afGetVirtualByteOrder(affd, AF_DEFAULT_TRACK);
  framesize = (int) afGetFrameSize(affd, AF_DEFAULT_TRACK, 0);
  framecount = afGetFrameCount(affd, AF_DEFAULT_TRACK);
  sample->speed = (int) afGetRate(affd, AF_DEFAULT_TRACK);

  if (channels <= 2)
    sample->channels = channels;
  else
    goto error_out;

  switch (precision)
    {
      case 8:
	sample->format = SND_PCM_FORMAT_S8;
	break;
      case 16:
	if (byteorder == AF_BYTEORDER_LITTLEENDIAN)
	  sample->format = SND_PCM_FORMAT_S16_LE;
	else
	  sample->format = SND_PCM_FORMAT_S16_BE;
	break;
      default:
	goto error_out;
	break;
    }

  sample->framesize = framesize;
  sample->periods = sample->framesize;
  sample->buffersize = (sample->periods * 8192) >> 2;
  sample->framecount = framecount;
  sample->audiodatalen = framecount * framesize;

  sample->audiodata = (char *) malloc(sample->audiodatalen);
  if (sample->audiodata != NULL)
    {
      ret = afReadFrames(affd, AF_DEFAULT_TRACK, sample->audiodata, framecount);
      if (ret != framecount)
	{
	  free(sample->audiodata);
	  goto error_out;
	}
    }
  else
    goto error_out;

  afCloseFile(affd);

  return sample;

 error_out: /* something bad happened */
  afCloseFile(affd);
  free(sample);
  return NULL;
}