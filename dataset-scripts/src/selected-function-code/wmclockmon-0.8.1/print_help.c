static void print_help(char *prog)
{
    printf("Usage : %s [OPTIONS]\n"
           "%s - Window Maker digital clock dockapp\n"
           " -h,  --help                  show this help text and exit\n"
           " -v,  --version               show program version and exit\n"
           " -d,  --display <string>      display to use\n"
           " -bl, --backlight             turn on back-light\n"
           " -lc, --light-color <color>   backlight/led colour\n"
           " -tm, --time-mode <mode>      start with time mode (0: clock, 1: internet time, 2: binary clock)\n"
           " -w,  --windowed              run the application in windowed mode\n"
           " -bw, --broken-wm             activate broken window manager fix\n"
           " -a,  --alarm <HH:MM>         set alarm time to HH:MM (24h clock mode)\n"
           " -c,  --alarm-cmd <string>    command to launch when alarm raises\n"
           " -mc, --message-cmd <string>  command to launch when alarm raises and a message is associated\n"
           " -12, --h12                   12 hours clock mode (default is 24)\n"
           " -s,  --style <file>          style to use for display\n"
           " -sd, --style-dir <dir>       directory where styles are stored\n"
           " -nb, --no-blink              disable blinking when alarm is raised\n"
           " -f,  --cfgfile <filename>    use 'filename' as configuration file\n"
           " -nl, --no-locale             don't use current locale\n"
           " -l,  --label <string>        use a label instead of date\n"
           " -ca, --cal-alrm              load calendar alarms for today\n"
           " -sc, --show-cal              show calendar at startup/00:00\n",
           prog, prog);
}