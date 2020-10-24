double
gmst(jd)
double jd;
{
	double t, theta0;


	/* Time, in Julian centuries of 36525 ephemeris days,
	   measured from the epoch 1900 January 0.5 ET. */

	t = ((floor(jd + 0.5) - 0.5) - 2415020.0) / 36525.0;

	theta0 = 6.6460656 + 2400.051262 * t + 0.00002581 * t * t;

	t = (jd + 0.5) - (floor(jd + 0.5));

	theta0 += (t * 24.0) * 1.002737908;

	theta0 = (theta0 - 24.0 * (floor(theta0 / 24.0)));

	return theta0;
}
