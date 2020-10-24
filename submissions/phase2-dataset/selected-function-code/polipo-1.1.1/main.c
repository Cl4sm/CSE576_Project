int
main(int argc, char **argv)
{
    FdEventHandlerPtr listener;
    int i;
    int rc;
    int expire = 0, printConfig = 0;

    initAtoms();
    CONFIG_VARIABLE(daemonise, CONFIG_BOOLEAN, "Run as a daemon");
    CONFIG_VARIABLE(pidFile, CONFIG_ATOM, "File with pid of running daemon.");

    preinitChunks();
    preinitLog();
    preinitObject();
    preinitIo();
    preinitDns();
    preinitServer();
    preinitHttp();
    preinitDiskcache();
    preinitLocal();
    preinitForbidden();
    preinitSocks();

    i = 1;
    while(i < argc) {
        if(argv[i][0] != '-')
            break;
        if(strcmp(argv[i], "--") == 0) {
            i++;
            break;
        } else if(strcmp(argv[i], "-h") == 0) {
            usage(argv[0]);
            exit(0);
        } else if(strcmp(argv[i], "-v") == 0) {
            printConfig = 1;
            i++;
        } else if(strcmp(argv[i], "-x") == 0) {
            expire = 1;
            i++;
        } else if(strcmp(argv[i], "-c") == 0) {
            i++;
            if(i >= argc) {
                usage(argv[0]);
                exit(1);
            }
            if(configFile)
                releaseAtom(configFile);
            configFile = internAtom(argv[i]);
            i++;
        } else {
            usage(argv[0]);
            exit(1);
        }
    }

    if(configFile)
        configFile = expandTilde(configFile);

    if(configFile == NULL) {
        configFile = expandTilde(internAtom("~/.polipo"));
        if(configFile)
            if(access(configFile->string, F_OK) < 0) {
                releaseAtom(configFile);
                configFile = NULL;
            }
    }

    if(configFile == NULL) {
        if(access("/etc/polipo/config", F_OK) >= 0)
            configFile = internAtom("/etc/polipo/config");
        if(configFile && access(configFile->string, F_OK) < 0) {
            releaseAtom(configFile);
            configFile = NULL;
        }
    }

    rc = parseConfigFile(configFile);
    if(rc < 0)
        exit(1);

    while(i < argc) {
        rc = parseConfigLine(argv[i], "command line", 0, 0);
        if(rc < 0)
            exit(1);
        i++;
    }

    initChunks();
    initLog();
    initObject();
    if(!expire && !printConfig)
        initEvents();
    initIo();
    initDns();
    initHttp();
    initServer();
    initDiskcache();
    initForbidden();
    initSocks();

    if(printConfig) {
        printConfigVariables(stdout, 0);
        exit(0);
    }

    if(expire) {
        expireDiskObjects();
        exit(0);
    }

    if(daemonise)
        do_daemonise(loggingToStderr());

    if(pidFile)
        writePid(pidFile->string);

    listener = create_listener(proxyAddress->string, 
                               proxyPort, httpAccept, NULL);
    if(!listener) {
        if(pidFile) unlink(pidFile->string);
        exit(1);
    }

    eventLoop();

    if(pidFile) unlink(pidFile->string);
    return 0;
}