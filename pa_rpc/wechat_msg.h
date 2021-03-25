#if !defined(_WECHAT_MSG_H_)
#define _WECHAT_MSG_H_

#include "pa_utils.h"

void PA_WECHAT_send_plan_msg(pa_sql_userinfo &_touser, pa_sql_plan &_plan);

#endif // _WECHAT_MSG_H_
