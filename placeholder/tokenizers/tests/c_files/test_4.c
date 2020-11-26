int main(int argc, char **argv)
{
    char *home = getenv("HOME");
    int ret = 0;

    setlocale(LC_ALL, "");

    nc = &my_nc;

    parse_args(argc, argv);

    safeset(nc->config_dir, concat(home, "/.nrss/"));
    safeset(nc->feed_dir, concat(nc->config_dir, "feeds/"));
    safeset(nc->config_file, concat(nc->config_dir, "config"));
    safeset(nc->log_file, concat(nc->config_dir, "log"));
    nc->cache_file = concat(nc->config_dir, ".cache");

    ret = unlink(nc->cache_file);
    if ((ret) && (errno != ENOENT)) {
        cleanup_paths();
        printf("Unlink failed: %s\n", strerror(errno));
        exit(-1);
    }

    ret = unlink(nc->log_file);
    if ((ret) && (errno != ENOENT)) {
        cleanup_paths();
        printf("Unlink failed: %s\n", strerror(errno));
        exit(-1);
    }

    ret = mkdir(nc->config_dir, 0755);
    if ((ret != 0) && (errno != EEXIST)) {
        logit("Couldn't create config dir: %s\n", strerror(errno));
        cleanup_paths();
        exit(-1);
    }

    ret = mkdir(nc->feed_dir, 0755);
    if ((ret != 0) && (errno != EEXIST)) {
        logit("Couldn't create feed dir: %s\n", strerror(errno));
        cleanup_paths();
        exit(-1);
    }

    logit("NRSS v%s\n", VERSION);

    cnf_parse(nc->config_file, &cfg, &cfg_list);

    signal(SIGALRM, alrm);
    signal(SIGWINCH, wnch);
    signal(SIGINT, cleanup);
    signal(SIGCHLD, chld);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGPIPE, pype);
    alarm(60);

    interface(nc->defs[DEF_COLS]);
    cleanup();

    return 0;
}
