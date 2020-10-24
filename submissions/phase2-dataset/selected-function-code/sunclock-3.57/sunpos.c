void
sunpos(jd, apparent, ra, dec, rv, slong)
double jd;
int apparent;
double *ra, *dec, *rv, *slong;
{
	double t, t2, t3, l, m, e, ea, v, theta, thetarad, omega, eps;

	/* Time, in Julian centuries of 36525 ephemeris days,
	   measured from the epoch 1900 January 0.5 ET. */

	t = (jd - 2415020.0) / 36525.0;
	t2 = t * t;
	t3 = t2 * t;

	/* Geometric mean longitude of the Sun, referred to the
	   mean equinox of the date. */

	l = fixangle(279.69668 + 36000.76892 * t + 0.0003025 * t2);

        /* Sun's mean anomaly. */

	m = fixangle(358.47583 + 35999.04975*t - 0.000150*t2 - 0.0000033*t3);

        /* Eccentricity of the Earth's orbit. */

	e = 0.01675104 - 0.0000418 * t - 0.000000126 * t2;

	/* Eccentric anomaly. */

	ea = kepler(m, e);

	/* True anomaly */

	v = fixangle(2 * todeg(atan(sqrt((1 + e) / (1 - e))  * tan(ea / 2))));

        /* Sun's true longitude. */

	theta = l + v - m;

	/* Obliquity of the ecliptic. */

	eps = 23.452294 - 0.0130125 * t - 0.00000164 * t2 + 0.000000503 * t3;

        /* Corrections for Sun's apparent longitude, if desired. */

	if (apparent) {
	   omega = fixangle(259.18 - 1934.142 * t);
	   theta = theta - 0.00569 - 0.00479 * sin(torad(omega));
	   eps += 0.00256 * cos(torad(omega));
	}

        /* Return Sun's longitude and radius vector */

	*slong = theta;
	*rv = (1.0000002 * (1 - e * e)) / (1 + e * cos(torad(v)));

	/* Determine solar co-ordinates. */

	epsrad = torad(eps);
	thetarad = torad(theta);
	*ra = atan2((cos(epsrad) * sin(thetarad)), cos(thetarad));
	*ra = fixangle(todeg(*ra));
	*dec = todeg(asin(sin(epsrad)) * sin(thetarad));
}
