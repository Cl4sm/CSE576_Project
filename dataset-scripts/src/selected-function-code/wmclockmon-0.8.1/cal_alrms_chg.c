static int cal_alrms_chg() {
    static time_t  cal_u_mtime = 0;
    static time_t  cal_y_mtime = 0;
    static time_t  cal_m_mtime = 0;
    char          *cal_u_fname = NULL;
    char          *cal_y_fname = NULL;
    char          *cal_m_fname = NULL;
    int            chg_u, chg_y, chg_m;

    if (! calalrms) return False;
    cal_u_fname = get_calend_file(1);
    cal_y_fname = get_calend_file(2);
    cal_m_fname = get_calend_file(3);
    chg_u = filestat(cal_u_fname, &cal_u_mtime, MTIME);
    chg_y = filestat(cal_y_fname, &cal_y_mtime, MTIME);
    chg_m = filestat(cal_m_fname, &cal_m_mtime, MTIME);
    free(cal_u_fname);
    free(cal_y_fname);
    free(cal_m_fname);
    if (chg_u || chg_y || chg_m) return True;
    return False;
}