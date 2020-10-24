char *gendatestr()
{
	time_t t;
	struct tm gmttm, lttm;
	int dayyear;
	char *timestr;
	const char *weekday = NULL, *month = NULL;

	/* 6 + 26 + ' ' + timezone which is 5 + '\n\0' == 40 */
	timestr = (char *)mymalloc(40);
	t = time(NULL);

	localtime_r(&t, &lttm);
	gmtime_r(&t, &gmttm);

	t = (((lttm.tm_hour - gmttm.tm_hour) * 60) +
	    (lttm.tm_min - gmttm.tm_min)) * 60;
	
	dayyear = lttm.tm_yday - gmttm.tm_yday;
	if(dayyear) {
		if (dayyear == -1 || dayyear > 1)
			t -= 24 * 60 * 60;
		else
			t += 24 * 60 * 60;
	}

	switch(lttm.tm_wday) {
		case 0: weekday = "Sun";
			break;
		case 1: weekday = "Mon";
			break;
		case 2: weekday = "Tue";
			break;
		case 3: weekday = "Wed";
			break;
		case 4: weekday = "Thu";
			break;
		case 5: weekday = "Fri";
			break;
		case 6: weekday = "Sat";
			break;
		default:
			break;
	}
	switch(lttm.tm_mon) {
		case 0: month = "Jan";
			break;
		case 1: month = "Feb";
			break;
		case 2: month = "Mar";
			break;
		case 3: month = "Apr";
			break;
		case 4: month = "May";
			break;
		case 5: month = "Jun";
			break;
		case 6: month = "Jul";
			break;
		case 7: month = "Aug";
			break;
		case 8: month = "Sep";
			break;
		case 9: month = "Oct";
			break;
		case 10: month = "Nov";
			 break;
		case 11: month = "Dec";
			 break;
		default:
			 break;
	}

	
	snprintf(timestr, 40, "Date: %s, %02d %s %04d %02d:%02d:%02d %+05d\n",	
			weekday, lttm.tm_mday, month, lttm.tm_year + 1900,
			lttm.tm_hour, lttm.tm_min, lttm.tm_sec, ((int)t)/36);

	return timestr;
}