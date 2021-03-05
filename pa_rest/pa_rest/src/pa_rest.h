// This file generated by ngrestcg
// For more information, please visit: https://github.com/loentar/ngrest

#ifndef PA_REST_H
#define PA_REST_H

#include <ngrest/common/Service.h>
#include <ngrest/common/Callback.h>
#include <ngrest/engine/Handler.h>
#include <vector>

//! Dummy description for the service
/*! Some detailed description of the service */
// '*location' comment sets resource path for this service
// *location: pa_rest

struct rest_userinfo {
    std::string name;
    std::string role;
    std::string company;
    std::string logo;
    bool online;
};

struct rest_appinfo {
    int app_id;
    std::string app_name;
    std::string app_description;
};

struct rest_stepinfo {
    int step_id;
    int order_number;
    std::string step_name;
    std::string step_description;
};

struct rest_ticket_brief {
    int ticket_id;
    std::string ticket_number;
    std::string creator;
    std::string timestamp;
    std::string assignee_role;
    std::string app_name;
    std::string next_step_name;
};

struct rest_tickets_part {
    std::vector<rest_ticket_brief> created_by_me;
    std::vector<rest_ticket_brief> operated_by_me;
    std::vector<rest_ticket_brief> need_i_handle;
};
class pa_rest: public ngrest::Service
{
public:
    // Here is an example of service operation
    //! Dummy description for the operation
    /*! Some detailed description of the operation */
    // To invoke this operation from browser open: http://localhost:9098/pa_rest/World!
    //
    // '*location' metacomment sets path to operation relative to service operation.
    // Default value is operation name.
    // This will bind "echo" method to resource path: http://host:port/pa_rest/{text}
    // *location: /{text}
    //
    // '*method' metacomment sets HTTP method for the operation. GET is default method.
    // *method: GET
    //
    std::string echo(const std::string& text);

    // *location: /userinfo/{pa_ssid}
    // *method: GET
    rest_userinfo proc_get_userinfo(const std::string& pa_ssid);

    // *location: /userinfo
   // *method: POST
    bool proc_post_userinfo(const std::string& pa_ssid, const std::string& name,  const std::string& company, const std::string& role);

    // *location: /wechat_login
    // *method: POST
    void proc_post_wechat_login_async(const std::string& code, ngrest::Callback<const std::string&>& callback);

    // *location: /company/{company_id}
    // *method: GET
    std::string proc_get_company(const std::string& company_id);

    // *location: /all_companies
    // *method: GET
    std::vector<std::string> proc_get_all_companies();
    // *location: /all_roles/{company_name}
    // *method: GET
    std::vector<std::string> proc_get_all_roles(const std::string& company_name);

    // *location: /upload_img
    // *method: POST
    bool proc_post_upload_img(const std::string& file_content, const std::string& pa_ssid);

    // *location: /pa_wx_sign
    // *method: POST
    void proc_post_wx_sign_async(const std::string& nonceStr, long timestamp, const std::string& url, ngrest::Callback<const std::string&>& callback);

    // *location: /company_id/{company_name}
    // *method: GET
    std::string proc_get_company_id(const std::string& company_name);


    // *location: /apps/{pa_ssid}
    // *method: GET
    std::vector<rest_appinfo> proc_get_apps(const std::string& pa_ssid);

    // *location: /steps/{app_id}
    // *method: GET
    std::vector<rest_stepinfo> proc_get_steps(int app_id);

    // *location: /ticket
    // *method: POST
    std::string proc_post_ticket(const std::string& pa_ssid, int step_id, const std::string& comments);

    // *location: /tickets_brief/{pa_ssid}
    // *method: GET
    rest_tickets_part proc_get_tickets_brief(const std::string& pa_ssid);
};

#endif // PA_REST_H
