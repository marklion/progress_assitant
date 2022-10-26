#if !defined(_COMPANY_MANAGEMENT_IMP_H_)
#define _COMPANY_MANAGEMENT_IMP_H_

#include <Python.h>
#include "../gen_code/company_management.h"
#include "../pa_util/pa_utils.h"
#include <algorithm>
#include "../external_src/CJsonObject.hpp"
#include "../external_src/Base64.h"
#include "../pa_util/pa_status_rule.h"
#include "../external_src/writer.hpp"
#include "pa_rpc_util.h"
#include "../pa_util/pa_advance.h"
#include "stuff_plan_management_imp.h"

#define SALE_CONFIG_FILE "/conf/data_config.json"
class company_management_handler : virtual public company_managementIf
{
public:
    company_management_handler() {
    }
    ~company_management_handler() {
    }
    virtual void get_all_type(std::vector<int64_t> &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                auto all_stuff_type = company->get_all_children<pa_sql_stuff_info>("belong_company");
                all_stuff_type.sort([] (pa_sql_stuff_info &s1, pa_sql_stuff_info &s2) {
                    return s1.saling > s2.saling;
                });
                for (auto &itr : all_stuff_type)
                {
                    _return.push_back(itr.get_pri_id());
                }
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }
    virtual int64_t add_type(const std::string &name, const int64_t price, const std::string &last, const std::string &ssid)
    {
        int64_t ret = 0;
        auto user = PA_DATAOPT_get_online_user(ssid,true);
        if (user) {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company) {
                auto exist_type = company->get_children<pa_sql_stuff_info>("belong_company", "name = '%s'", name.c_str());
                if (!exist_type)
                {
                    pa_sql_stuff_info tmp;
                    tmp.name = name;
                    tmp.price = price;
                    tmp.last = last;
                    tmp.set_parent(*company, "belong_company");
                    tmp.saling = 1;
                    ret = tmp.insert_record();
                }
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        return ret;
    }
    void check_related_balance(pa_sql_plan &_plan);
    virtual bool edit_type(const stuff_detail &stuff, const std::string &ssid)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (user)
        {
            if (user->groupid != 1)
            {
                PA_RETURN_NOPRIVA_MSG();
            }
            auto company = user->get_parent<pa_sql_company>("belong_company");
            auto stuff_need_edit = sqlite_orm::search_record<pa_sql_stuff_info>(stuff.type_id);
            if (stuff_need_edit)
            {
                auto stuff_company = stuff_need_edit->get_parent<pa_sql_company>("belong_company");
                if (stuff_company && company && stuff_company->get_pri_id() == company->get_pri_id())
                {
                    stuff_need_edit->last = stuff.last;
                    auto orig_price = stuff_need_edit->price;
                    stuff_need_edit->price = stuff.price;
                    stuff_need_edit->need_sec_check = stuff.need_sec_check;

                    ret = stuff_need_edit->update_record();
                    if (orig_price != stuff_need_edit->price)
                    {
                        std::string remark = "调整了该计划中的货品单价，原价" + std::to_string(orig_price) + "，现价" + std::to_string(stuff_need_edit->price);
                        auto related_plans = PA_RPC_get_all_plans_related_by_user(ssid, "belong_stuff_ext_key == %ld AND status < 4", stuff_need_edit->get_pri_id());
                        for (auto &itr : related_plans)
                        {
                            itr.price = stuff.price;
                            itr.update_record();
                            itr.send_wechat_msg(*user, remark);
                            check_related_balance(itr);
                        }
                    }
                }
                else
                {
                    PA_RETURN_NOPRIVA_MSG();
                }
            }
            else
            {
                PA_RETURN_NOSTUFF_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        return ret;
    }
    virtual void remove_type(const stuff_detail &stuff, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            auto stuff_need_remove = sqlite_orm::search_record<pa_sql_stuff_info>(stuff.type_id);
            if (stuff_need_remove)
            {
                auto stuff_company = stuff_need_remove->get_parent<pa_sql_company>("belong_company");
                if (stuff_company && company && stuff_company->get_pri_id() == company->get_pri_id())
                {
                    stuff_need_remove->saling = 0;
                    stuff_need_remove->update_record();
                }
                else
                {
                    PA_RETURN_NOPRIVA_MSG();
                }
            }
            else
            {
                PA_RETURN_NOSTUFF_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }

    virtual void get_all_apply(std::vector<user_apply> &_return, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto all_apply = opt_user->get_all_children<pa_sql_user_apply>("assignee");
            for (auto &itr : all_apply)
            {
                auto assigner_user = itr.get_parent<pa_sql_userinfo>("assigner");
                if (assigner_user)
                {
                    user_apply tmp;
                    tmp.apply_id = itr.get_pri_id();
                    tmp.logo = assigner_user->logo;
                    tmp.name = assigner_user->name;
                    tmp.phone = assigner_user->phone;
                    tmp.status = itr.status;
                    _return.push_back(tmp);
                }
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
    }
    virtual bool approve_apply(const int64_t apply_id, const std::string &ssid, const bool approve)
    {
        bool ret = false;
        auto apply = sqlite_orm::search_record<pa_sql_user_apply>(apply_id);
        auto opt_user = PA_DATAOPT_get_online_user(ssid, true);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (!apply || apply->status != 0)
        {
            PA_RETURN_MSG("申请不存在或已审批");
        }
        if (apply && opt_user && apply->status == 0)
        {
            auto company = opt_user->get_parent<pa_sql_company>("belong_company");
            auto assigner = apply->get_parent<pa_sql_userinfo>("assigner");
            if (company && assigner)
            {
                if (approve)
                {
                    assigner->set_parent(*company, "belong_company");
                    assigner->buyer = 0;
                    ret = assigner->update_record();
                    apply->status = 1;
                    ret &= apply->update_record();
                }
                else
                {
                    apply->status = 2;
                    ret = apply->update_record();
                }
                if (ret)
                {
                    PA_WECHAT_send_process_apply_msg(*assigner, *apply);
                }
                auto other_apply = assigner->get_all_children<pa_sql_user_apply>("assigner");
                for (auto &itr : other_apply)
                {
                    itr.status = apply->status;
                    itr.update_record();
                }
            }
        }

        return ret;
    }

    virtual bool readd_type(const stuff_detail &stuff, const std::string &ssid)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            auto stuff_need_readd = sqlite_orm::search_record<pa_sql_stuff_info>(stuff.type_id);
            if (stuff_need_readd)
            {
                auto stuff_company = stuff_need_readd->get_parent<pa_sql_company>("belong_company");
                if (stuff_company && company && stuff_company->get_pri_id() == company->get_pri_id())
                {
                    stuff_need_readd->saling = 1;
                    ret = stuff_need_readd->update_record();
                }
                else
                {
                    PA_RETURN_NOPRIVA_MSG();
                }
            }
            else
            {
                PA_RETURN_NOSTUFF_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        return ret;
    }

    virtual void generate_statistics(std::string &_return, const std::string &ssid, const int64_t begin_date, const int64_t end_date)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        std::list<pa_sql_plan> statistics_plan;
        if (opt_user)
        {
            if (opt_user->buyer)
            {
                statistics_plan = opt_user->get_all_children<pa_sql_plan>("created_by", "create_time >= %ld AND create_time <= %ld AND status = 4", begin_date, end_date);
            }
            else
            {
                auto company = opt_user->get_parent<pa_sql_company>("belong_company");
                if (company)
                {
                    auto stuffs = company->get_all_children<pa_sql_stuff_info>("belong_company");
                    for (auto &itr : stuffs)
                    {
                        auto plans = itr.get_all_children<pa_sql_plan>("belong_stuff", "create_time >= %ld AND create_time <= %ld AND status = 4", begin_date, end_date);
                        statistics_plan.insert(statistics_plan.begin(), plans.begin(), plans.end());
                    }
                }
                else
                {
                    PA_RETURN_NOCOMPANY_MSG();
                }
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (statistics_plan.size() > 0)
        {
            std::string file_name_no_ext = std::to_string(begin_date) + "-" + std::to_string(end_date);
            std::string file_name = file_name_no_ext + ".csv";
            std::ofstream stream("/dist/logo_res/" + file_name);
            std::string csv_bom = {
                (char)0xef, (char)0xbb, (char)0xbf};
            stream << csv_bom;
            csv2::Writer<csv2::delimiter<','>> writer(stream);
            std::vector<std::string> table_header = {
                "计划单号", "货品名称", "计划量", "总价", "计划人", "计划到厂", "提货时间"};
            writer.write_row(table_header);
            for (auto &itr : statistics_plan)
            {
                auto archive_plan = itr.get_parent<pa_sql_archive_plan>("archived");
                if (archive_plan)
                {
                    auto archive_status = archive_plan->get_children<pa_sql_archive_status_in_plan>("belong_plan", "status_index = 3");
                    std::string deliver_time = "未提货";
                    if (archive_status)
                    {
                        deliver_time = archive_status->timestamp;
                    }
                    std::vector<std::string> one_record = {
                        archive_plan->plan_number, archive_plan->stuff_name, archive_plan->count, archive_plan->total_price, archive_plan->created_user, archive_plan->plan_time, deliver_time};
                    writer.write_row(one_record);
                }
            }
            stream.close();

            std::string py_converter =
                "import pandas as pd\n"
                "import sys\n"
                "csv = pd.read_csv('/dist/logo_res/" +
                file_name + "', encoding='utf-8')\n"
                            "csv.to_excel('/dist/logo_res/" +
                file_name_no_ext + ".xlsx', sheet_name='data', index=False)\n";

            if (Py_IsInitialized())
            {
                PyRun_SimpleString(py_converter.c_str());
            }
            else
            {
                PA_RETURN_MSG("导出失败");
            }

            _return = "/logo_res/" + file_name_no_ext + ".xlsx";
        }
    }

    virtual bool set_notice(const std::string &ssid, const std::string &notice)
    {
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company)
            {
                company->notice = notice;
                ret = company->update_record();
            }
            else
            {
                PA_RETURN_NOCOMPANY_MSG();
            }
        }
        else
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        return ret;
    }

    virtual void get_notice(std::string &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (company)
        {
            _return = company->notice;
        }
        else
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
    }
    virtual void clear_notice(const std::string &ssid)
    {
        set_notice(ssid, "");
    }
    virtual void get_all_compay_user(std::vector<user_info> &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        auto all_users = company->get_all_children<pa_sql_userinfo>("belong_company");
        for (auto &itr : all_users)
        {
            user_info tmp;
            tmp.buyer = itr.buyer;
            tmp.company = company->name;
            tmp.logo = itr.logo;
            tmp.name = itr.name;
            tmp.phone = itr.phone;
            tmp.user_id = itr.get_pri_id();
            tmp.groupid = itr.groupid;
            tmp.is_read_only = itr.is_read_only;
            _return.push_back(tmp);
        }
    }

    virtual bool remove_user_from_company(const std::string &ssid, const int64_t user_id)
    {
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto user_need_remove = company->get_children<pa_sql_userinfo>("belong_company", "PRI_ID = %ld", user_id);
        if (!user_need_remove)
        {
            PA_RETURN_MSG("用户不存在");
        }
        auto related_plan = user_need_remove->get_all_children<pa_sql_plan>("created_by", "status < 4");
        if (related_plan.size() > 0)
        {
            PA_RETURN_RELATED_PLAN_OPEN();
        }
        auto login_user = user_need_remove->get_children<pa_sql_userlogin>("online_user");
        if (login_user)
        {
            login_user->remove_record();
        }
        user_need_remove->remove_record();

        return true;
    }

    virtual void get_company_logo(std::string &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        _return = company->logo;
    }

    std::unique_ptr<pa_sql_company> get_belong_company(const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        return company;
    }

    virtual bool set_address(const std::string &ssid, const std::string &address)
    {
        auto company = get_belong_company(ssid);
        company->address = address;
        return company->update_record();
    }
    virtual void get_address(std::string &_return, const std::string &ssid)
    {
        auto company = get_belong_company(ssid);
        _return = company->address;
    }
    virtual bool set_contact(const std::string &ssid, const std::string &contact)
    {
        auto company = get_belong_company(ssid);
        company->contact = contact;
        return company->update_record();
    }
    virtual void get_contact(std::string &_return, const std::string &ssid)
    {
        auto company = get_belong_company(ssid);
        _return = company->contact;
    }

    virtual void get_address_contact(company_address_contact_info &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        _return.address = company->address;
        _return.contact = company->contact;
    }

    void update_company_attachment_pic(pa_sql_company &_company)
    {
        auto attachments = _company.get_all_children<pa_sql_company_attachment>("belong_company");
        std::string params;
        for (auto &itr : attachments)
        {
            params += "/dist" + itr.pic_path + " ";
        }
        std::string cmd = "flock /tmp/script_lock -c \"/script/long_pic_make.sh " + params + "\"";
        auto fp = popen(cmd.c_str(), "r");
        if (fp)
        {
            char buff[1024];
            std::string content;
            unsigned int read_len = 0;
            while (0 < (read_len = fread(buff, 1, sizeof(buff), fp)))
            {
                content.append(buff, read_len);
            }
            if (content.length() > 0)
            {
                _company.attachment_picture = PA_DATAOPT_store_logo_to_file(content, std::to_string(_company.get_pri_id()) + "attach");
            }
            else
            {
                _company.attachment_picture = "";
            }
            _company.update_record();

            pclose(fp);
        }
    }

    virtual bool add_attachment(const std::string &ssid, const std::string &base64content, const bool is_pdf)
    {
        bool ret = false;
        auto company = get_belong_company(ssid);
        std::string content;

        Base64::Decode(base64content, &content);
        if (content.length() > 0)
        {
            pa_sql_company_attachment tmp;
            tmp.set_parent(*company, "belong_company");
            ret = tmp.insert_record();
            auto file_name = std::to_string(company->get_pri_id()) + std::to_string(tmp.get_pri_id()) + "single_attach";

            tmp.res_path = PA_DATAOPT_store_attach_file(content, is_pdf, file_name);
            if (is_pdf)
            {
                std::string cmd = "flock /tmp/script_lock -c \"/script/long_pic_make.sh /dist" + tmp.res_path + "\"";
                auto fp = popen(cmd.c_str(), "r");
                if (fp)
                {
                    char buff[1024];
                    std::string content;
                    unsigned int read_len = 0;
                    while (0 < (read_len = fread(buff, 1, sizeof(buff), fp)))
                    {
                        content.append(buff, read_len);
                    }
                    tmp.pic_path = PA_DATAOPT_store_attach_file(content, false, file_name);
                    pclose(fp);
                }
            }
            else
            {
                tmp.pic_path = tmp.res_path;
            }

            ret = tmp.update_record();
            if (ret)
            {
                update_company_attachment_pic(*company);
            }
        }

        return ret;
    }
    virtual void del_attachment(const std::string &ssid, const int64_t id)
    {
        auto company = get_belong_company(ssid);
        auto attach = company->get_children<pa_sql_company_attachment>("belong_company", "PRI_ID = %ld", id);
        if (attach)
        {
            attach->remove_record();
            update_company_attachment_pic(*company);
        }
    }
    virtual void get_all_attachment(std::vector<company_attachment> &_return, const std::string &ssid)
    {
        auto company = get_belong_company(ssid);
        auto all_attach = company->get_all_children<pa_sql_company_attachment>("belong_company");
        for (auto &itr : all_attach)
        {
            company_attachment tmp;
            tmp.id = itr.get_pri_id();
            tmp.path = itr.res_path;
            tmp.pic_path = itr.pic_path;
            _return.push_back(tmp);
        }
    }
    virtual void get_attachment(std::string &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        if (company->attachment_picture.length() == 0)
        {
            PA_RETURN_MSG("无资质证明");
        }
        _return = company->attachment_picture;
    }

    std::unique_ptr<real_access_record> make_access_record(pa_sql_userinfo &user)
    {
        auto company = user.get_parent<pa_sql_company>("belong_company");
        if (company)
        {
            std::unique_ptr<real_access_record> ret(new real_access_record());
            ret->name = user.name;
            ret->logo = user.logo;
            ret->attachment = company->attachment_picture;
            ret->company_name = company->name;
            ret->phone = user.phone;
            return ret;
        }
        return std::unique_ptr<real_access_record>();
    }

    virtual void get_real_access(std::vector<real_access_record> &_return, const std::string &ssid)
    {
        auto plans = PA_RPC_get_all_plans_related_by_user(ssid, "(proxy_company == '' OR proxy_company IS NULL) GROUP BY created_by_ext_key");
        for (auto &itr : plans)
        {
            auto created_user = itr.get_parent<pa_sql_userinfo>("created_by");
            if (created_user)
            {
                auto access_record = make_access_record(*created_user);
                if (access_record)
                {
                    _return.push_back(*access_record);
                }
            }
        }
    }

    virtual void
    get_all_access(std::vector<real_access_record> &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (pa_advance_has_permission(*user, PA_ADVANCE_GET_ALL_ACCESS_RECORD))
        {
            auto all_buyer = sqlite_orm::search_record_all<pa_sql_userinfo>("buyer != 0");
            for (auto &itr : all_buyer)
            {
                auto access_record = make_access_record(itr);
                if (access_record)
                {
                    _return.push_back(*access_record);
                }
            }
        }
    }

    virtual bool add_contract(const std::string &ssid, const common_contract &contract)
    {
        bool ret = false;

        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (user->buyer)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto b_side_company = user->get_parent<pa_sql_company>("belong_company");
        auto a_side_company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", contract.a_side_company.c_str());
        if (!a_side_company)
        {
            pa_sql_company tmp;
            tmp.name = contract.a_side_company;
            tmp.insert_record();
            a_side_company.reset(sqlite_orm::search_record<pa_sql_company>("name = '%s'", contract.a_side_company.c_str()).release());
        }

        if (!a_side_company || !b_side_company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        auto exist_contract = b_side_company->get_children<pa_sql_contract>("b_side", "a_side_ext_key = %ld", a_side_company->get_pri_id());
        if (exist_contract)
        {
            PA_RETURN_MSG("合同已存在");
        }

        pa_sql_contract tmp;
        tmp.end_time = contract.end_time;
        tmp.number = contract.number;
        tmp.start_time = contract.start_time;
        tmp.set_parent(*a_side_company, "a_side");
        tmp.set_parent(*b_side_company, "b_side");
        tmp.customer_code = contract.customer_code;
        tmp.max_vehicle_limit = contract.max_vehicle_limit;

        ret = tmp.insert_record();
        if (ret)
        {
            tmp.update_status();
        }

        return ret;
    }
    virtual void del_contract(const std::string &ssid, const int64_t id)
    {
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (user->buyer)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        auto contract = company->get_children<pa_sql_contract>("b_side", "PRI_ID = %ld", id);
        if (!contract)
        {
            PA_RETURN_MSG("合同不存在");
        }
        contract->remove_record();
    }
    virtual void get_all_contract(std::vector<common_contract> &_return, const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }

        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        std::string query = "b_side";
        if (user->buyer)
        {
            query = "a_side";
        }
        auto all_contract = company->get_all_children<pa_sql_contract>(query, "PRI_ID != 0 ORDER BY PRI_ID DESC");
        for (auto &itr : all_contract)
        {
            itr.update_status();
            auto a_side_company = itr.get_parent<pa_sql_company>("a_side");
            auto b_side_company = itr.get_parent<pa_sql_company>("b_side");
            if (a_side_company && b_side_company)
            {
                common_contract tmp;
                tmp.a_side_company = a_side_company->name;
                tmp.b_side_company = b_side_company->name;
                tmp.end_time = itr.end_time;
                tmp.id = itr.get_pri_id();
                tmp.number = itr.number;
                tmp.start_time = itr.start_time;
                tmp.status = itr.status;
                tmp.customer_code = itr.customer_code;
                tmp.balance = itr.balance;
                tmp.max_vehicle_limit = itr.max_vehicle_limit;
                _return.push_back(tmp);
            }
        }
    }

    virtual void get_contract(common_contract &_return, const std::string &a_side_company, const std::string &b_side_company)
    {
        auto a_side = sqlite_orm::search_record<pa_sql_company>("name = '%s'", a_side_company.c_str());
        auto b_side = sqlite_orm::search_record<pa_sql_company>("name = '%s'", b_side_company.c_str());
        if (!a_side || !b_side)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto contract = a_side->get_children<pa_sql_contract>("a_side", "b_side_ext_key = %ld", b_side->get_pri_id());
        if (!contract)
        {
            PA_RETURN_MSG("合同不存在");
        }
        contract->update_status();
        _return.a_side_company = a_side_company;
        _return.b_side_company = b_side_company;
        _return.end_time = contract->end_time;
        _return.id = contract->get_pri_id();
        _return.number = contract->number;
        _return.start_time = contract->start_time;
        _return.status = contract->status;
        _return.customer_code = contract->customer_code;
        _return.balance = contract->balance;
        _return.max_vehicle_limit = contract->max_vehicle_limit;
    }

    virtual bool set_work_time(const std::string &ssid, const int64_t start_work_time, const int64_t end_work_time)
    {
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = user->get_parent<pa_sql_company>("belong_company");
        if (!company || user->buyer)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        company->work_start_time = start_work_time;
        company->work_end_time = end_work_time;

        return company->update_record();
    }

    virtual void get_work_time(company_work_time &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        _return.start_time = company->work_start_time;
        _return.end_time = company->work_end_time;
    }

    virtual bool update_contract(const std::string &ssid, const common_contract &contract)
    {
        bool ret = false;

        auto contract_from_sql = sqlite_orm::search_record<pa_sql_contract>(contract.id);
        if (!contract_from_sql)
        {
            PA_RETURN_MSG("合同不存在");
        }

        contract_from_sql->end_time = contract.end_time;
        contract_from_sql->number = contract.number;
        contract_from_sql->start_time = contract.start_time;
        contract_from_sql->customer_code = contract.customer_code;
        contract_from_sql->max_vehicle_limit = contract.max_vehicle_limit;

        contract_from_sql->update_status();
        ret = contract_from_sql->update_record();

        return ret;
    }

    virtual void get_company_position_config(company_positon_lat_lag &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (!company)
        {
            PA_RETURN_MSG("公司不存在");
        }

        auto positon = PA_DATAOPT_get_position_config(company_name);
        _return.distance = positon.distance;
        _return.lag = positon.lag;
        _return.lat = positon.lat;
    }

    virtual bool set_third_info(const third_dev_info &_info, const std::string &ssid)
    {
        bool ret = false;
        auto opt_user = PA_DATAOPT_get_online_user(ssid, true);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        company->third_key = _info.key;
        company->third_url = _info.ctrl_url;
        company->third_dms_url = _info.dms_url;
        company->third_token = _info.token;
        company->zc_url = _info.zc_url;
        company->zh_ssid = _info.zh_ssid;
        company->remote_event_url = _info.remote_event_url;

        ret = company->update_record();

        return ret;
    }

    virtual void get_third_info(third_dev_info &_return, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        _return.key = company->third_key;
        _return.ctrl_url = company->third_url;
        _return.dms_url = company->third_dms_url;
        _return.token = company->third_token;
        _return.zc_url = company->zc_url;
        _return.zh_ssid = company->zh_ssid;
        _return.remote_event_url = company->remote_event_url;
    }

    virtual void get_related_company(std::vector<std::string> &_return, const std::string &ssid)
    {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        if (opt_user->buyer == 0)
        {
            auto related_plan = PA_RPC_get_all_plans_related_by_user(ssid, "proxy_company == '' AND status < 4 GROUP BY created_by_ext_key");
            for (auto &itr : related_plan)
            {
                auto created_user = itr.get_parent<pa_sql_userinfo>("created_by");
                if (created_user)
                {
                    auto belong_company = created_user->get_parent<pa_sql_company>("belong_company");
                    if (belong_company)
                    {
                        _return.push_back(belong_company->name);
                    }
                }
            }
            related_plan = PA_RPC_get_all_plans_related_by_user(ssid, "proxy_company != '' AND status < 4 GROUP BY proxy_company");
            for (auto &itr : related_plan)
            {
                _return.push_back(itr.proxy_company);
            }

            std::sort(_return.begin(), _return.end());
            _return.erase(std::unique(_return.begin(), _return.end()), _return.end());
            return;
        }
        else
        {
            auto related_plan = PA_RPC_get_all_plans_related_by_user(ssid, "status < 4 GROUP BY belong_stuff_ext_key");
            for (auto &itr : related_plan)
            {
                auto belong_stuff = itr.get_parent<pa_sql_stuff_info>("belong_stuff");
                if (belong_stuff)
                {
                    auto belong_company = belong_stuff->get_parent<pa_sql_company>("belong_company");
                    if (belong_company)
                    {
                        _return.push_back(belong_company->name);
                    }
                }
            }
            std::sort(_return.begin(), _return.end());
            _return.erase(std::unique(_return.begin(), _return.end()), _return.end());
        }
    }

    virtual bool set_user_group(const std::string &ssid, const int64_t user_id, const int64_t groupid)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid, true);
        if (!opt_user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = opt_user->get_parent<pa_sql_company>("belong_company");
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        if (!PA_DATAOPT_is_admin(opt_user->phone, company->name))
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto user = company->get_children<pa_sql_userinfo>("belong_company", "PRI_ID == %ld", user_id);
        if (!user)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        user->groupid = groupid;
        return user->update_record();
    }

    virtual bool add_gps_stuff(const std::string &ssid, const std::string &stuff_name)
    {
        bool ret = false;

        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto exist_record = company->get_children<pa_sql_gps_stuff>("belong_company", "stuff_name == '%s'", stuff_name.c_str());
        if (exist_record)
        {
            PA_RETURN_MSG("已存在");
        }
        pa_sql_gps_stuff tmp;
        tmp.stuff_name = stuff_name;
        tmp.set_parent(*company, "belong_company");
        ret = tmp.insert_record();

        return ret;
    }
    virtual bool del_gps_stuff(const std::string &ssid, const std::string &stuff_name)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto exist_record = company->get_children<pa_sql_gps_stuff>("belong_company", "stuff_name == '%s'", stuff_name.c_str());
        if (!exist_record)
        {
            PA_RETURN_MSG("不存在");
        }
        exist_record->remove_record();

        return true;
    }
    virtual void get_gps_stuff(std::vector<std::string> &_return, const std::string &ssid)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto all_gps_stuff = company->get_all_children<pa_sql_gps_stuff>("belong_company");
        for (auto &itr : all_gps_stuff)
        {
            _return.push_back(itr.stuff_name);
        }
    }

    virtual bool add_stamp_pic(const std::string &ssid, const std::string &pic_base64)
    {
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        if (!PA_DATAOPT_is_admin(user->phone, company->name))
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        std::string file_content;
        Base64::Decode(pic_base64, &file_content);
        company->stamp_pic = PA_DATAOPT_store_attach_file(file_content, false, company->name + "stamp" + std::to_string(time(nullptr)));
        return company->update_record();
    }
    virtual bool del_stamp_pic(const std::string &ssid)
    {
        auto user = PA_DATAOPT_get_online_user(ssid, true);
        if (!user)
        {
            PA_RETURN_UNLOGIN_MSG();
        }
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        if (!PA_DATAOPT_is_admin(user->phone, company->name))
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        if (company->stamp_pic.length() > 0)
        {
            std::string rm_cmd = "rm " + company->stamp_pic;
            system(rm_cmd.c_str());
        }
        company->stamp_pic = "";
        return company->update_record();
    }
    virtual void get_stamp_pic(std::string &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (company)
        {
            _return = company->stamp_pic;
        }
    }

    virtual void get_customize(company_customize &_return, const std::string &_company_name)
    {
        bool ret = false;
        std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
        std::istreambuf_iterator<char> beg(config_file), end;
        std::string config_string(beg, end);
        neb::CJsonObject config(config_string);

        for (size_t i = 0; i < config.GetArraySize(); i++)
        {
            auto company_config = config[i];
            auto company_name = company_config("name");
            if (company_name == _company_name && company_config.KeyExist("customize"))
            {
                auto company_customize_config = company_config["customize"];
                company_customize_config.Get("need_license", _return.need_driver_license);
                company_customize_config.Get("need_register", _return.need_driver_register);
                company_customize_config.Get("need_balance_auto_change", _return.need_balance_auto_change);
                company_customize_config.Get("need_sec_check", _return.need_sec_check);
                break;
            }
        }
    }
    enum company_customize_need_en
    {
        need_driver_register,
        need_driver_license,
        need_balance_auto_change,
        need_sec_check,
    };
    bool company_customize_need(const std::string &_company_name, company_customize_need_en _need)
    {
        bool ret = false;

        company_customize tmp;
        get_customize(tmp, _company_name);
        switch (_need)
        {
        case need_driver_license:
            ret = tmp.need_driver_license;
            break;
        case need_driver_register:
            ret = tmp.need_driver_register;
            break;
        case need_balance_auto_change:
            ret = tmp.need_balance_auto_change;
            break;
        case need_sec_check:
            ret = tmp.need_sec_check;
            break;
        default:
            break;
        }
        return ret;
    }

    virtual bool add_event_sub(const std::string &ssid, const std::string &event_name)
    {
        bool ret = false;
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }

        auto evts = company->event_types;
        std::vector<std::string> evt_vs;
        if (std::string::npos == evts.find(event_name))
        {
            evts.append(event_name + ";");
        }
        company->event_types = evts;
        ret = company->update_record();

        return ret;
    }
    virtual bool del_event_sub(const std::string &ssid, const std::string &event_name)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOCOMPANY_MSG();
        }
        auto pos = company->event_types.find(event_name);
        if (pos != std::string::npos)
        {
            company->event_types.erase(pos, event_name.length() + 1);
        }

        return company->update_record();
    }
    virtual void get_event_sub(std::vector<std::string> &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (company)
        {
            auto last_events = company->event_types;
            auto found_pos = last_events.find_first_of(';');
            while (found_pos != std::string::npos)
            {
                _return.push_back(last_events.substr(0, found_pos));
                last_events.erase(0, found_pos + 1);
                found_pos = last_events.find_first_of(';');
            }
        }
    }

    virtual void get_execute_record(execute_record_info &_return, const int64_t contract_id, const std::string &begin_date, const std::string &end_date)
    {
        auto contract = sqlite_orm::search_record<pa_sql_contract>(contract_id);
        if (contract)
        {
            auto ers = contract->get_all_children<pa_sql_execute_record>("belong_contract", "date(plan_date) >= ('%s') AND date(plan_date) <= date('%s')", begin_date.c_str(), end_date.c_str());
            int64_t vc = 0;
            int64_t dc = 0;
            for (auto &itr : ers)
            {
                vc += itr.plan_vehicle_count;
                dc += itr.deliver_count;
            }
            _return.deliver_count = dc;
            _return.vehicle_count = vc;
        }
    }

    virtual int add_license_require_by_name(const std::string &ssid, const std::string &name)
    {
        int ret = -1;
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company || !company->is_sale)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto exist_record = company->get_children<pa_sql_license_require>("belong_company", "name == '%s'", name.c_str());
        if (exist_record)
        {
            ret = exist_record->get_pri_id();
        }
        else
        {
            pa_sql_license_require tmp;
            tmp.name = name;
            tmp.set_parent(*company, "belong_company");
            if (tmp.insert_record())
            {
                ret = tmp.get_pri_id();
            }
        }

        return ret;
    }
    virtual bool add_license_require(const std::string &ssid, const license_require_info &lic_info)
    {
        bool ret = false;
        auto tmp = lic_info;
        tmp.id = add_license_require_by_name(ssid, lic_info.name);
        ret = update_license_require(ssid, tmp);

        return ret;
    }
    virtual bool update_license_require(const std::string &ssid, const license_require_info &lic_info)
    {
        bool ret = false;

        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company || !company->is_sale)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto lr = sqlite_orm::search_record<pa_sql_license_require>(lic_info.id);
        if (!lr)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        lr->name = lic_info.name;
        lr->prompt = lic_info.prompt;
        lr->input_method = "";
        for (auto &itr : lic_info.input_method)
        {
            lr->input_method.append(std::to_string(itr));
        }
        lr->use_for = lic_info.use_for;

        ret = lr->update_record();

        return ret;
    }
    virtual void del_license_require(const std::string &ssid, const int64_t id)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company || !company->is_sale)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto exist_record = company->get_children<pa_sql_license_require>("belong_company", "PRI_ID == %d", id);
        if (exist_record)
        {
            exist_record->remove_record();
        }
    }
    virtual void get_license_require(std::vector<license_require_info> &_return, const std::string &company_name)
    {
        auto company = sqlite_orm::search_record<pa_sql_company>("name = '%s'", company_name.c_str());
        if (company)
        {
            auto lrs = company->get_all_children<pa_sql_license_require>("belong_company");
            for (auto &itr : lrs)
            {
                license_require_info tmp;
                tmp.name = itr.name;
                tmp.id = itr.get_pri_id();
                for (auto &si : itr.input_method)
                {
                    tmp.input_method.push_back(license_input_method::type(si - '0'));
                }
                tmp.prompt = itr.prompt;
                tmp.use_for = license_use_for::type(itr.use_for);
                _return.push_back(tmp);
            }
        }
    }

    virtual void get_execute_rate_by_name(execute_record_info &_return, const std::string &ssid, const std::string &name)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        auto customer = sqlite_orm::search_record<pa_sql_company>("name == '%s'", name.c_str());
        if (!company || !customer)
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto contract = company->get_children<pa_sql_contract>("b_side", "a_side_ext_key == %ld", customer->get_pri_id());
        if (!contract)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        auto today = PA_DATAOPT_current_time().substr(0, 10);
        get_execute_record(_return, contract->get_pri_id(), today, today);
    }

    virtual void export_exe_rate(std::string &_return, const std::string &ssid, const std::string &begin_date, const std::string &end_date)
    {
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!company)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        struct export_exe_item
        {
            std::string company_name;
            int vehicle_count = 0;
            int deliver_count = 0;
        };
        std::vector<export_exe_item> export_items;
        auto related_customers = company->get_all_children<pa_sql_contract>("b_side");
        for (auto &itr : related_customers)
        {
            execute_record_info tmp;
            get_execute_record(tmp, itr.get_pri_id(), begin_date, end_date);
            export_exe_item tmp_item;
            auto customer = itr.get_parent<pa_sql_company>("a_side");
            if (customer)
            {
                tmp_item.company_name = customer->name;
            }
            tmp_item.deliver_count = tmp.deliver_count;
            tmp_item.vehicle_count = tmp.vehicle_count;
            export_items.push_back(tmp_item);
        }
        std::string file_name_no_ext = "exe_rate_export" + std::to_string(time(NULL)) + std::to_string(company->get_pri_id());
        std::string file_name = "/dist/logo_res/" + file_name_no_ext + ".csv";
        std::ofstream stream(file_name);
        std::string csv_bom = {
            (char)0xef, (char)0xbb, (char)0xbf};
        stream << csv_bom;
        csv2::Writer<csv2::delimiter<','>> writer(stream);
        std::vector<std::string> table_header = {
            "客户名称", "计划数", "完成数", "完成率"};
        writer.write_row(table_header);
        for (auto &itr : export_items)
        {
            std::vector<std::string> one_record;
            one_record.push_back(itr.company_name);
            one_record.push_back(std::to_string(itr.vehicle_count));
            one_record.push_back(std::to_string(itr.deliver_count));
            if (itr.vehicle_count > 0)
            {
                one_record.push_back(std::to_string(itr.deliver_count * 100 / itr.vehicle_count) + "%");
            }
            else
            {
                one_record.push_back("无");
            }
            writer.write_row(one_record);
        }
        stream.close();
        std::string py_converter =
            "import pandas as pd\n"
            "import sys\n"
            "csv = pd.read_csv('" +
            file_name + "', encoding='utf-8')\n"
                        "csv.index = csv.index + 1\n"
                        "csv.to_excel('/dist/logo_res/" +
            file_name_no_ext + ".xlsx', sheet_name='data')\n";

        if (Py_IsInitialized())
        {
            PyRun_SimpleString(py_converter.c_str());
            _return = "/logo_res/" + file_name_no_ext + ".xlsx";
        }
        else
        {
            PA_RETURN_MSG("导出失败");
        }
    }

    virtual bool change_user_read_only(const std::string &ssid, const int64_t user_id)
    {
        bool ret = false;

        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        auto company = PA_DATAOPT_get_company_by_ssid(ssid);
        if (!opt_user || !company || !PA_DATAOPT_is_admin(opt_user->phone, company->name))
        {
            PA_RETURN_NOPRIVA_MSG();
        }

        auto dest_user = company->get_children<pa_sql_userinfo>("belong_company", "PRI_ID == %ld", user_id);
        if (!dest_user)
        {
            PA_RETURN_NOPRIVA_MSG();
        }
        dest_user->is_read_only = !dest_user->is_read_only;

        ret = dest_user->update_record();

        return ret;
    }
};
#endif // _COMPANY_MANAGEMENT_IMP_H_