#include "../includes/df_common.h"

const char *df_error_to_string(DfError err)
{
    switch (err)
    {
    case DF_OK:
        return "No error";
    case DF_ERR_NULL_PTR:
        return "Null pointer";
    case DF_ERR_ALLOC_FAILED:
        return "Memory allocation failed";
    case DF_ERR_INDEX_OUT_OF_BOUNDS:
        return "Index out of bounds";
    case DF_ERR_OUT_OF_RANGE:
        return "Value out of range";
    case DF_ERR_EMPTY:
        return "Structure is empty";
    case DF_ERR_ALREADY_FREED:
        return "Memory has already been freed";
    default:
        return "Unknown error";
    }
}
