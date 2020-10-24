void
interpolate (const struct jpeg_decompress_struct *const jpg, struct jfif *const jfif,
             const struct point_dimdir_s *const point, const enum method_t method)
{
    int icomp;


    /* Iterate over components.  */
    for (icomp = 0; icomp < jpg->num_components; ++icomp)
    {
        const jpeg_component_info *const comp = &(jpg->comp_info [icomp]);

        /* Downsampled position of the pixel block.  */
        const int x_pos = point->p.x * comp->h_samp_factor / jpg->max_h_samp_factor;
        const int y_pos = point->p.y * comp->v_samp_factor / jpg->max_v_samp_factor;

        /* Downsampled size of the pixel block.  */
        const int x_size = ((point->p.x_size + jpg->max_h_samp_factor / comp->h_samp_factor - 1)
                            / (jpg->max_h_samp_factor / comp->h_samp_factor));
        const int y_size = ((point->p.y_size + jpg->max_v_samp_factor / comp->v_samp_factor - 1)
                            / (jpg->max_v_samp_factor / comp->v_samp_factor));


        /* Show pixel block.  */
        if (opt_verbose && icomp == 0)
            fprintf (stderr, _("%s: interpolating %u,%u,%u,%u (%s)\n"), invocation_name,
                     point->p.x, point->p.y, point->p.x_size, point->p.y_size,
                     point->d == vertical ? _("vertical") : point->d == horizontal ? _("horizontal") : _("2-dim"));


        /* Interpolate pixel.  */
        switch (method)
        {
          case average:
            /* Average of adjacent pixels.  */
            switch (point->d)
            {
              case vertical:
                {
                    int x;

                    for (x = 0; x < x_size; ++x)
                    {
                        int sum_weight = 0;
                        double v = 0.0;

                        if (XY_VALID (x_pos + x, y_pos - 1))
                        {
                            v += get_pixel (jfif, icomp, x_pos + x, y_pos - 1);
                            ++sum_weight;
                        }
                        if (XY_VALID (x_pos + x, y_pos + y_size))
                        {
                            v += get_pixel (jfif, icomp, x_pos + x, y_pos + y_size);
                            ++sum_weight;
                        }

                        if (sum_weight > 0)
                        {
                            int y;

                            v /= (double)sum_weight;

                            for (y = 0; y < y_size; ++y)
                                if (XY_VALID (x_pos + x, y_pos + y))
                                    set_pixel (jfif, icomp, x_pos + x, y_pos + y, v);
                        }
                    }
                }
                break;

              case horizontal:
                {
                    int y;

                    for (y = 0; y < y_size; ++y)
                    {
                        int sum_weight = 0;
                        double v = 0.0;

                        if (XY_VALID (x_pos - 1, y_pos + y))
                        {
                            v += get_pixel (jfif, icomp, x_pos - 1, y_pos + y);
                            ++sum_weight;
                        }
                        if (XY_VALID (x_pos + x_size, y_pos + y))
                        {
                            v += get_pixel (jfif, icomp, x_pos + x_size, y_pos + y);
                            ++sum_weight;
                        }

                        if (sum_weight > 0)
                        {
                            int x;

                            v /= (double)sum_weight;

                            for (x = 0; x < x_size; ++x)
                                if (XY_VALID (x_pos + x, y_pos + y))
                                    set_pixel (jfif, icomp, x_pos + x, y_pos + y, v);
                        }
                    }
                }
                break;

              case twodim:
                {
                    int sum_weight = 0;
                    double v = 0.0;
                    int x, y;

                    for (x = 0; x < x_size; ++x)
                    {
                        if (XY_VALID (x_pos + x, y_pos - 1))
                        {
                            v += get_pixel (jfif, icomp, x_pos + x, y_pos - 1);
                            ++sum_weight;
                        }
                        if (XY_VALID (x_pos + x, y_pos + y_size))
                        {
                            v += get_pixel (jfif, icomp, x_pos + x, y_pos + y_size);
                            ++sum_weight;
                        }
                    }

                    for (y = 0; y < y_size; ++y)
                    {
                        if (XY_VALID (x_pos - 1, y_pos + y))
                        {
                            v += get_pixel (jfif, icomp, x_pos - 1, y_pos + y);
                            ++sum_weight;
                        }
                        if (XY_VALID (x_pos + x_size, y_pos + y))
                        {
                            v += get_pixel (jfif, icomp, x_pos + x_size, y_pos + y);
                            ++sum_weight;
                        }
                    }

                    if (sum_weight > 0)
                    {
                        v /= (double)sum_weight;

                        for (x = 0; x < x_size; ++x)
                            for (y = 0; y < y_size; ++y)
                                if (XY_VALID (x_pos + x, y_pos + y))
                                    set_pixel (jfif, icomp, x_pos + x, y_pos + y, v);
                    }
                }
            }
            break;

          case linear:
            /* (Bi)linear interpolation.  */
            set_weighted_pixels (jfif, comp, icomp, x_pos, y_pos, x_size, y_size, point->d, 1);
            break;

          case quadratic:
            /* (Bi)quadratic interpolation.  */
            set_weighted_pixels (jfif, comp, icomp, x_pos, y_pos, x_size, y_size, point->d, 2);
            break;

          case cubic:
            /* (Bi)cubic interpolation.  */
            set_weighted_pixels (jfif, comp, icomp, x_pos, y_pos, x_size, y_size, point->d, 3);
        }
    }
}