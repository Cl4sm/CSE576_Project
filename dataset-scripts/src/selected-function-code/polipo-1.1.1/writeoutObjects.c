void
writeoutObjects(int all)
{
    ObjectPtr object = object_list;
    int bytes;
    int objects;
    int n;

    if(diskIsClean) return;

    objects = 0;
    bytes = 0;
    while(object) {
        do {
            if(!all) {
                if(objects >= maxObjectsWhenIdle || 
                   bytes >= maxWriteoutWhenIdle) {
                    if(workToDo()) return;
                    objects = 0;
                    bytes = 0;
                }
            }
            n = writeoutToDisk(object, -1, all ? -1 : maxWriteoutWhenIdle);
            bytes += n;
        } while(!all && n == maxWriteoutWhenIdle);
        objects++;
        object = object->next;
    }
    diskIsClean = 1;
}