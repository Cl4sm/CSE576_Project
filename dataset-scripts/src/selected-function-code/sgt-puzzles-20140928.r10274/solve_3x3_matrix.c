static int solve_3x3_matrix(double mx[9], double vin[3], double vout[3])
{
    double inv[9];
    double det;

    det = (mx[0]*mx[4]*mx[8] + mx[1]*mx[5]*mx[6] + mx[2]*mx[3]*mx[7] -
           mx[0]*mx[5]*mx[7] - mx[1]*mx[3]*mx[8] - mx[2]*mx[4]*mx[6]);
    if (det == 0)
        return FALSE;

    inv[0] = (mx[4]*mx[8] - mx[5]*mx[7]) / det;
    inv[1] = (mx[2]*mx[7] - mx[1]*mx[8]) / det;
    inv[2] = (mx[1]*mx[5] - mx[2]*mx[4]) / det;
    inv[3] = (mx[5]*mx[6] - mx[3]*mx[8]) / det;
    inv[4] = (mx[0]*mx[8] - mx[2]*mx[6]) / det;
    inv[5] = (mx[2]*mx[3] - mx[0]*mx[5]) / det;
    inv[6] = (mx[3]*mx[7] - mx[4]*mx[6]) / det;
    inv[7] = (mx[1]*mx[6] - mx[0]*mx[7]) / det;
    inv[8] = (mx[0]*mx[4] - mx[1]*mx[3]) / det;

    vout[0] = inv[0]*vin[0] + inv[1]*vin[1] + inv[2]*vin[2];
    vout[1] = inv[3]*vin[0] + inv[4]*vin[1] + inv[5]*vin[2];
    vout[2] = inv[6]*vin[0] + inv[7]*vin[1] + inv[8]*vin[2];

    return TRUE;
}