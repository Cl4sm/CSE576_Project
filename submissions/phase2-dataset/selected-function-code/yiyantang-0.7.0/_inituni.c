void _inituni ()
{
    if (!init_stat[GBUNI])
    {
	gb2uni_init ();
	init_stat[GBHZ] = 1;
    }
    if (!init_stat[BIG5UNI])
    {
	big2uni_init ();
	init_stat[BIG5UNI] = 1;
    }
    if (!init_stat[UTF7UNI])
    {
	utf7_uni_init ();
	init_stat[UTF7UNI] = 1;
    }
    if (!init_stat[UTF8UNI])
    {
	utf8_uni_init ();
	init_stat[UTF8UNI] = 1;
    }
}
