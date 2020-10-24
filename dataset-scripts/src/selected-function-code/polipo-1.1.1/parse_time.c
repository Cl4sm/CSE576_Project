int
parse_time(const char *buf, int offset, int len, time_t *time_return)
{
    struct tm tm;
    time_t t;
    int i = offset;

    i = skip_word(buf, i, len); if(i < 0) return -1;
    i = skip_separator(buf, i, len); if(i < 0) return -1;

    if(i >= len)
        return -1;

    if(d2i(buf[i]) >= 0) {
        i = parse_int(buf, i, len, &tm.tm_mday); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_month(buf, i, len, &tm.tm_mon); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_year); if(i < 0) return -1;
        if(tm.tm_year < 100)
            tm.tm_year += 1900;
        if(tm.tm_year < 1937)
            tm.tm_year += 100;
        if(tm.tm_year < 1937)
            return -1;

        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_hour); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_min); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_sec); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = skip_word(buf, i, len); if(i < 0) return -1;
    } else {                    /* funny American format */
        i = parse_month(buf, i, len, &tm.tm_mon); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_mday); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_hour); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_min); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_sec); if(i < 0) return -1;
        i = skip_separator(buf, i, len); if(i < 0) return -1;
        i = parse_int(buf, i, len, &tm.tm_year); if(i < 0) return -1;
        if(tm.tm_year < 100)
            tm.tm_year += 1900;
        if(tm.tm_year < 1937)
            tm.tm_year += 100;
        if(tm.tm_year < 1937 || tm.tm_year > 2040)
            return -1;
    }

    if(tm.tm_year < 2038) {
        tm.tm_year -= 1900;
        tm.tm_isdst = -1;
        t = mktime_gmt(&tm);
        if(t == -1)
            return -1;
    } else {
        t = time_t_max;
    }

    *time_return = t;
    return i;
}