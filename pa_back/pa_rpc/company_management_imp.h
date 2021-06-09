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
class company_management_handler : virtual public company_managementIf
{
public:
    company_management_handler() {
        Py_Initialize();
    }
    ~company_management_handler() {
        Py_Finalize();
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
        sqlite_orm_lock a;
        auto user = PA_DATAOPT_get_online_user(ssid);
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
    virtual bool edit_type(const stuff_detail& stuff, const std::string& ssid) {
        bool ret = false;
        sqlite_orm_lock a;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user)
        {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            auto stuff_need_edit = sqlite_orm::search_record<pa_sql_stuff_info>(stuff.type_id);
            if (stuff_need_edit)
            {
                auto stuff_company = stuff_need_edit->get_parent<pa_sql_company>("belong_company");
                if (stuff_company && company && stuff_company->get_pri_id() == company->get_pri_id())
                {
                    stuff_need_edit->last = stuff.last;
                    stuff_need_edit->price = stuff.price;

                    ret = stuff_need_edit->update_record();
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
        sqlite_orm_lock a;
        auto user = PA_DATAOPT_get_online_user(ssid);
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

    virtual void get_all_apply(std::vector<user_apply> &_return, const std::string &ssid)  {
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
        if (opt_user)
        {
            auto all_apply = opt_user->get_all_children<pa_sql_user_apply>("assignee");
            for (auto &itr:all_apply)
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
    virtual bool approve_apply(const int64_t apply_id, const std::string &ssid, const bool approve) {
        bool ret = false;
        sqlite_orm_lock a;
        auto apply = sqlite_orm::search_record<pa_sql_user_apply>(apply_id);
        auto opt_user = PA_DATAOPT_get_online_user(ssid);
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
                for (auto &itr:other_apply)
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
        sqlite_orm_lock a;
        auto user = PA_DATAOPT_get_online_user(ssid);
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
        sqlite_orm_lock a;
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
                "csv = pd.read_csv('/dist/logo_res/" + file_name + "', encoding='utf-8')\n"
                "csv.to_excel('/dist/logo_res/" + file_name_no_ext + ".xlsx', sheet_name='data', index=False)\n";

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
        sqlite_orm_lock a;
        bool ret = false;
        auto user = PA_DATAOPT_get_online_user(ssid);
        if (user) {
            auto company = user->get_parent<pa_sql_company>("belong_company");
            if (company) {
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
        sqlite_orm_lock a;
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
        for (auto &itr:all_users)
        {
            user_info tmp;
            tmp.buyer = itr.buyer;
            tmp.company = company->name;
            tmp.logo = itr.logo;
            tmp.name = itr.name;
            tmp.phone = itr.phone;
            tmp.user_id = itr.get_pri_id();
            _return.push_back(tmp);
        }
    }

    virtual bool remove_user_from_company(const std::string &ssid, const int64_t user_id) 
    {
        sqlite_orm_lock a;
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
        sqlite_orm_lock a;
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
        sqlite_orm_lock a;
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

    void update_company_attachment_pic(pa_sql_company &_company) { 
        sqlite_orm_lock a;
        auto attachments = _company.get_all_children<pa_sql_company_attachment>("belong_company");
        std::string params;
        for (auto &itr:attachments)
        {
            params += "/dist" + itr.pic_path + " ";
        }
        std::string cmd = "flock /tmp/script_lock -c \"/script/long_pic_make.sh "  + params + "\"";
        auto fp = popen(cmd.c_str(), "r");
        if (fp)
        {
            char buff[1024];
            std::string content;
            unsigned int read_len = 0;
            while (0 < (read_len =  fread(buff, 1, sizeof(buff), fp)))
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
        sqlite_orm_lock a;
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
        sqlite_orm_lock a;
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
        for (auto &itr:all_attach)
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
        auto plans = PA_RPC_get_all_plans_related_by_user(ssid, "proxy_company == '' OR proxy_company IS NULL GROUP BY created_by_ext_key");
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
                auto access_record = make_access_record(itr );
                if (access_record)
                {
                    _return.push_back(*access_record);
                }
            }
        }
    }

    virtual bool add_contract(const std::string &ssid, const common_contract &contract)
    {
        sqlite_orm_lock a;
        bool ret = false;

        auto user = PA_DATAOPT_get_online_user(ssid);
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

        ret = tmp.insert_record();
        if (ret)
        {
            tmp.update_status();
        }

        return ret;
    }
    virtual void del_contract(const std::string &ssid, const int64_t id)
    {
        sqlite_orm_lock a;
        auto user = PA_DATAOPT_get_online_user(ssid);
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
        auto all_contract = company->get_all_children<pa_sql_contract>(query);
        for (auto &itr:all_contract)
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
    }
};
#endif // _COMPANY_MANAGEMENT_IMP_H_