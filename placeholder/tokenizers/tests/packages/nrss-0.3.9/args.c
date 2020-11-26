/* NRSS - ncurses RSS reader
   Copyright (C) 2007 Jack Miller <jjm2n4@umr.edu>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2 as 
   published by the Free Software Foundation.
*/
#include "nrss.h"

void print_version()
{
    printf("NRSS version %s\n", VERSION);
}

void print_help()
{
    print_version();
    printf("For configuration + key bindings, read \"man nrss\"\n\n");
    printf("Usage: nrss [options]\n\n");
    printf("Command line options:\n");
    printf("\t-h\tThis help\n");
    printf("\t-v\tVersion\n");
    printf("\t-c 0-9\tNumber of columns\n");
    printf("\t-o\tOffline\n");
    printf("\nFile path options:\n");
    printf("\t-D <path>\tPath to config directory (~/.nrss)\n");
    printf("\t-C <path>\tPath to config file (~/.nrss/config)\n");
    printf("\t-L <path>\tPath to log file (~/.nrss/log)\n");
    printf("\t-F <path>\tPath to feed directory (~/.nrss/feeds/)\n");
    printf("\nReport bugs to <jjm2n4@umr.edu>\n");
}

void parse_args(int argc, char **argv)
{
    int c = 0;

    while ((c = getopt(argc, argv, "hovc:D:C:L:F:")) != -1) {
        switch (c) {
        case 'h':
            print_help();
            exit(0);
            break;
        case 'v':
            print_version();
            exit(0);
            break;
        case 'c':
            if ((nc->defs[DEF_COLS] = atoi(optarg)) <= 0)
                nc->defs[DEF_COLS] = 1;
            nc->bits |= CFG_SETCOLS;
            break;
        case 'o':
            nc->bits |= CFG_OFFLINE;
            break;
        case 'D':
            nc->config_dir = xstrdup(optarg);
            break;
        case 'C':
            nc->config_file = xstrdup(optarg);
            break;
        case 'L':
            nc->log_file = xstrdup(optarg);
            break;
        case 'F':
            nc->feed_dir = optarg;
            break;
        }
    }
}
