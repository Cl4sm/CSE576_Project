void
initForbidden(void)
{
    redirectorKill();

    if(forbiddenFile)
        forbiddenFile = expandTilde(forbiddenFile);

    if(forbiddenFile == NULL) {
        forbiddenFile = expandTilde(internAtom("~/.polipo-forbidden"));
        if(forbiddenFile) {
            if(access(forbiddenFile->string, F_OK) < 0) {
                releaseAtom(forbiddenFile);
                forbiddenFile = NULL;
            }
        }
    }

    if(forbiddenFile == NULL) {
        if(access("/etc/polipo/forbidden", F_OK) >= 0)
            forbiddenFile = internAtom("/etc/polipo/forbidden");
    }

    parseDomainFile(forbiddenFile, &forbiddenDomains, &forbiddenRegex);


    if(uncachableFile)
        uncachableFile = expandTilde(uncachableFile);

    if(uncachableFile == NULL) {
        uncachableFile = expandTilde(internAtom("~/.polipo-uncachable"));
        if(uncachableFile) {
            if(access(uncachableFile->string, F_OK) < 0) {
                releaseAtom(uncachableFile);
                uncachableFile = NULL;
            }
        }
    }

    if(uncachableFile == NULL) {
        if(access("/etc/polipo/uncachable", F_OK) >= 0)
            uncachableFile = internAtom("/etc/polipo/uncachable");
    }

    parseDomainFile(uncachableFile, &uncachableDomains, &uncachableRegex);

    if(forbiddenTunnelsFile)
        forbiddenTunnelsFile = expandTilde(forbiddenTunnelsFile);
    
    if(forbiddenTunnelsFile == NULL) {
        forbiddenTunnelsFile = expandTilde(internAtom("~/.polipo-forbiddenTunnels"));
        if(forbiddenTunnelsFile) {
            if(access(forbiddenTunnelsFile->string, F_OK) < 0) {
                releaseAtom(forbiddenTunnelsFile);
                forbiddenTunnelsFile = NULL;
            }
        }
    }
    
    if(forbiddenTunnelsFile == NULL) {
        if(access("/etc/polipo/forbiddenTunnels", F_OK) >= 0)
            forbiddenTunnelsFile = internAtom("/etc/polipo/forbiddenTunnels");
    }
    
    parseDomainFile(forbiddenTunnelsFile, &forbiddenTunnelsDomains, &forbiddenTunnelsRegex);
    //
    
    return;
}