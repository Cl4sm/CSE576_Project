static unsigned long
SFscrollTimerInterval()
{
	static int	maxVal = 200;
	static int	varyDist = 50;
	static int	minDist = 50;
	int		t;
	int		dist;

	if (SFcurrentListY < SFlowerY) {
		dist = SFlowerY - SFcurrentListY;
	} else if (SFcurrentListY > SFupperY) {
		dist = SFcurrentListY - SFupperY;
	} else {
		return (unsigned long) 1;
	}

	t = maxVal - ((maxVal / varyDist) * (dist - minDist));

	if (t < 1) {
		t = 1;
	}

	if (t > maxVal) {
		t = maxVal;
	}

	return (unsigned long) t;
}
