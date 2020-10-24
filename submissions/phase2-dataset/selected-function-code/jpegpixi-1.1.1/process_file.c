void
process_file (const struct rbtree *const points, const enum method_t method)
{
    jvirt_barray_ptr *coeff;


    /* Copy DCT coefficients from input to output file.  */
    coeff = jpeg_read_coefficients (&injpg);
    jpeg_copy_critical_parameters (&injpg, &outjpg);
    outjpg.optimize_coding = TRUE;
    jpeg_write_coefficients (&outjpg, coeff);


    /* Copy saved markers from input to output file, unless file is to be stripped.  */
    if (!opt_strip)
    {
        jpeg_saved_marker_ptr marker;

        for (marker = injpg.marker_list; marker != 0; marker = marker->next)
        {
            /* JFIF and Adobe markers are taken care of by the library, so don't write duplicates.  */
            if ((!outjpg.write_JFIF_header || marker->marker != JPEG_APP0 || marker->data_length < 5
                 || GETJOCTET (marker->data [0]) != 0x4a || GETJOCTET (marker->data [1]) != 0x46
                 || GETJOCTET (marker->data [2]) != 0x49 || GETJOCTET (marker->data [3]) != 0x46
                 || GETJOCTET (marker->data [4]) != 0)
                && (!outjpg.write_Adobe_marker || marker->marker != JPEG_APP0 + 14 || marker->data_length < 5
                    || GETJOCTET (marker->data [0]) != 0x41 || GETJOCTET (marker->data [1]) != 0x64
                    || GETJOCTET (marker->data [2]) != 0x6f || GETJOCTET (marker->data [3]) != 0x62
                    || GETJOCTET (marker->data [4]) != 0x65))
                jpeg_write_marker (&outjpg, marker->marker, marker->data, marker->data_length);
        }
    }


    /* Interpolate pixels.  */
    {
        struct point_dimdir_s *point = rbtree_first (points);

        if (point != 0)
        {
            struct jfif *const jfif = jfif_init (&injpg, coeff);

            while (point != 0)
            {
                if (point->p.x + point->p.x_size <= injpg.image_width
                    && point->p.y + point->p.y_size <= injpg.image_height
                    && point->p.x_size <= injpg.image_width
                    && point->p.y_size <= injpg.image_height)
                    interpolate (&injpg, jfif, point, method);
                else
                    /* TRANSLATORS: Please use the "multiplication sign" (Unicode character <U00D7>)
                       in place of the "x". */
                    fprintf (stderr, _("%s: %s: %u,%u,%u,%u ignored: Image size %ux%u\n"),
                             invocation_name, infilename1 != 0 ? infilename1 : _("STDIN"),
                             point->p.x, point->p.y, point->p.x_size, point->p.y_size,
                             (unsigned int)injpg.image_width, (unsigned int)injpg.image_height);

                point = rbtree_next (point);
            }

            jfif_fini (jfif);
        }
    }


    /* Finish processing and close files.  */
    jpeg_finish_compress (&outjpg);
    jpeg_finish_decompress (&injpg);

    if (outfilename1 != 0 && fclose (outfile) == EOF)
    {
        fprintf (stderr, _("%s: %s: %s\n"), invocation_name, outfilename1, strerror (errno));
        exit (1);
    }

    if (infilename1 != 0 && fclose (infile) == EOF)
    {
        fprintf (stderr, _("%s: %s: %s\n"), invocation_name, infilename1, strerror (errno));
        exit (1);
    }


    /* Delete jpeglib objects.  */
    jpeg_destroy_decompress (&injpg);
    jpeg_destroy_compress (&outjpg);
}