ObjectPtr
findObject(int type, const void *key, int key_size)
{
    int h;
    ObjectPtr object;

    if(key_size >= 50000)
        return NULL;

    h = hash(type, key, key_size, log2ObjectHashTableSize);
    object = objectHashTable[h];
    if(!object)
        return NULL;
    if(object->type != type || object->key_size != key_size ||
       memcmp(object->key, key, key_size) != 0) {
        return NULL;
    }
    if(object->next)
        object->next->previous = object->previous;
    if(object->previous)
        object->previous->next = object->next;
    if(object_list == object)
        object_list = object->next;
    if(object_list_end == object)
        object_list_end = object->previous;
    object->previous = NULL;
    object->next = object_list;
    if(object_list)
        object_list->previous = object;
    object_list = object;
    if(!object_list_end)
        object_list_end = object;
    return retainObject(object);
}