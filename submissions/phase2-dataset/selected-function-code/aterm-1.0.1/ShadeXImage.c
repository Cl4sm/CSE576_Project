{
  int sh_r, sh_g, sh_b;
  RUINT32T mask_r, mask_g, mask_b;
  RUINT32T *lookup, *lookup_r, *lookup_g, *lookup_b;
  unsigned int lower_lim_r, lower_lim_g, lower_lim_b;
  unsigned int upper_lim_r, upper_lim_g, upper_lim_b;
  int i;

#ifdef DO_CLOCKING
  clock_t started = clock();
#endif

#ifdef ATERM
  Visual* visual = DefaultVisual(Xdisplay, Xscreen);
#else
  Visual* visual = DefaultVisual(dpy, DefaultScreen(dpy));
#endif

  if( visual->class != TrueColor || srcImage->format != ZPixmap ) return ;

  /* for convenience */
  mask_r = visual->red_mask;
  mask_g = visual->green_mask;
  mask_b = visual->blue_mask;

  /* boring lookup table pre-initialization */
  switch (srcImage->bits_per_pixel) {
    case 15:
      if ((mask_r != 0x7c00) ||
          (mask_g != 0x03e0) ||
          (mask_b != 0x001f))
        return;
        lookup = (RUINT32T *) malloc (sizeof (RUINT32T)*(32+32+32));
        lookup_r = lookup;
        lookup_g = lookup+32;
        lookup_b = lookup+32+32;
        sh_r = 10;
        sh_g = 5;
        sh_b = 0;
      break;
    case 16:
      if ((mask_r != 0xf800) ||
          (mask_g != 0x07e0) ||
          (mask_b != 0x001f))
        return;
        lookup = (RUINT32T *) malloc (sizeof (RUINT32T)*(32+64+32));
        lookup_r = lookup;
        lookup_g = lookup+32;
        lookup_b = lookup+32+64;
        sh_r = 11;
        sh_g = 5;
        sh_b = 0;
      break;
    case 24:
      if ((mask_r != 0xff0000) ||
          (mask_g != 0x00ff00) ||
          (mask_b != 0x0000ff))
        return;
        lookup = (RUINT32T *) malloc (sizeof (RUINT32T)*(256+256+256));
        lookup_r = lookup;
        lookup_g = lookup+256;
        lookup_b = lookup+256+256;
        sh_r = 16;
        sh_g = 8;
        sh_b = 0;
      break;
    case 32:
      if ((mask_r != 0xff0000) ||
          (mask_g != 0x00ff00) ||
          (mask_b != 0x0000ff))
        return;
        lookup = (RUINT32T *) malloc (sizeof (RUINT32T)*(256+256+256));
        lookup_r = lookup;
        lookup_g = lookup+256;
        lookup_b = lookup+256+256;
        sh_r = 16;
        sh_g = 8;
        sh_b = 0;
      break;
    default:
      return; /* we do not support this color depth */
  }

  /* prepare limits for color transformation (each channel is handled separately) */
  if (shading->shading < 0) {
    int shade;
    shade = -shading->shading;
    if (shade < 0) shade = 0;
    if (shade > 100) shade = 100;

    lower_lim_r = 65535-shading->tintColor.red;
    lower_lim_g = 65535-shading->tintColor.green;
    lower_lim_b = 65535-shading->tintColor.blue;

    lower_lim_r = 65535-(unsigned int)(((RUINT32T)lower_lim_r)*((RUINT32T)shade)/100);
    lower_lim_g = 65535-(unsigned int)(((RUINT32T)lower_lim_g)*((RUINT32T)shade)/100);
    lower_lim_b = 65535-(unsigned int)(((RUINT32T)lower_lim_b)*((RUINT32T)shade)/100);

    upper_lim_r = upper_lim_g = upper_lim_b = 65535;
  } else {
    int shade;
    shade = shading->shading;
    if (shade < 0) shade = 0;
    if (shade > 100) shade = 100;

    lower_lim_r = lower_lim_g = lower_lim_b = 0;

    upper_lim_r = (unsigned int)((((RUINT32T)shading->tintColor.red)*((RUINT32T)shading->shading))/100);
    upper_lim_g = (unsigned int)((((RUINT32T)shading->tintColor.green)*((RUINT32T)shading->shading))/100);
    upper_lim_b = (unsigned int)((((RUINT32T)shading->tintColor.blue)*((RUINT32T)shading->shading))/100);
  }

  /* switch red and blue bytes if necessary, we need it for some weird XServers like XFree86 3.3.3.1 */
  if ((srcImage->bits_per_pixel == 24) && (mask_r >= 0xFF0000 ))
  {
    unsigned int tmp;

    tmp = lower_lim_r;
    lower_lim_r = lower_lim_b;
    lower_lim_b = tmp;

    tmp = upper_lim_r;
    upper_lim_r = upper_lim_b;
    upper_lim_b = tmp;
  }

  /* fill our lookup tables */
  for (i = 0; i <= mask_r>>sh_r; i++)
  {
    RUINT32T tmp;
    tmp = ((RUINT32T)i)*((RUINT32T)(upper_lim_r-lower_lim_r));
    tmp += ((RUINT32T)(mask_r>>sh_r))*((RUINT32T)lower_lim_r);
    lookup_r[i] = (tmp/65535)<<sh_r;
  }
  for (i = 0; i <= mask_g>>sh_g; i++)
  {
    RUINT32T tmp;
    tmp = ((RUINT32T)i)*((RUINT32T)(upper_lim_g-lower_lim_g));
    tmp += ((RUINT32T)(mask_g>>sh_g))*((RUINT32T)lower_lim_g);
    lookup_g[i] = (tmp/65535)<<sh_g;
  }
  for (i = 0; i <= mask_b>>sh_b; i++)
  {
    RUINT32T tmp;
    tmp = ((RUINT32T)i)*((RUINT32T)(upper_lim_b-lower_lim_b));
    tmp += ((RUINT32T)(mask_b>>sh_b))*((RUINT32T)lower_lim_b);
    lookup_b[i] = (tmp/65535)<<sh_b;
  }

  /* apply table to input image (replacing colors by newly calculated ones) */
  switch (srcImage->bits_per_pixel)
  {
    case 15:
    {
      unsigned short *p1, *pf, *p, *pl;
      p1 = (unsigned short *) srcImage->data;
      pf = (unsigned short *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);
      while (p1 < pf)
      {
        p = p1;
        pl = p1 + srcImage->width;
        for (; p < pl; p++)
        {
          *p = lookup_r[(*p & 0x7c00)>>10] |
               lookup_g[(*p & 0x03e0)>> 5] |
               lookup_b[(*p & 0x001f)];
        }
        p1 = (unsigned short *) ((char *) p1 + srcImage->bytes_per_line);
      }
      break;
    }
    case 16:
    {
      unsigned short *p1, *pf, *p, *pl;
      p1 = (unsigned short *) srcImage->data;
      pf = (unsigned short *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);
      while (p1 < pf)
      {
        p = p1;
        pl = p1 + srcImage->width;
        for (; p < pl; p++)
        {
          *p = lookup_r[(*p & 0xf800)>>11] |
               lookup_g[(*p & 0x07e0)>> 5] |
               lookup_b[(*p & 0x001f)];
        }
        p1 = (unsigned short *) ((char *) p1 + srcImage->bytes_per_line);
      }
      break;
    }
    case 24:
    {
      unsigned char *p1, *pf, *p, *pl;
      p1 = (unsigned char *) srcImage->data;
      pf = (unsigned char *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);
      while (p1 < pf)
      {
        p = p1;
        pl = p1 + srcImage->width * 3;
        for (; p < pl; p += 3)
        {
          p[0] = lookup_r[(p[0] & 0xff0000)>>16];
          p[1] = lookup_r[(p[1] & 0x00ff00)>> 8];
          p[2] = lookup_r[(p[2] & 0x0000ff)];
        }
        p1 = (unsigned char *) ((char *) p1 + srcImage->bytes_per_line);
      }
      break;
    }
    case 32:
    {
      RUINT32T *p1, *pf, *p, *pl;
      p1 = (RUINT32T *) srcImage->data;
      pf = (RUINT32T *) (srcImage->data + srcImage->height * srcImage->bytes_per_line);

      while (p1 < pf)
      {
        p = p1;
        pl = p1 + srcImage->width;
        for (; p < pl; p++)
        {
          *p = lookup_r[(*p & 0xff0000)>>16] |
               lookup_g[(*p & 0x00ff00)>> 8] |
               lookup_b[(*p & 0x0000ff)] |
               (*p & ~0xffffff);
        }
        p1 = (RUINT32T *) ((char *) p1 + srcImage->bytes_per_line);
      }
      break;
    }
  }

  free (lookup);

#ifdef DO_CLOCKING
    printf( "\n Shading time (clocks): %lu\n",clock()-started );
#endif

}
