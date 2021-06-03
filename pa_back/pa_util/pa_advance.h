#if !defined(_PA_ADVANCE_H_)
#define _PA_ADVANCE_H_
#include "pa_data_base.h"

typedef enum {
    PA_ADVANCE_NOTHING,
    PA_ADVANCE_GET_ALL_ACCESS_RECORD,
} pa_advance_type;

bool pa_advance_has_permission(pa_sql_userinfo &_user, pa_advance_type _type);

#endif // _PA_ADVANCE_H_
