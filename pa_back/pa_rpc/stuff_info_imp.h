#if !defined(_STUFF_INFO_IMP_H_)
#define _STUFF_INFO_IMP_H_

#include "../gen_code/stuff_info.h"
#include "../pa_util/pa_utils.h"
#include "pa_rpc_util.h"

class stuff_info_handler : virtual public stuff_infoIf
{
public:
    virtual void get_today(std::vector<stuff_detail> &_return, const std::string &ssid)
    {
        auto all_stuff = sqlite_orm::search_record_all<pa_sql_stuff_info>("");
        for (auto &itr : all_stuff)
        {
            if (itr.saling == 1)
            {
                if (PA_RPC_has_follow_stuff(ssid, itr.get_pri_id()))
                {
                    auto belong_company = itr.get_parent<pa_sql_company>("belong_company");
                    if (belong_company)
                    {
                        stuff_detail tmp;
                        tmp.company = belong_company->name;
                        tmp.last = itr.last;
                        tmp.name = itr.name;
                        tmp.price = itr.price;
                        tmp.type_id = itr.get_pri_id();
                        _return.push_back(tmp);
                    }
                }
            }
        }
    }
    virtual void get_stuff_detail(stuff_detail &_return, const int64_t type_id, const std::string &ssid)
    {
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        auto detail_permission = false;
        if (stuff_info)
        {
            auto company = stuff_info->get_parent<pa_sql_company>("belong_company");
            auto opt_user = PA_DATAOPT_get_online_user(ssid);
            if (company && opt_user)
            {
                if (company->get_children<pa_sql_userinfo>("belong_company", "PRI_ID = %ld", opt_user->get_pri_id()))
                {
                    detail_permission = true;
                }
                else if (PA_RPC_has_follow_stuff(ssid, stuff_info->get_pri_id()))
                {
                    detail_permission = true;
                }
                _return.company = company->name;
                if (detail_permission)
                {
                    _return.price = stuff_info->price;
                }
                _return.last = stuff_info->last;
                _return.name = stuff_info->name;
                _return.type_id = type_id;
                _return.saling = stuff_info->saling == 1 ? true : false;
                _return.company_address = company->address;
                _return.company_contact = company->contact;
            }
        }
    }

    virtual bool add_company_follow_stuff(const std::string &company_name, const int64_t type_id, const std::string &ssid)
    {
        bool ret = false;
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto stuff_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (stuff_company && opt_company && stuff_company->get_pri_id() == opt_company->get_pri_id())
        {
            auto follow_company = PA_DATAOPT_fetch_company(company_name);
            if (follow_company)
            {
                auto exist_follow = follow_company->get_children<pa_sql_company_follow>("follower", "follow_stuff_ext_key == %ld", stuff_info->get_pri_id());
                if (exist_follow)
                {
                    ret = true;
                }
                else
                {
                    pa_sql_company_follow tmp;
                    tmp.set_parent(*follow_company, "follower");
                    tmp.set_parent(*stuff_info, "follow_stuff");
                    ret = tmp.insert_record();
                }
            }
        }
        else
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        return ret;
    }
    virtual bool cancle_company_follow_stuff(const std::string &company_name, const int64_t type_id, const std::string &ssid)
    {
        bool ret = false;
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto stuff_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (stuff_company && opt_company && stuff_company->get_pri_id() == opt_company->get_pri_id())
        {
            auto follow_company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
            if (follow_company)
            {
                auto exist_follow = follow_company->get_children<pa_sql_company_follow>("follower", "follow_stuff_ext_key == %ld", stuff_info->get_pri_id());
                if (exist_follow)
                {
                    exist_follow->remove_record();
                    ret = true;
                }
            }
        }
        else
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        return ret;
    }
    virtual void get_follow_stuff_by_company(std::vector<stuff_detail> &_return, const std::string &company_name)
    {
        auto follow_company = sqlite_orm::search_record<pa_sql_company>("name == '%s'", company_name.c_str());
        if (follow_company)
        {
            auto follow_info = follow_company->get_all_children<pa_sql_company_follow>("follower");
            for (auto &itr : follow_info)
            {
                auto stuff_info = itr.get_parent<pa_sql_stuff_info>("follow_stuff");
                if (stuff_info)
                {
                    auto sale_company = stuff_info->get_parent<pa_sql_company>("belong_company");
                    if (sale_company)
                    {
                        stuff_detail tmp;
                        tmp.company = sale_company->name;
                        tmp.company_address = sale_company->address;
                        tmp.company_contact = sale_company->contact;
                        tmp.last = stuff_info->last;
                        tmp.name = stuff_info->name;
                        tmp.price = stuff_info->price;
                        tmp.saling = stuff_info->saling;
                        tmp.type_id = stuff_info->get_pri_id();
                        _return.push_back(tmp);
                    }
                }
            }
        }
    }
    virtual void get_follow_company_by_stuff(std::vector<std::string> &_return, const int64_t type_id, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto opt_company = opt_user->get_parent<pa_sql_company>("belong_company");
        auto stuff_info = sqlite_orm::search_record<pa_sql_stuff_info>(type_id);
        if (!stuff_info)
        {
            PA_RETURN_NOSTUFF_MSG();
        }
        auto stuff_company = stuff_info->get_parent<pa_sql_company>("belong_company");
        if (stuff_company && opt_company && stuff_company->get_pri_id() == opt_company->get_pri_id())
        {
            auto follow_info = stuff_info->get_all_children<pa_sql_company_follow>("follow_stuff");
            for (auto &itr : follow_info)
            {
                auto company = itr.get_parent<pa_sql_company>("follower");
                if (company)
                {
                    _return.push_back(company->name);
                }
            }
        }
    }

    virtual void get_today_unfollow(std::vector<stuff_detail> &_return, const std::string &ssid)
    {
        auto all_stuff = sqlite_orm::search_record_all<pa_sql_stuff_info>("");
        for (auto &itr : all_stuff)
        {
            if (itr.saling == 1)
            {
                if (!PA_RPC_has_follow_stuff(ssid, itr.get_pri_id()))
                {
                    auto belong_company = itr.get_parent<pa_sql_company>("belong_company");
                    if (belong_company)
                    {
                        stuff_detail tmp;
                        tmp.company = belong_company->name;
                        tmp.last = itr.last;
                        tmp.name = itr.name;
                        tmp.type_id = itr.get_pri_id();
                        _return.push_back(tmp);
                    }
                }
            }
        }
    }

    virtual void get_related_stuff(std::vector<std::string> &_return, const std::string &ssid)
    {
        auto all_plan_group_by_stuff = PA_RPC_get_all_plans_related_by_user(ssid, "PRI_ID != 0 GROUP BY belong_stuff_ext_key");
        for (auto &itr : all_plan_group_by_stuff)
        {
            auto stuff_info = itr.get_parent<pa_sql_stuff_info>("belong_stuff");
            if (stuff_info)
            {
                _return.push_back(stuff_info->name);
            }
        }
    }

    virtual bool create_bidding(const std::string &ssid, const bidding_params &bp)
    {
        bool ret = false;

        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company || !company->is_sale)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto stuff = company->get_children<pa_sql_stuff_info>("belong_company", "name == '%s'", bp.stuff_name.c_str());
        if (!stuff)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        pa_sql_bidding bd_tmp;
        bd_tmp.bidding_times = bp.bidding_times;
        bd_tmp.deposit = bp.deposit;
        bd_tmp.end_time = bp.end_time;
        bd_tmp.max_price = bp.max_price;
        bd_tmp.min_price = bp.min_price;
        bd_tmp.total_count = bp.total_count;
        bd_tmp.status = 0;
        bd_tmp.set_parent(*stuff, "belong_stuff");
        if (bd_tmp.insert_record())
        {
            pa_sql_bidding_turn bt_tmp;
            bt_tmp.status = 0;
            bt_tmp.turn = 1;
            bt_tmp.set_parent(bd_tmp, "belong_bidding");
            ret = bt_tmp.insert_record();
            for (auto &itr : bp.customers)
            {
                auto cc = sqlite_orm::search_record<pa_sql_company>("name = '%s'", itr.c_str());
                if (cc)
                {
                    pa_sql_bidding_customer bc_tmp;
                    bc_tmp.has_call = 0;
                    bc_tmp.price = 0;
                    bc_tmp.set_parent(bt_tmp, "belong_bidding_turn");
                    bc_tmp.set_parent(*cc, "call_company");
                    bc_tmp.insert_record();
                }
            }
        }

        return ret;
    }
    virtual void get_all_bidding(std::vector<bidding_params> &_return, const std::string &ssid, const int64_t status_condition)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        std::string status_query_cmd = "status == " + std::to_string(status_condition);
        if (-1 == status_condition)
        {
            status_query_cmd = "status != -1";
        }
        if (company->is_sale)
        {
            auto all_stuff = company->get_all_children<pa_sql_stuff_info>("belong_company");
            for (auto &single_stuff : all_stuff)
            {
                auto all_bidding = single_stuff.get_all_children<pa_sql_bidding>("belong_stuff");
                for (auto &itr : all_bidding)
                {
                    bidding_params tmp;
                    tmp.bidding_times = itr.bidding_times;
                    tmp.cur_status = itr.status;
                    tmp.deposit = itr.deposit;
                    tmp.end_time = itr.end_time;
                    tmp.id = itr.get_pri_id();
                    tmp.max_price = itr.max_price;
                    tmp.min_price = itr.min_price;
                    tmp.stuff_name = single_stuff.name;
                    tmp.total_count = itr.total_count;
                    auto bt_tmp = itr.get_all_children<pa_sql_bidding_turn>("belong_bidding");
                    for (auto &single_turn : bt_tmp)
                    {
                        bidding_status bs_tmp;
                        bs_tmp.status = single_turn.status;
                        bs_tmp.bidding_turn = single_turn.turn;
                        double top_price = 0;
                        std::string top_customer;
                        auto all_bc = single_turn.get_all_children<pa_sql_bidding_customer>("belong_bidding_turn");
                        for (auto &single_customer : all_bc)
                        {
                            bidding_customer bc_tmp;
                            auto customer = single_customer.get_parent<pa_sql_company>("call_company");
                            if (customer)
                            {
                                bc_tmp.company_name = customer->name;
                                if (single_customer.has_call)
                                {
                                    bc_tmp.price = single_customer.price;
                                    bc_tmp.timestamp = single_customer.timestamp;
                                    if (single_customer.price > top_price)
                                    {
                                        top_price = single_customer.price;
                                        top_customer = customer->name;
                                    }
                                }
                            }
                            bs_tmp.all_customers_price.push_back(bc_tmp);
                            if (single_turn.turn == 1)
                            {
                                tmp.customers.push_back(customer->name);
                            }
                        }
                        bs_tmp.cur_top_customer = top_customer;
                        bs_tmp.cur_top_price = top_price;
                        tmp.all_status.push_back(bs_tmp);
                    }
                    _return.push_back(tmp);
                }
            }
        }
        else
        {
            std::list<pa_sql_bidding_turn>  bidding_turn_by_company;
            auto bidding_call_company = company->get_all_children<pa_sql_bidding_customer>("call_company");
            for (auto &itr:bidding_call_company)
            {
                auto related_turn = itr.get_parent<pa_sql_bidding_turn>("belong_bidding_turn");
                if (related_turn && related_turn->turn == 1)
                {
                    bidding_turn_by_company.push_back(*related_turn);
                }
            }
            for (auto &itr : bidding_turn_by_company)
            {
                auto bidding_detail = itr.get_parent<pa_sql_bidding>("belong_bidding");
                if (bidding_detail && (-1 == status_condition || bidding_detail->status == status_condition))
                {
                    bidding_params tmp;
                    tmp.bidding_times = bidding_detail->bidding_times;
                    tmp.cur_status = bidding_detail->status;
                    tmp.deposit = bidding_detail->deposit;
                    tmp.end_time = bidding_detail->end_time;
                    tmp.id = bidding_detail->get_pri_id();
                    tmp.max_price = bidding_detail->max_price;
                    tmp.min_price = bidding_detail->min_price;
                    auto stuff = bidding_detail->get_parent<pa_sql_stuff_info>("belong_stuff");
                    tmp.stuff_name = stuff->name;
                    tmp.total_count = bidding_detail->total_count;
                    auto related_turn = bidding_detail->get_all_children<pa_sql_bidding_turn>("belong_bidding");
                    for (auto &single_turn : related_turn)
                    {
                        bidding_status bs_tmp;
                        bs_tmp.bidding_turn = single_turn.turn;
                        bs_tmp.status = single_turn.status;
                        auto related_call = single_turn.get_all_children<pa_sql_bidding_customer>("belong_bidding_turn", "call_company_ext_key == %ld", company->get_pri_id());
                        for (auto &single_call : related_call)
                        {
                            bidding_customer bc_tmp;
                            bc_tmp.company_name = company->name;
                            bc_tmp.price = single_call.price;
                            bc_tmp.timestamp = single_call.timestamp;
                            bs_tmp.all_customers_price.push_back(bc_tmp);
                        }
                        tmp.all_status.push_back(bs_tmp);
                    }
                    _return.push_back(tmp);
                }
            }
        }
    }
    virtual bool close_bidding(const std::string &ssid, const int64_t bidding_id)
    {
        bool ret = false;

        return ret;
    }
    virtual bool call_bidding(const std::string &ssid, const int64_t bidding_id, const double price)
    {
        bool ret = false;

        return ret;
    }
};
#endif // _STUFF_INFO_IMP_H_