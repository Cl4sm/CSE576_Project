void
idct (double *values, const JCOEF *freqs, const UINT16 *quant)
{
    int i;
    double tmp [DCTSIZE2], *ptr;


    ptr = tmp;
    for (i = 0; i < DCTSIZE; ++i)
    {
        if (freqs [DCTSIZE * 1] == 0 && freqs [DCTSIZE * 2] && freqs [DCTSIZE * 3] && freqs [DCTSIZE * 4]
            && freqs [DCTSIZE * 5] && freqs [DCTSIZE * 6] && freqs [DCTSIZE * 6])
        {
            const double dc_value = (double)freqs [DCTSIZE * 0] * (double)quant [DCTSIZE * 0] * scale [i];

            ptr [DCTSIZE * 0] = dc_value;
            ptr [DCTSIZE * 1] = dc_value;
            ptr [DCTSIZE * 2] = dc_value;
            ptr [DCTSIZE * 3] = dc_value;
            ptr [DCTSIZE * 4] = dc_value;
            ptr [DCTSIZE * 5] = dc_value;
            ptr [DCTSIZE * 6] = dc_value;
            ptr [DCTSIZE * 7] = dc_value;
        }
        else
        {
            double tmp0 = (double)freqs [DCTSIZE * 0] * (double)quant [DCTSIZE * 0] * scale [i];
            double tmp4 = (double)freqs [DCTSIZE * 1] * (double)quant [DCTSIZE * 1] * scale [i] * scale [1];
            double tmp1 = (double)freqs [DCTSIZE * 2] * (double)quant [DCTSIZE * 2] * scale [i] * scale [2];
            double tmp5 = (double)freqs [DCTSIZE * 3] * (double)quant [DCTSIZE * 3] * scale [i] * scale [3];
            double tmp2 = (double)freqs [DCTSIZE * 4] * (double)quant [DCTSIZE * 4] * scale [i];
            double tmp6 = (double)freqs [DCTSIZE * 5] * (double)quant [DCTSIZE * 5] * scale [i] * scale [5];
            double tmp3 = (double)freqs [DCTSIZE * 6] * (double)quant [DCTSIZE * 6] * scale [i] * scale [6];
            double tmp7 = (double)freqs [DCTSIZE * 7] * (double)quant [DCTSIZE * 7] * scale [i] * scale [7];

            double tmp10 = tmp0 + tmp2;
            double tmp11 = tmp0 - tmp2;
            double tmp13 = tmp1 + tmp3;
            double tmp12 = (tmp1 - tmp3) * 1.4142135623730950488 - tmp13;

            const double z13 = tmp6 + tmp5;
            const double z10 = tmp6 - tmp5;
            const double z11 = tmp4 + tmp7;
            const double z12 = tmp4 - tmp7;
            const double z5 = (z10 + z12) * 1.8477590650225735123;

            tmp0 = tmp10 + tmp13;
            tmp3 = tmp10 - tmp13;
            tmp1 = tmp11 + tmp12;
            tmp2 = tmp11 - tmp12;

            tmp7 = z11 + z13;
            tmp11 = (z11 - z13) * 1.4142135623730950488;
            tmp10 =  1.0823922002923939688 * z12 - z5;
            tmp12 = -2.6131259297527530557 * z10 + z5;

            tmp6 = tmp12 - tmp7;
            tmp5 = tmp11 - tmp6;
            tmp4 = tmp10 + tmp5;

            ptr [DCTSIZE * 0] = tmp0 + tmp7;
            ptr [DCTSIZE * 1] = tmp1 + tmp6;
            ptr [DCTSIZE * 2] = tmp2 + tmp5;
            ptr [DCTSIZE * 3] = tmp3 - tmp4;
            ptr [DCTSIZE * 4] = tmp3 + tmp4;
            ptr [DCTSIZE * 5] = tmp2 - tmp5;
            ptr [DCTSIZE * 6] = tmp1 - tmp6;
            ptr [DCTSIZE * 7] = tmp0 - tmp7;
        }

        ++freqs;
        ++ptr;
        ++quant;
    }


    ptr = tmp;
    for (i = 0; i < DCTSIZE; ++i)
    {
        double tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

        double tmp10 = ptr [0] + ptr [4];
        double tmp11 = ptr [0] - ptr [4];
        double tmp13 = ptr [2] + ptr [6];
        double tmp12 = (ptr [2] - ptr [6]) * 1.4142135623730950488 - tmp13;

        const double z13 = ptr [5] + ptr [3];
        const double z10 = ptr [5] - ptr [3];
        const double z11 = ptr [1] + ptr [7];
        const double z12 = ptr [1] - ptr [7];
        const double z5 = (z10 + z12) * 1.8477590650225735123;

        tmp0 = tmp10 + tmp13;
        tmp3 = tmp10 - tmp13;
        tmp1 = tmp11 + tmp12;
        tmp2 = tmp11 - tmp12;

        tmp7 = z11 + z13;
        tmp11 = (z11 - z13) * 1.4142135623730950488;
        tmp10 =  1.0823922002923939688 * z12 - z5;
        tmp12 = -2.6131259297527530557 * z10 + z5;

        tmp6 = tmp12 - tmp7;
        tmp5 = tmp11 - tmp6;
        tmp4 = tmp10 + tmp5;

        values [0] = (tmp0 + tmp7);
        values [1] = (tmp1 + tmp6);
        values [2] = (tmp2 + tmp5);
        values [3] = (tmp3 - tmp4);
        values [4] = (tmp3 + tmp4);
        values [5] = (tmp2 - tmp5);
        values [6] = (tmp1 - tmp6);
        values [7] = (tmp0 - tmp7);

        ptr += DCTSIZE;
        values += DCTSIZE;
    }
}