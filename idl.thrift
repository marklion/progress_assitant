
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

struct plan_status_rule {
    1:string name,
    2:string author,
    3:string timestamp,
    4:string comment,
    5:i64 index,
    6:string prompt,
}

struct common_contract {
    1:string a_side_company,
    2:string b_side_company,
    3:string start_time,
    4:string end_time,
    5:string number,
    6:i64 id,
    7:i64 status,
    8:string customer_code,
}

struct real_access_record {
    1:string name,
    2:string logo,
    3:string phone,
    4:string company_name,
    5:string attachment,
}

struct company_work_time{
    1:i64 start_time,
    2:i64 end_time,
}

struct company_positon_lat_lag {
    1:double lat,
    2:double lag,
    3:double distance,
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
    list<real_access_record> get_real_access(1:string ssid) throws (1:gen_exp e),
    list<real_access_record> get_all_access(1:string ssid) throws (1:gen_exp e),
    bool add_contract(1:string ssid, 2:common_contract contract) throws (1:gen_exp e),
    bool update_contract(1:string ssid, 2:common_contract contract) throws (1:gen_exp e),
    void del_contract(1:string ssid, 2:i64 id) throws (1:gen_exp e),
    list<common_contract> get_all_contract(1:string ssid) throws (1:gen_exp e),
    common_contract get_contract(1:string a_side_company, 2:string b_side_company) throws (1:gen_exp e),
    bool set_work_time(1:string ssid, 2:i64 start_work_time, 3:i64 end_work_time) throws (1:gen_exp e),
    company_work_time get_work_time(1:string company_name) throws (1:gen_exp e),
    company_positon_lat_lag get_company_position_config(1:string company_name) throws (1:gen_exp e),
}

service stuff_info {
    list<stuff_detail> get_today(1:string ssid) throws (1:gen_exp e),
    list<stuff_detail> get_today_unfollow(1:string ssid) throws (1:gen_exp e),
    stuff_detail get_stuff_detail(1:i64 type_id, 2:string ssid) throws (1:gen_exp e),
    bool add_company_follow_stuff(1:string company_name, 2:i64 type_id, 3:string ssid) throws (1:gen_exp e),
    bool cancle_company_follow_stuff(1:string company_name, 2:i64 type_id, 3:string ssid) throws (1:gen_exp e),
    list<stuff_detail> get_follow_stuff_by_company(1:string company_name) throws (1:gen_exp e),
    list<string> get_follow_company_by_stuff(1:i64 type_id, 2:string ssid) throws (1:gen_exp e),
    list<string> get_related_stuff(1:string ssid) throws (1:gen_exp e),
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
    8:i64 vichele_id,
    9:bool finish,
    10:string deliver_timestamp,
    11:string register_timestamp,
    12:string register_number,
    13:string enter_location,
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
    10:string proxy_company,
    11:string created_user_name,
    12:string buy_company,
    13:string sale_company,
    14:string comment,
    15:bool is_cancel,
}

struct plan_status {
    1:i64 plan_id, 
    2:i64 status,
    3:i64 plan_time,
    4:string conflict_reason,
    5:string status_prompt,
    6:bool is_cancel,
    7:string stuff_type,
}

struct plan_number_id{
    1:i64 id,
    2:string number,
}

struct deliver_info {
    1:i64 id,
    2:double count,
}

struct vichele_statistics {
    1:string company,
    2:string main_vichele,
    3:string behind_vichele,
    4:string driver_name,
    5:string driver_phone,
    6:bool delivered,
    7:i64 plan_id,
    8:string plan_order,
}

struct vichele_stuff_statistics {
    1:vichele_statistics vichele,
    2:string stuff_name,
}

struct vichele_search_result {
    1:plan_number_id plan_info,
    2:string vichele_numbers,
    3:string plan_time,
    4:string status,
}

struct company_plan_brief {
    1:i64 today_plan_count,
    2:i64 today_vichele_count,
    3:i64 tomorrow_plan_count,
    4:i64 tomorrow_vichele_count,
}

struct today_driver_info {
    1:i64 id,
    2:string destination_company,
    3:string destination_address,
    4:string order_company,
    5:string main_vichele,
    6:string behind_vichele,
    7:string stuff_name,
    8:string register_timestamp,
    9:string register_number,
    10:string enter_location,
    11:bool is_registered,
}

struct driver_detail_info {
    1:string phone,
    2:string id,
}

service stuff_plan_management {
    i64 create_plan(1:stuff_plan plan, 2:string ssid, 3:string proxy_company) throws (1:gen_exp e),
    list<plan_status> get_created_plan(1:string ssid, 2:i64 anchor) throws (1:gen_exp e),
    list<plan_status> get_company_plan(1:string ssid, 2:i64 anchor) throws (1:gen_exp e),
    stuff_plan get_plan(1:i64 plan_id) throws (1:gen_exp e),
    bool update_plan(1:stuff_plan plan, 2:string ssid) throws (1:gen_exp e),
    bool confirm_plan(1:i64 plan_id, 2:string ssid, 3:string comment) throws (1:gen_exp e),
    bool confirm_pay(1:i64 plan_id, 2:string ssid, 3:string comment) throws (1:gen_exp e),
    bool confirm_deliver(1:i64 plan_id, 2:string ssid, 3:list<deliver_info> deliver_infos, 4:string reason) throws (1:gen_exp e),
    string export_plan(1:string ssid, 2:list<i64> plan_ids) throws (1:gen_exp e),
    bool except_close(1:i64 plan_id, 2:string ssid, 3:string reason) throws (1:gen_exp e),
    string verify_plan(1:stuff_plan plan, 2:string ssid) throws (1:gen_exp e),
    bool send_file_via_email(1:string ssid, 2:string filepath, 3:string email) throws (1:gen_exp e),
    bool reject_plan(1:i64 plan_id, 2:string ssid, 3:string reject_reason) throws (1:gen_exp e),
    list<plan_number_id> search_plan_by_driver_phone(1:string phone) throws (1:gen_exp e),
    list<plan_status_rule> get_status_rule(1:i64 plan_id) throws (1:gen_exp e),
    list<bool> get_change_rule(1:string ssid, 2:i64 plan_id) throws (1:gen_exp e),
    void clean_unclose_plan() throws (1:gen_exp e),
    list<vichele_statistics> get_today_statistics(1:string ssid) throws (1:gen_exp e),
    bool plan_created_by_user(1:string ssid, 2:i64 plan_id) throws (1:gen_exp e),
    string export_plan_by_plan_date(1:string ssid, 2:string plan_date, 3:string create_date) throws (1:gen_exp e),
    string export_plan_by_create_date(1:string ssid, 2:i64 begin_date, 3:i64 end_date) throws (1:gen_exp e),
    list<vichele_search_result> search_plan_by_vichele_number(1:string ssid, 2:string vichele_number) throws (1:gen_exp e),
    list<vichele_statistics> get_tomorrow_statistics(1:string ssid) throws (1:gen_exp e),
    company_plan_brief get_company_brief(1:string ssid) throws (1:gen_exp e),
    bool push_user_pay(1:string ssid, 2:i64 plan_id) throws (1:gen_exp e),
    i64 get_count_by_status(1:string ssid, 2:i64 status) throws (1:gen_exp e),
    bool cancel_vichele_from_plan(1:string ssid, 2:list<i64> ids) throws (1:gen_exp e),
    string driver_silent_login(1:string code) throws (1:gen_exp e),
    bool driver_silent_send_sms(1:string driver_phone) throws (1:gen_exp e),
    string driver_silent_register(1:string code, 2:string driver_id, 3:string driver_phone, 4:string verify_code) throws (1:gen_exp e),
    void driver_silent_unregister(1:string silent_id) throws (1:gen_exp e),
    bool verify_driver_silent_login(1:string silent_id) throws (1:gen_exp e),
    list<today_driver_info> get_today_driver_info(1:string silent_id) throws (1:gen_exp e),
    driver_detail_info get_driver_info(1:string silent_id) throws (1:gen_exp e),
    bool register_vichele(1:string silent_id, 2:i64 vichele_id) throws (1:gen_exp e),
    bool unregister_vichele(1:string silent_id, 2:i64 vichele_id) throws (1:gen_exp e),
}

struct api_extra_transformation {
    1:string driver_name,
    2:string driver_phone,
    3:string driver_id,
}

struct api_transformation_info {
    1:i64 id,
    2:string main_vichele_number,
    3:string behind_vichele_number,
    4:string stuff_name,
    5:double enter_count,
    6:double exit_count,
    7:string company_name,
    8:api_extra_transformation extra_info,
    9:bool is_sale,
}

service open_api_management {
    bool register_api_user(1:string company_name, 2:string email, 3:string password) throws (1:gen_exp e),
    bool verify_email_code(1:string email, 2:string code) throws (1:gen_exp e),
    bool unregister_api_user(1:string email, 2:string password) throws (1:gen_exp e),
    string get_token(1:string email, 2:string password) throws (1:gen_exp e),
    list<api_transformation_info> get_today_transformation(1:string token) throws (1:gen_exp e),
    bool push_exit_count(1:i64 id, 2:double count, 3:bool is_sale, 4:string token) throws (1:gen_exp e),
    bool push_arrange(1:i64 id, 2:string order, 3:bool is_sale, 4:string location, 5:string token) throws (1:gen_exp e),
}

struct vichele_stay_alone {
    1:i64 id,
    2:string stuff_name,
    3:string company_name,
    4:string main_vichele_number,
    5:string behind_vichele_number,
    6:double count,
    7:string comment,
    8:string date,
    9:string destination,
    10:i64 status,
    11:string creator_name,
    12:string creator_phone,
    13:bool repeated,
}

struct silent_user_info {
    1:string name,
    2:string phone,
}

service vichele_management {
    bool create_vichele_info(1:string open_id, 2:list<vichele_stay_alone> vichele_info) throws (1:gen_exp e),
    bool delete_vichele_info(1:string open_id, 2:i64 vichele_id) throws (1:gen_exp e),
    bool update_vichele_info(1:string open_id, 2:vichele_stay_alone vichele_info) throws (1:gen_exp e),
    list<vichele_stay_alone> get_created_vichele_info(1:string open_id, 2:i64 ancher) throws (1:gen_exp e),
    string silent_login(1:string code) throws (1:gen_exp e),
    bool verify_login(1:string open_id) throws (1:gen_exp e),
    silent_user_info get_silent_user_info(1:string open_id) throws (1:gen_exp e),
    void set_silent_user_info(1:string open_id, 2:silent_user_info info) throws (1:gen_exp e),
    list<string> get_input_history(1:string open_id, 2:vichele_stay_alone search_key) throws (1:gen_exp e),
    list<vichele_stay_alone> get_company_vichele_info(1:string ssid, 2:i64 anchor) throws (1:gen_exp e),
    bool confirm_vichele(1:string ssid, 2:list<vichele_stay_alone> info) throws (1:gen_exp e),
    bool cancel_vichele(1:string ssid, 2:list<vichele_stay_alone> info) throws (1:gen_exp e),
}