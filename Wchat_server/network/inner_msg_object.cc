#include "inner_msg_object.h"

OBJ_HANDLE MSGObject::next_object_id_ = -1;

MSGObject::MSGObject(void)
{
    if (next_object_id_ = -1) {
        next_object_id_ = 2000; // start obj_id
    }

    object_id_ = next_object_id_++;
}

int MSGObject::on_msg(InnerMsg msg)
{
    return 0;
}