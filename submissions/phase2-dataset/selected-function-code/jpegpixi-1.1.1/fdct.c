void
fdct (JCOEF *freqs, const double *values, const UINT16 *quant)
{
    int i;
    double tmp [DCTSIZE2], *ptr;


    ptr = tmp;
    for (i = 0; i < DCTSIZE; ++i)
    {
        double z2, z3, z4, z5, z11, z13;

        const double tmp0 = (values [0] + values [7]) / 64.0;
        const double tmp7 = (values [0] - values [7]) / 64.0;
        const double tmp1 = (values [1] + values [6]) / 64.0;
        const double tmp6 = (values [1] - values [6]) / 64.0;
        const double tmp2 = (values [2] + values [5]) / 64.0;
        const double tmp5 = (values [2] - values [5]) / 64.0;
        const double tmp3 = (values [3] + values [4]) / 64.0;
        const double tmp4 = (values [3] - values [4]) / 64.0;

        double tmp10 = tmp0 + tmp3;
        double tmp13 = tmp0 - tmp3;
        double tmp11 = tmp1 + tmp2;
        double tmp12 = tmp1 - tmp2;

        const double z1 = (tmp12 + tmp13) * 0.7071067811865475244;

        ptr [0] = tmp10 + tmp11;
        ptr [4] = tmp10 - tmp11;
        ptr [2] = tmp13 + z1;
        ptr [6] = tmp13 - z1;

        tmp10 = tmp4 + tmp5;
        tmp11 = tmp5 + tmp6;
        tmp12 = tmp6 + tmp7;

        z5 = (tmp10 - tmp12) * 0.3826834323650897717;
        z2 = 0.5411961001461969844 * tmp10 + z5;
        z4 = 1.3065629648763765279 * tmp12 + z5;
        z3 = 0.7071067811865475244 * tmp11;
        z11 = tmp7 + z3;
        z13 = tmp7 - z3;

        ptr [5] = z13 + z2;
        ptr [3] = z13 - z2;
        ptr [1] = z11 + z4;
        ptr [7] = z11 - z4;

        values += DCTSIZE;
        ptr += DCTSIZE;
    }


    ptr = tmp;
    for (i = 0; i < DCTSIZE; ++i)
    {
        double z2, z3, z4, z5, z11, z13;

        const double tmp0 = ptr [DCTSIZE * 0] + ptr [DCTSIZE * 7];
        const double tmp7 = ptr [DCTSIZE * 0] - ptr [DCTSIZE * 7];
        const double tmp1 = ptr [DCTSIZE * 1] + ptr [DCTSIZE * 6];
        const double tmp6 = ptr [DCTSIZE * 1] - ptr [DCTSIZE * 6];
        const double tmp2 = ptr [DCTSIZE * 2] + ptr [DCTSIZE * 5];
        const double tmp5 = ptr [DCTSIZE * 2] - ptr [DCTSIZE * 5];
        const double tmp3 = ptr [DCTSIZE * 3] + ptr [DCTSIZE * 4];
        const double tmp4 = ptr [DCTSIZE * 3] - ptr [DCTSIZE * 4];

        double tmp10 = tmp0 + tmp3;
        double tmp13 = tmp0 - tmp3;
        double tmp11 = tmp1 + tmp2;
        double tmp12 = tmp1 - tmp2;

        const double z1 = (tmp12 + tmp13) * 0.7071067811865475244;

        freqs [DCTSIZE * 0] = coeff ((tmp10 + tmp11) / (scale [i]), quant [DCTSIZE * 0]);
        freqs [DCTSIZE * 4] = coeff ((tmp10 - tmp11) / (scale [i]), quant [DCTSIZE * 4]);
        freqs [DCTSIZE * 2] = coeff ((tmp13 + z1) / (scale [i] * scale [2]), quant [DCTSIZE * 2]);
        freqs [DCTSIZE * 6] = coeff ((tmp13 - z1) / (scale [i] * scale [6]), quant [DCTSIZE * 6]);

        tmp10 = tmp4 + tmp5;
        tmp11 = tmp5 + tmp6;
        tmp12 = tmp6 + tmp7;

        z5 = (tmp10 - tmp12) * 0.3826834323650897717;
        z2 = 0.5411961001461969844 * tmp10 + z5;
        z4 = 1.3065629648763765279 * tmp12 + z5;
        z3 = 0.7071067811865475244 * tmp11;
        z11 = tmp7 + z3;
        z13 = tmp7 - z3;

        freqs [DCTSIZE * 5] = coeff ((z13 + z2) / (scale [i] * scale [5]), quant [DCTSIZE * 5]);
        freqs [DCTSIZE * 3] = coeff ((z13 - z2) / (scale [i] * scale [3]), quant [DCTSIZE * 3]);
        freqs [DCTSIZE * 1] = coeff ((z11 + z4) / (scale [i] * scale [1]), quant [DCTSIZE * 1]);
        freqs [DCTSIZE * 7] = coeff ((z11 - z4) / (scale [i] * scale [7]), quant [DCTSIZE * 7]);

        ++ptr;
        ++freqs;
        ++quant;
    }
}