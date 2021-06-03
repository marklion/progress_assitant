#if !defined(_WECHAT_MSG_H_)
#define _WECHAT_MSG_H_

#include "pa_utils.h"

void PA_WECHAT_send_plan_msg(pa_sql_userinfo &_touser, pa_sql_plan &_plan, const std::string &_remark);
void PA_WECHAT_send_create_apply_msg(pa_sql_userinfo &_touser, pa_sql_user_apply &_apply);
void PA_WECHAT_send_process_apply_msg(pa_sql_userinfo &_touser, pa_sql_user_apply &_apply);
std::string PA_WECHAT_wx_sign(const std::string& nonceStr, long timestamp, const std::string &url);
#endif // _WECHAT_MSG_H_
