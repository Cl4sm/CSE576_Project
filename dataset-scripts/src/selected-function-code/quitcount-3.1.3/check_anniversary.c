static gboolean check_anniversary(gpointer data)
{
	time_t now = time(NULL);
	int num_day = (now - stop_date)/86400;
	int num_cig = num_day * num_per_day;
	float price, life;
	gboolean anniversary = FALSE;
	struct tm *tmtime;
	int now_day, quit_day, now_month, quit_month, now_year, quit_year;
	gchar *str = NULL;

	if (num_cig == 0)
		return TRUE;

	tmtime = gmtime(&stop_date);
	quit_day = tmtime->tm_mday;
	quit_month = tmtime->tm_mon;
	quit_year = tmtime->tm_year;
	tmtime = gmtime(&now);
	now_day = tmtime->tm_mday;
	now_month = tmtime->tm_mon;
	now_year = tmtime->tm_year;

	life = num_cig*10.0;
	price = unit_price*num_cig;
	
	if ((int)life < 7*60*24 && ((int)life) % (60*24) == 0) {
		str = g_strdup_printf(ngettext("Life expectancy gain: %d day!","Life expectancy gain: %d days!",(int)life/(60*24)), (int)life/(60*24));
		anniversary = TRUE;
	} else if ((int)life < 30*60*24 && ((int)life) % (7*60*24) == 0) {
		str = g_strdup_printf(ngettext("Life expectancy gain: %d week!","Life expectancy gain: %d weeks!",(int)life/(60*24*7)), (int)life/(60*24*7));
		anniversary = TRUE;
	} else if ((int)life < 365*60*24 && ((int)life) % (30*60*24) == 0) {
		str = g_strdup_printf(ngettext("Life expectancy gain: %d month!","Life expectancy gain: %d months!",(int)life/(60*24*30)), (int)life/(60*24*30));
		anniversary = TRUE;
	} else if ((int)life >= 365*60*24 && ((int)life) % (365*60*24) == 0) {
		str = g_strdup_printf(ngettext("Life expectancy gain: %d year!","Life expectancy gain: %d years!",(int)life/(365*60*24)), (int)life/(365*60*24));
		anniversary = TRUE;
	} 

	if ((int)price < 100 && ((int)price) % 10 == 0) {
		str = g_strdup_printf(_("%d %s saved!"),(int)price, currency);
		anniversary = TRUE;
	} else if ((int)price < 1000 && ((int)price) % 100 == 0) {
		str = g_strdup_printf(_("%d %s saved!"),(int)price, currency);
		anniversary = TRUE;
	} else if ((int)price < 10000 && ((int)price) % 1000 == 0) {
		str = g_strdup_printf(_("%d %s saved!"),(int)price, currency);
		anniversary = TRUE;
	} else if ((int)price < 100000 && ((int)price) % 10000 == 0) {
		str = g_strdup_printf(_("%d %s saved!"),(int)price, currency);
		anniversary = TRUE;
	}
	if (num_day < 31 && (num_day % 7) == 0) {
		str = g_strdup_printf(ngettext("You quit since %d week!", "You quit since %d weeks!",(int)num_day/7), (int)num_day/7);
		anniversary = TRUE;
	} else if (num_day < 363 && num_day > 1 && now_day == quit_day) {
		int nmon = 0;
		if (quit_month > now_month)
			now_month += 12;
		nmon = now_month-quit_month;
		str = g_strdup_printf(ngettext("You quit since %d month!", "You quit since %d months!", nmon), nmon);
		anniversary = TRUE;
	} else if (num_day >= 363 && now_day == quit_day && now_month == quit_month) {
		int nyear = 0;
		nyear = now_year-quit_year;
		str = g_strdup_printf(ngettext("You quit since %d year!", "You quit since %d years!",nyear), nyear);
		anniversary = TRUE;
	} else if (num_day <=500 && (num_day % 100) == 0) {
		str = g_strdup_printf(_("You quit since %d days!"), num_day);
		anniversary = TRUE;
	} else if (num_day >= 1000 && (num_day % 1000) == 0) {
		str = g_strdup_printf(_("You quit since %d days!"), num_day);
		anniversary = TRUE;
	}
	if (str)
		gtk_status_icon_set_tooltip_text(icon, str);
	else
		gtk_status_icon_set_has_tooltip(icon, FALSE);
	g_free(str);

	set_icon(anniversary);

	return TRUE;
}