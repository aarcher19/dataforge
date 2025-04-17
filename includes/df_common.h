#ifndef DF_COMMON_H
#define DF_COMMON_H

typedef enum
{
    DF_OK = 0,
    DF_ERR_NULL_PTR,
    DF_ERR_ALLOC_FAILED,
    DF_ERR_INDEX_OUT_OF_BOUNDS,
    DF_ERR_OUT_OF_RANGE,
    DF_ERR_EMPTY,
    DF_ERR_ALREADY_FREED,
    DF_ERR_ELEMENT_NOT_FOUND,
    DF_ERR_END_OF_LIST,
} DfError;

const char *df_error_to_string(DfError err);

typedef struct
{
    DfError error;
    void *value;
} DfResult;

#endif