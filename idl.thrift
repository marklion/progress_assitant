
exception gen_exp {
    1:string msg,
}

struct user_info {
    1:i64 user_id,
    2:string name,
    3:string logo,
    4:string company,
    5:bool buyer,
    6:string phone,
}

struct driver_info {
    1:string name, 
    2:string phone,
    3:i64 id,
}

struct vichele_info_t {
    1:string number,
    2:i64 id,
}

service user_management{
    user_info get_user_info(1:string ssid) throws (1:gen_exp e),
    string user_login(1:string code) throws (1:gen_exp e),
    bool update_user_info(1:user_info info, 2:string ssid, 3:string verify_code) throws (1:gen_exp e),
    void logff_user(1:string ssid) throws (1:gen_exp e),
    list<vichele_info_t> get_bound_vichele(1:string ssid, 2:bool main_vichele) throws (1:gen_exp e),
    bool bind_new_vichele(1:string ssid, 2:string vichele, 3:bool main_vichele) throws (1:gen_exp e),
    void remove_vichele(1:string ssid, 2:i64 id, 3:bool main_vichele) throws (1:gen_exp e),
    bool update_logo(1:string content, 2:string ssid) throws (1:gen_exp e),
    string get_customer_info(1:i64 user_id) throws (1:gen_exp e),
    bool is_admin(1:string ssid) throws (1:gen_exp e),
    string get_wx_api_signature(1:i64 timestamp, 2:string nonceStr, 3:string url),
    list<driver_info> get_bound_driver_info(1:string ssid) throws (1:gen_exp e),
    bool bind_new_driver(1:string ssid, 2:driver_info driver) throws (1:gen_exp e),
    void remove_driver(1:string ssid, 2:i64 id) throws (1:gen_exp e),
    bool send_sms_verify(1:string ssid, 2:string phone) throws (1:gen_exp e),
    string get_user_email(1:string ssid) throws (1:gen_exp e),
    bool has_apply(1:string ssid) throws (1:gen_exp e),
}

struct stuff_detail {
    1:string name, 
    2:string last,
    3:i64 price,
    4:string company,
    5:i64 type_id,
    6:bool saling,
    7:string company_address,
    8:string company_contact,
}

struct user_apply {
    1:string name, 
    2:string phone,
    3:string logo,
    4:i64 apply_id,
    5:i64 status,
}

struct company_address_contact_info {
    1:string address,
    2:string contact,
}

struct company_attachment {
    1:i64 id,
    2:string path,
    3:string pic_path,
}

service company_management {
    list<i64> get_all_type(1:string ssid) throws (1:gen_exp e),
    i64 add_type(1:string name, 2:i64 price, 3:string last,  4:string ssid) throws (1:gen_exp e),
    bool edit_type(1:stuff_detail stuff, 2:string ssid) throws (1:gen_exp e),
    void remove_type(1:stuff_detail stuff, 2:string ssid) throws (1:gen_exp e),
    bool readd_type(1:stuff_detail stuff, 2:string ssid) throws (1:gen_exp e),
    list<user_apply> get_all_apply(1:string ssid) throws (1:gen_exp e),
    bool approve_apply(1:i64 apply_id, 2:string ssid, 3:bool approve) throws (1:gen_exp e),
    string generate_statistics(1:string ssid, 2:i64 begin_date, 3:i64 end_date) throws (1:gen_exp e),
    bool set_notice(1:string ssid, 2:string notice) throws (1:gen_exp e),
    string get_notice(1:string company_name) throws (1:gen_exp e),
    void clear_notice(1:string ssid) throws (1:gen_exp e),
    list<user_info> get_all_compay_user(1:string ssid) throws (1:gen_exp e),
    bool remove_user_from_company(1:string ssid, 2:i64 user_id) throws (1:gen_exp e),
    string get_company_logo(1:string ssid) throws (1:gen_exp e),
    bool set_address(1:string ssid, 2:string address) throws (1:gen_exp e),
    string get_address(1:string ssid) throws (1:gen_exp e),
    bool set_contact(1:string ssid, 2:string contact) throws (1:gen_exp e),
    string get_contact(1:string ssid) throws (1:gen_exp e),
    company_address_contact_info get_address_contact(1:string company_name) throws (1:gen_exp e),
    bool add_attachment(1:string ssid, 2:binary base64content, 3:bool is_pdf) throws (1:gen_exp e),
    void del_attachment(1:string ssid, 2:i64 id) throws (1:gen_exp e),
    list<company_attachment> get_all_attachment(1:string ssid) throws (1:gen_exp e),
    string get_attachment(1:string company_name) throws (1:gen_exp e),
}

service stuff_info {
    list<stuff_detail> get_today() throws (1:gen_exp e),
    stuff_detail get_stuff_detail(1:i64 type_id) throws (1:gen_exp e),
}

struct plan_confirm_info {
    1:string timestamp,
    2:string name,
}

struct pay_confirm_info {
    1:string timestamp,
    2:string name,
}

struct vichele_in_plan {
    1:string main_vichele,
    2:string behind_vichele,
    3:string driver_name,
    4:string driver_phone,
    5:double count,
    6:string drop_address,
    7:string use_for,
}

struct stuff_plan {
    1:i64 type_id,
    2:double count,
    3:list<vichele_in_plan> vichele_info,
    4:i64 plan_id,
    5:string plan_time,
    6:i64 created_time,
    7:string name, 
    8:double price,
    9:i64 status,
    10:string comment,
    11:plan_confirm_info plan_confirm,
    12:pay_confirm_info pay_confirm,
    13:string pay_info,
    14:string pay_timestamp,
    15:string close_timestamp,
    16:string close_by,
    17:string except_close_by,
    18:string except_close_timestamp,
    19:string except_close_reason,
    20:string proxy_company,
    21:string created_user_name,
    22:string buy_company,
    23:string sale_company,
    24:string reject_reason,
}

struct plan_status {
    1:i64 plan_id, 
    2:i64 status,
    3:i64 plan_time,
    4:string conflict_reason,
}

service stuff_plan_management {
    i64 create_plan(1:stuff_plan plan, 2:string ssid, 3:string proxy_company) throws (1:gen_exp e),
    list<plan_status> get_created_plan(1:string ssid) throws (1:gen_exp e),
    list<plan_status> get_company_plan(1:string ssid) throws (1:gen_exp e),
    stuff_plan get_plan(1:i64 plan_id) throws (1:gen_exp e),
    bool update_plan(1:stuff_plan plan, 2:string ssid) throws (1:gen_exp e),
    bool confirm_plan(1:i64 plan_id, 2:string ssid) throws (1:gen_exp e),
    bool has_priv_edit(1:i64 plan_id, 2:string ssid) throws (1:gen_exp e),
    bool upload_payinfo(1:i64 plan_id, 2:string ssid, 3:string content) throws (1:gen_exp e),
    bool confirm_pay(1:i64 plan_id, 2:string ssid) throws (1:gen_exp e),
    bool confirm_close(1:i64 plan_id, 2:string ssid) throws (1:gen_exp e),
    string export_plan(1:string ssid, 2:list<i64> plan_ids) throws (1:gen_exp e),
    bool except_close(1:i64 plan_id, 2:string ssid, 3:string reason) throws (1:gen_exp e),
    string verify_plan(1:stuff_plan plan, 2:string ssid) throws (1:gen_exp e),
    bool send_file_via_email(1:string ssid, 2:string filepath, 3:string email) throws (1:gen_exp e),
    bool reject_plan(1:i64 plan_id, 2:string ssid, 3:string reject_reason) throws (1:gen_exp e),
}
