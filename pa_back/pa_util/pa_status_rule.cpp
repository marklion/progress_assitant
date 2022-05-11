#include "pa_status_rule.h"
#include "pa_data_base.h"

static bool plan_is_created_by_user(pa_sql_plan &_plan, pa_sql_userinfo &_user)
{
    bool ret = false;

    auto created_user = _plan.get_parent<pa_sql_userinfo>("created_by");
    if (created_user && created_user->get_pri_id() == _user.get_pri_id())
    {
        ret = true;
    }
    else if (_user.is_sys_admin)
    {
        ret = true;
    }
    return ret;
}

static bool plan_is_belong_to_user(pa_sql_plan &_plan, pa_sql_userinfo &_user)
{
    bool ret = false;

    auto opt_company = _user.get_parent<pa_sql_company>("belong_company");
    auto stuff = _plan.get_parent<pa_sql_stuff_info>("belong_stuff");
    if (stuff)
    {
        auto belong_company = stuff->get_parent<pa_sql_company>("belong_company");
        if (belong_company && opt_company && belong_company->get_pri_id() == opt_company->get_pri_id())
        {
            ret = true;
        }
        else if (_user.is_sys_admin)
        {
            ret = true;
        }
    }

    return ret;
}

class pa_status_item_init : public pa_status_item
{
public:
    virtual int get_index()
    {
        return 0;
    }
    virtual std::string get_name()
    {
        return "计划创建";
    }
    virtual std::string get_prompt()
    {
        return "待提交";
    }
    virtual bool can_do_action(pa_sql_plan &_plan, pa_sql_userinfo &_user)
    {
        return plan_is_created_by_user(_plan, _user);
    }
};
class pa_status_item_confirm : public pa_status_item
{
public:
    virtual int get_index()
    {
        return 1;
    }
    virtual std::string get_name()
    {
        return "计划确认";
    }
    virtual std::string get_prompt()
    {
        return "待确认计划";
    }
    virtual bool can_do_action(pa_sql_plan &_plan, pa_sql_userinfo &_user)
    {
        return plan_is_belong_to_user(_plan, _user);
    }
};

class pa_status_item_pay_confirm : public pa_status_item
{
public:
    virtual int get_index()
    {
        return 2;
    }
    virtual std::string get_name()
    {
        return "收款确认";
    }
    virtual std::string get_prompt()
    {
        return "待确认收款";
    }
    virtual bool can_do_action(pa_sql_plan &_plan, pa_sql_userinfo &_user)
    {
        return plan_is_belong_to_user(_plan, _user);
    }
};

class pa_status_item_deliver : public pa_status_item
{
public:
    virtual int get_index()
    {
        return 3;
    }
    virtual std::string get_name()
    {
        return "出货";
    }
    virtual std::string get_prompt()
    {
        return "待出货确认";
    }
    virtual bool can_do_action(pa_sql_plan &_plan, pa_sql_userinfo &_user)
    {
        return plan_is_belong_to_user(_plan, _user);
    }
};

class pa_status_item_close : public pa_status_item
{
public:
    virtual int get_index()
    {
        return 4;
    }
    virtual std::string get_name()
    {
        return "计划关闭";
    }
    virtual std::string get_prompt()
    {
        return "已归档";
    }
    virtual bool can_do_action(pa_sql_plan &_plan, pa_sql_userinfo &_user)
    {
        return true;
    }
};

std::vector<std::shared_ptr<pa_status_item>> g_all_status;

void PA_STATUS_RULE_init()
{
    g_all_status.push_back(std::make_shared<pa_status_item_init>());
    g_all_status.push_back(std::make_shared<pa_status_item_confirm>());
    g_all_status.push_back(std::make_shared<pa_status_item_pay_confirm>());
    g_all_status.push_back(std::make_shared<pa_status_item_deliver>());
    g_all_status.push_back(std::make_shared<pa_status_item_close>());

    auto created_can_do = [](pa_sql_plan &_plan, pa_sql_userinfo &_user) -> bool {
        return plan_is_created_by_user(_plan, _user);
    };
    auto company_staff_can_do = [](pa_sql_plan &_plan, pa_sql_userinfo &_user) -> bool {
        return plan_is_belong_to_user(_plan, _user);
    };

    auto both_can_do = [=](pa_sql_plan &_plan, pa_sql_userinfo &_user) -> bool {
        return created_can_do(_plan, _user) || company_staff_can_do(_plan, _user);
    };

    // 初次填写或修改内容后提交
    g_all_status[0]->set_next(1, created_can_do);
    // 确认计划
    g_all_status[1]->set_next(2, company_staff_can_do);
    // 收款正常
    g_all_status[2]->set_next(3, company_staff_can_do);
    // 出货确认
    g_all_status[3]->set_next(4, company_staff_can_do);

    // 驳回后修改
    g_all_status[0]->set_next(0, created_can_do);
    // 驳回计划
    g_all_status[1]->set_next(0, both_can_do);
    // 收款异常
    g_all_status[2]->set_next(1, company_staff_can_do);
    // 提货或付款前买方修改
    g_all_status[3]->set_next(0, created_can_do);
    g_all_status[2]->set_next(0, created_can_do);
    //任意阶段直接关闭
    g_all_status[0]->set_next(4, both_can_do);
    g_all_status[1]->set_next(4, both_can_do);
    g_all_status[2]->set_next(4, both_can_do);
    g_all_status[4]->set_next(3, both_can_do);
}
std::vector<std::shared_ptr<pa_status_item>> PA_STATUS_RULE_get_all()
{
    return g_all_status;
}

bool PA_STATUS_RULE_can_be_change(pa_sql_plan &_plan, pa_sql_userinfo &_user, int _index)
{
    bool ret = false;

    if (_index >= 0 && _index < g_all_status.size() && _plan.status >= 0 && _plan.status < g_all_status.size())
    {
        ret = g_all_status[_plan.status]->can_be_convert(_index, _plan, _user);
    }

    return ret;
}

bool PA_STATUS_RULE_can_be_acted(pa_sql_plan &_plan, pa_sql_userinfo &_user)
{
    bool ret = false;

    if (_plan.status >= 0 && _plan.status < g_all_status.size())
    {
        ret = g_all_status[_plan.status]->can_do_action(_plan, _user);
    }

    return ret;
}
bool PA_STATUS_RULE_action(pa_sql_plan &_plan, pa_sql_userinfo &_user, const std::string &_timestamp, const std::string &_comment)
{
    bool ret = false;

    if (PA_STATUS_RULE_can_be_acted(_plan, _user))
    {
        auto plan_status = _plan.get_children<pa_sql_plan_status>("belong_plan", "status_index = %d", _plan.status);
        if (plan_status)
        {
            plan_status->comment = _comment;
            plan_status->timestamp = _timestamp;
            plan_status->set_parent(_user, "author");
            ret = plan_status->update_record();
        }
        else
        {
            pa_sql_plan_status tmp;
            tmp.comment = _comment;
            tmp.timestamp = _timestamp;
            tmp.status_index = _plan.status;
            tmp.set_parent(_plan, "belong_plan");
            tmp.set_parent(_user, "author");
            ret = tmp.insert_record();
        }
    }

    return ret;
}
bool PA_STATUS_RULE_change_status(pa_sql_plan &_plan, pa_sql_userinfo &_user)
{
    bool ret = false;

    auto next_index = -1;
    if (_plan.status >= 0 && _plan.status < g_all_status.size() - 1)
    {
        next_index = _plan.status + 1;
        ret = PA_STATUS_RULE_change_status(_plan, next_index, _user);
    }

    return ret;
}

bool PA_STATUS_RULE_change_status(pa_sql_plan &_plan, int _dest_index, pa_sql_userinfo &_user)
{
    bool ret = false;

    if (PA_STATUS_RULE_can_be_change(_plan, _user, _dest_index))
    {
        _plan.status = _dest_index;
        ret = _plan.update_record();
    }

    return ret;
}

std::map<int, std::shared_ptr<pa_sql_plan_status>> PA_STATUS_RULE_get_status(pa_sql_plan &_plan)
{
    std::map<int, std::shared_ptr<pa_sql_plan_status>> ret;

    auto statuses = _plan.get_all_children<pa_sql_plan_status>("belong_plan");
    for (auto &itr : statuses)
    {
        ret[itr.status_index] = std::make_shared<pa_sql_plan_status>(itr);
    }

    return ret;
}