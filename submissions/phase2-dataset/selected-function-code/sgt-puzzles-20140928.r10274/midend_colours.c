float *midend_colours(midend *me, int *ncolours)
{
    float *ret;

    ret = me->ourgame->colours(me->frontend, ncolours);

    {
        int i;

        /*
         * Allow environment-based overrides for the standard
         * colours by defining variables along the lines of
         * `NET_COLOUR_4=6000c0'.
         */

        for (i = 0; i < *ncolours; i++) {
            char buf[80], *e;
            unsigned int r, g, b;
            int j, k;

            sprintf(buf, "%s_COLOUR_%d", me->ourgame->name, i);
            for (j = k = 0; buf[j]; j++)
		if (!isspace((unsigned char)buf[j]))
		    buf[k++] = toupper((unsigned char)buf[j]);
	    buf[k] = '\0';
            if ((e = getenv(buf)) != NULL &&
                sscanf(e, "%2x%2x%2x", &r, &g, &b) == 3) {
                ret[i*3 + 0] = r / 255.0F;
                ret[i*3 + 1] = g / 255.0F;
                ret[i*3 + 2] = b / 255.0F;
            }
        }
    }

    return ret;
}