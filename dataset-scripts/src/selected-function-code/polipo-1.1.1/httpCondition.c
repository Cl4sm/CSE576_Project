int
httpCondition(ObjectPtr object, HTTPConditionPtr condition)
{
    int rc = CONDITION_MATCH;

    assert(!(object->flags & OBJECT_INITIAL));

    if(!condition) return CONDITION_MATCH;

    if(condition->ims >= 0) {
        if(object->last_modified < 0 ||
           condition->ims < object->last_modified)
            return rc;
        else
            rc = CONDITION_NOT_MODIFIED;
    }

    if(condition->inms >= 0) {
        if(object->last_modified < 0 || 
           condition->inms >= object->last_modified)
            return rc;
        else
            rc = CONDITION_FAILED;
    }

    if(condition->inm) {
        if(!object->etag || strcmp(object->etag, condition->inm) != 0)
            return rc;
        else
            rc = CONDITION_NOT_MODIFIED;
    }

    if(condition->im) {
        if(!object->etag || strcmp(object->etag, condition->im) != 0)
            rc = CONDITION_FAILED;
        else
            return rc;
    }

    return rc;
}