DiskObjectPtr
processObject(DiskObjectPtr dobjects, char *filename, struct stat *sb)
{
    DiskObjectPtr dobject = NULL;
    int c = 0;

    dobject = readDiskObject((char*)filename, sb);
    if(dobject == NULL)
        return dobjects;

    if(!dobjects ||
       (c = strcmp(dobject->location, dobjects->location)) <= 0) {
        if(dobjects && c == 0) {
            mergeDobjects(dobjects, dobject);
        } else {
            dobject->next = dobjects;
            dobjects = dobject;
        }
    } else {
        DiskObjectPtr other = dobjects;
        while(other->next) {
            c = strcmp(dobject->location, other->next->location);
            if(c < 0)
                break;
            other = other->next;
        }
        if(strcmp(dobject->location, other->location) == 0) {
            mergeDobjects(other, dobject);
        } else {
            dobject->next = other->next;
            other->next = dobject;
        }
    }
    return dobjects;
}