#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "basic_head.h"
#include "message.h"

typedef uint8_t OBJ_HANDLE;

class MSGObject {
public:
    MSGObject(void);
    virtual ~MSGObject(void);

    OBJ_HANDLE get_handler(void) const { return object_id_; }
    virtual int on_msg(InnerMsg msg);

private:;
    static OBJ_HANDLE next_object_id_;
    OBJ_HANDLE object_id_;
};

#endif