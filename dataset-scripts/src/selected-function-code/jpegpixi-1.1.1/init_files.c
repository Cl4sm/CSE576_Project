void
init_files (const char *const infilename, const char *const outfilename,
            unsigned int *const image_width, unsigned int *const image_height)
{
    /* Initialize jpeglib objects.  */
    infilename1 = infilename;
    outfilename1 = outfilename;

    jpeg_std_error (&errmgr);
    errmgr.output_message = &display_error_message;

    jpeg_create_decompress (&injpg);
    injpg.err = &errmgr;
    jpeg_create_compress (&outjpg);
    outjpg.err = &errmgr;


    /* Open input file.  */
    if (infilename != 0)
    {
        if ((infile = fopen (infilename, "rb")) == 0)
        {
            fprintf (stderr, _("%s: %s: %s\n"), invocation_name, infilename, strerror (errno));
            exit (1);
        }
    }
    else
        infile = stdin;

    jpeg_stdio_src (&injpg, infile);


    /* Save all markers, unless file is to be stripped.  */
    if (!opt_strip)
    {
        int i;

        jpeg_save_markers (&injpg, JPEG_COM, 0xffff);
        for (i = 0; i < 16; ++i)
            jpeg_save_markers (&injpg, JPEG_APP0 + i, 0xffff);
    }


    /* Read header, and display information about the image.  */
    jpeg_read_header (&injpg, TRUE);

    if (injpg.image_width <= 0 || injpg.image_height <= 0)
    {
        fprintf (stderr, _("%s: %s: Image has zero size\n"), invocation_name, infilename);
        exit (1);
    }

    *image_width = injpg.image_width;
    *image_height = injpg.image_height;

    if (opt_info)
    {
        /* TRANSLATORS: Please use the "multiplication sign" (Unicode character <U00D7>)
           in place of the "x". */
        fprintf (stderr, _("%s: %s: size %ux%u, colorspace "), invocation_name,
                 infilename != 0 ? infilename : _("STDIN"),
                 (unsigned int)injpg.image_width, (unsigned int)injpg.image_height);

        switch (injpg.jpeg_color_space)
        {
          case JCS_GRAYSCALE:
            fputs (_("grayscale"), stderr);
            break;

          case JCS_RGB:
            fputs ("RGB", stderr);
            break;

          case JCS_YCbCr:
            fputs ("YUV", stderr);
            break;

          case JCS_CMYK:
            fputs ("CMYK", stderr);
            break;

          case JCS_YCCK:
            fputs ("YUVK", stderr);
            break;

          default:
            fputs (_("unknown"), stderr);
        }

        fputs (_(", sampling"), stderr);

        {
            int icomp;

            for (icomp = 0; icomp < injpg.num_components; ++icomp)
            {
                const jpeg_component_info *const comp = &(injpg.comp_info [icomp]);

                /* TRANSLATORS: Please use the "multiplication sign" (Unicode character <U00D7>)
                   in place of the "x". */
                fprintf (stderr, _(" %dx%d"), comp->h_samp_factor, comp->v_samp_factor);
            }
        }

        putc ('\n', stderr);
    }


    /* Create output file.  */
    if (outfilename != 0)
    {
        if ((outfile = fopen (outfilename, "wb")) == 0)
        {
            fprintf (stderr, _("%s: %s: %s\n"), invocation_name, outfilename, strerror (errno));
            exit (1);
        }
    }
    else
        outfile = stdout;

    jpeg_stdio_dest (&outjpg, outfile);
}