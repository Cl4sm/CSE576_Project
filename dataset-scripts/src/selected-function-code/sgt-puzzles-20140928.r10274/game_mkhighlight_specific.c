void game_mkhighlight_specific(frontend *fe, float *ret,
			       int background, int highlight, int lowlight)
{
    float max;
    int i;

    /*
     * Drop the background colour so that the highlight is
     * noticeably brighter than it while still being under 1.
     */
    max = ret[background*3];
    for (i = 1; i < 3; i++)
        if (ret[background*3+i] > max)
            max = ret[background*3+i];
    if (max * 1.2F > 1.0F) {
        for (i = 0; i < 3; i++)
            ret[background*3+i] /= (max * 1.2F);
    }

    for (i = 0; i < 3; i++) {
	if (highlight >= 0)
	    ret[highlight * 3 + i] = ret[background * 3 + i] * 1.2F;
	if (lowlight >= 0)
	    ret[lowlight * 3 + i] = ret[background * 3 + i] * 0.8F;
    }
}