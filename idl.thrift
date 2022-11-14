
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
    7:i64 groupid,
    8:bool is_read_only,
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
    string exchange_ssid(1:string enc_text) throws (1:gen_exp e),
    string exchange_enc_text(1:string ssid) throws (1:gen_exp e),
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
    9:bool need_sec_check,
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
    9:double balance,
    10:i64 max_vehicle_limit,
}

struct execute_record_info {
    1:i64 vehicle_count,
    2:i64 deliver_count,
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

struct third_dev_info {
    1:string key,
    2:string ctrl_url,
    3:string dms_url,
    4:string token,
    5:string zc_url,
    6:string zh_ssid,
    7:string remote_event_url,
}

struct company_customize {
    1:bool need_driver_license,
    2:bool need_driver_register,
    3:bool need_balance_auto_change,
    4:bool need_sec_check,
}

enum license_use_for {
    driver,
    main_vehicle,
    behind_vehicle,
}

enum license_input_method {
    input_content,
    upload_picture,
}

struct license_require_info{
    1:i64 id,
    2:string name,
    3:license_use_for use_for,
    4:list<license_input_method> input_method,
    5:string prompt,
    6:bool ltv,
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
    bool set_driver_notice(1:string ssid, 2:string notice) throws (1:gen_exp e),
    string get_driver_notice(1:string company_name) throws (1:gen_exp e),
    void clear_driver_notice(1:string ssid) throws (1:gen_exp e),
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
    bool set_third_info(1:third_dev_info _info, 2:string ssid) throws (1:gen_exp e),
    third_dev_info get_third_info(1:string ssid) throws (1:gen_exp e),
    list<string> get_related_company(1:string ssid) throws (1:gen_exp e),
    bool set_user_group(1:string ssid, 2:i64 user_id, 3:i64 groupid) throws (1:gen_exp e),
    bool add_gps_stuff(1:string ssid, 2:string stuff_name) throws (1:gen_exp e),
    bool del_gps_stuff(1:string ssid, 2:string stuff_name) throws (1:gen_exp e),
    list<string> get_gps_stuff(1: string ssid) throws (1:gen_exp e),
    bool add_stamp_pic(1:string ssid, 2:string pic_base64) throws (1:gen_exp e),
    bool del_stamp_pic(1:string ssid) throws (1:gen_exp e),
    string get_stamp_pic(1:string company_name) throws (1:gen_exp e),
    company_customize get_customize(1:string company_name) throws (1:gen_exp e),
    bool add_event_sub(1:string ssid, 2:string event_name) throws (1:gen_exp e),
    bool del_event_sub(1:string ssid, 2:string event_name) throws (1:gen_exp e),
    list<string> get_event_sub(1:string company_name) throws (1:gen_exp e),
    execute_record_info get_execute_record(1:i64 contract_id, 2:string begin_date, 3:string end_date) throws (1:gen_exp e),
    bool add_license_require(1:string ssid, 2:license_require_info lic_info) throws (1:gen_exp e),
    bool update_license_require(1:string ssid, 2:license_require_info lic_info) throws (1:gen_exp e),
    void del_license_require(1:string ssid, 2:i64 id) throws (1:gen_exp e),
    list<license_require_info> get_license_require(1:string company_name) throws (1:gen_exp e),
    execute_record_info get_execute_rate_by_name(1:string ssid, 2:string name) throws (1:gen_exp e),
    string export_exe_rate(1:string ssid, 2:string begin_date, 3:string end_date) throws (1:gen_exp e),
    bool change_user_read_only(1:string ssid, 2:i64 user_id) throws (1:gen_exp e),
}

struct bidding_customer{
    1:string company_name,
    2:double price,
    3:string timestamp,
}

struct bidding_status {
    1:string cur_top_customer,
    2:double cur_top_price,
    3:i64 bidding_turn,
    # 0--active 1--finish 2--exception_close
    4:i64 status,
    5:list<bidding_customer> all_customers_price,
    6:string end_time,
}

struct bidding_params {
    1:i64 id,
    2:string stuff_name,
    3:double max_price,
    4:double min_price,
    5:i64 bidding_times,
    6:list<string> customers,
    7:double deposit,
    8:double total_count,
    9:i64 cur_status,
    10:list<bidding_status> all_status,
}

struct price_timer_param {
    1:i64 id,
    2:string stuff_name,
    3:i64 hours,
    4:double price,
    5:string expired_time,
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
    bool create_bidding(1:string ssid, 2:bidding_params bp) throws (1:gen_exp e),
    # status_condition == -1 --> all_status
    list<bidding_params> get_all_bidding(1:string ssid, 2:i64 status_condition) throws (1:gen_exp e),
    bool close_bidding(1:string ssid, 2:i64 bidding_id) throws (1:gen_exp e),
    bool call_bidding(1:string ssid, 2:i64 bidding_id, 3:double price) throws (1:gen_exp e),
    bidding_params get_bidding(1:string ssid, 2:i64 bidding_id) throws (1:gen_exp e),
    bool create_price_timer(1:string ssid, 2:price_timer_param timer_p) throws (1:gen_exp e),
    void remove_price_timer(1:string ssid, 2:i64 id) throws (1:gen_exp e),
    list<price_timer_param> get_all_price_timer(1:string ssid) throws (1:gen_exp e),
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
    14:string p_time,
    15:double p_weight,
    16:double m_weight,
    17:string driver_id,
    18:string driver_silent_id,
    19:string archive_license,
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
    16:bool from_bidding,
}

struct plan_status {
    1:i64 plan_id,
    2:i64 status,
    3:i64 plan_time,
    4:string conflict_reason,
    5:string status_prompt,
    6:bool is_cancel,
    7:string stuff_type,
    8:string company,
}

struct plan_number_id{
    1:i64 id,
    2:string number,
}

struct deliver_info {
    1:i64 id,
    2:double count,
    3:double p_weight,
    4:double m_weight,
    5:string p_time,
    6:string m_time,
    7:string seal_no,
    8:string ticket_no,
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
    9:i64 vichele_id,
    10:bool sec_check_passed,
    11:double p_weight,
    12:double m_weight,
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

struct company_stuff_plan_brief {
    1:company_plan_brief brief,
    2:string stuff_name,
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
    12:string register_order,
    13:bool is_buy,
    14:list<string> company_for_select,
    15:bool need_tmd,
    16:string tmd_no,
    17:string date,
    18:bool can_enter,
    19:string attach_url,
    20:double count,
    21:bool upload_permit,
    22:bool need_license,
    23:bool need_checkin,
    24:bool sec_check_passed,
    25:bool need_sec_check,
    26:string notice,
}

struct driver_detail_info {
    1:string phone,
    2:string id,
}

struct driver_license_info {
    1:i64 id,
    2:string expire_date,
    3:string attachment_path,
}

struct vehicle_license_info {
    1:i64 id,
    2:string expire_date,
    3:string attachment_path,
}

struct license_common_data {
    1:i64 id,
    2:string expired_date,
    3:string input_content,
    4:string attachment_path,
    5:string related_info,
    6:i64 related_type_id,
    7:bool has_confirmed,
    8:string comment,
    9:string confirmer,
}

service stuff_plan_management {
    i64 create_plan(1:stuff_plan plan, 2:string ssid, 3:string proxy_company) throws (1:gen_exp e),
    list<plan_status> get_created_plan(1:string ssid, 2:i64 anchor, 3:i64 status, 4:string stuff_name, 5:string company_name, 6:string plan_date) throws (1:gen_exp e),
    list<plan_status> get_company_plan(1:string ssid, 2:i64 anchor, 3:i64 status, 4:string stuff_name, 5:string company_name, 6:string plan_date) throws (1:gen_exp e),
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
    list<vichele_stuff_statistics> get_today_statistics(1:string ssid) throws (1:gen_exp e),
    bool plan_created_by_user(1:string ssid, 2:i64 plan_id) throws (1:gen_exp e),
    string export_plan_by_plan_date(1:string ssid, 2:string plan_date, 3:string create_date) throws (1:gen_exp e),
    string export_plan_by_create_date(1:string ssid, 2:i64 begin_date, 3:i64 end_date) throws (1:gen_exp e),
    list<vichele_search_result> search_plan_by_vichele_number(1:string ssid, 2:string vichele_number) throws (1:gen_exp e),
    list<vichele_stuff_statistics> get_tomorrow_statistics(1:string ssid) throws (1:gen_exp e),
    list<company_stuff_plan_brief> get_company_brief(1:string ssid) throws (1:gen_exp e),
    bool push_user_pay(1:string ssid, 2:i64 plan_id) throws (1:gen_exp e),
    i64 get_count_by_status(1:string ssid, 2:i64 status) throws (1:gen_exp e),
    bool cancel_vichele_from_plan(1:string ssid, 2:list<i64> ids) throws (1:gen_exp e),
    string driver_silent_login(1:string code) throws (1:gen_exp e),
    bool driver_silent_send_sms(1:string driver_phone) throws (1:gen_exp e),
    string driver_silent_register(1:string code, 2:string driver_id, 3:string driver_phone, 4:string verify_code) throws (1:gen_exp e),
    void driver_silent_unregister(1:string silent_id) throws (1:gen_exp e),
    void driver_silent_reset(1:string ssid, 2:string silent_id ) throws (1:gen_exp e),
    bool verify_driver_silent_login(1:string silent_id) throws (1:gen_exp e),
    list<today_driver_info> get_today_driver_info(1:string silent_id) throws (1:gen_exp e),
    driver_detail_info get_driver_info(1:string silent_id) throws (1:gen_exp e),
    bool register_vichele(1:string silent_id, 2:i64 vichele_id) throws (1:gen_exp e),
    bool unregister_vichele(1:string silent_id, 2:i64 vichele_id) throws (1:gen_exp e),
    bool multi_confirm_plan(1:string ssid, 2:list<i64> plan_ids) throws (1:gen_exp e),
    bool change_driver(1:string ssid, 2:i64 vichele_id, 3:string driver_name, 4:string driver_phone) throws (1:gen_exp e),
    bool change_plan_price(1:string ssid, 2:list<i64> plan_id, 3:double new_price) throws (1:gen_exp e),
    driver_license_info add_driver_license(1:string silent_id, 2:string license_attachment_base64, 3:string expire_date) throws (1:gen_exp e),
    bool del_driver_license(1:string silent_id, 2:i64 license_data_id) throws (1:gen_exp e),
    bool update_driver_license(1:string silent_id, 2:string ssid, 3:driver_license_info license_data) throws (1:gen_exp e),
    list<driver_license_info> get_all_license_info_by_driver_phone(1:string ssid, 2:string phone) throws (1:gen_exp e),
    list<driver_license_info> get_self_all_license_info(1:string silent_id) throws (1:gen_exp e),
    vehicle_license_info add_vehicle_license(1:string silent_id, 2:string license_attachment_base64, 3:string expire_date, 4:string plate_no) throws (1:gen_exp e)
    void del_vehicle_license(1:string silent_id, 2:i64 data_id) throws (1:gen_exp e),
    list<vehicle_license_info> get_license_by_vehicle_number(1:string plate_no) throws (1:gen_exp e),
    string export_plan_by_deliver_date(1:string ssid, 2:string deliver_date) throws (1:gen_exp e),
    bool sec_check_pass(1:string ssid, 2:i64 vehicle_id) throws (1:gen_exp e),
    bool sec_check_reject(1:string ssid, 2:i64 vehicle_id) throws (1:gen_exp e),
    bool add_sec_check_data(1:string silent_id, 2:license_common_data lcd) throws (1:gen_exp e),
    bool update_sec_check_data(1:string silent_id, 2:license_common_data lcd) throws (1:gen_exp e),
    bool del_sec_check_data(1:string silent_id, 2:license_common_data lcd) throws (1:gen_exp e),
    license_common_data get_all_sec_check_data(1:i64 related_type_id, 2:string related_info) throws (1:gen_exp e),
    bool confirm_sec_check_data(1:string ssid, 2:i64 lcd_id, 3:bool is_confirm, 4:string comment) throws (1:gen_exp e);
    string export_sec_check_history(1:string ssid, 2:string begin_date, 3:string end_date) throws (1:gen_exp e),
    string export_plan_by_deliver_date_range(1:string ssid, 2:string begin_date, 3:string end_date) throws(1:gen_exp e),
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

struct call_vehicle_req {
    1:string plateNo,
    2:string driverName,
    3:i64 index,
    4:string stationName,
}

struct meta_stuff_info {
    1:string stuffId,
    2:string stuffName,
    3:double weight,
}

struct vehicle_info_resp {
    1:string id,
    2:string plateNo,
    3:string backPlateNo,
    4:string stuffName,
    5:string stuffId,
    6:double enterWeight,
    7:string companyName,
    8:string driverName,
    9:bool isSale,
    10:double price,
    11:string customerId,
    12:string orderNo,
    13:list<meta_stuff_info> multiStuff,
    14:bool isMulti,
    15:string createTime,
    16:string driverPhone,
    17:string driverId,
    18:string supplierName,
    19:string supplierId,
    20:string vehicleTeamName,
    21:string vehicleTeamId,
    22:string tmd_no,
    23:string attachUrl,
    24:list<vehicle_license_info> allLicenseInfo,
    25:string sale_address,
    26:bool has_license,
    27:bool has_payed,
}

struct push_weight_req {
    1:string id,
    2:string plateNo,
    3:string customerId,
    4:string customerName,
    5:string stuffName,
    6:double pWeight,
    7:double mWeight,
    8:string pTime,
    9:string mTime,
    10:double jWeight,
    11:string sealNo,
    12:string ticketNo,
}

struct push_base_req {
    1:string id,
    2:string name,
    3:string pid,
    4:string unit,
    5:string type,
    6:string code,
}

struct push_balance_req {
    1:string customerId,
    2:string customerName,
    3:double balance,
    4:string reason,
}

struct push_zone_change_req {
    1:string plateNo,
    2:i64 eventType,
    3:string eventInfo,
    4:string eventCreateTime,
}

struct push_manual_permit_req {
    1:string plateNo,
    2:string driverId,
}

struct ticket_detail {
    1:string ticket_no,
    2:string customer_name,
    3:string supplier_name,
    4:string main_vichele_number,
    5:string behind_vichele_number,
    6:string seal_no,
    7:string stuff_name,
    8:string p_weight,
    9:string m_weight,
    10:string j_weight,
    11:string m_date,
    12:string p_date,
    13:string title,
    14:string transfor_company,
}

struct today_plan_brief_info{
    1:string total_vehicle;
}

struct create_plan_req {
    1:string plateNo,
	2:string behindPlateNo,
	3:string driverName,
	4:string driverPhone,
	5:string driverID,
	6:string userFor,
	7:string deliverAddress,
	8:string arriveDate,
	9:string customerName,
    10:string stuffName,
}

service open_api_management {
    bool register_api_user(1:string company_name, 2:string email, 3:string password) throws (1:gen_exp e),
    bool verify_email_code(1:string email, 2:string code) throws (1:gen_exp e),
    bool unregister_api_user(1:string email, 2:string password) throws (1:gen_exp e),
    string get_token(1:string email, 2:string password) throws (1:gen_exp e),
    bool proc_call_vehicle(1:call_vehicle_req _req, 2:string token) throws (1:gen_exp e),
    vehicle_info_resp proc_vehicle_info(1:string plateNo, 2:string driverId, 3:string token) throws (1:gen_exp e),
    list<vehicle_info_resp> proc_all_vehicle_info(1:string token) throws (1:gen_exp e),
    bool proc_push_weight(1:push_weight_req _req, 2:string token) throws (1:gen_exp e),
    bool proc_add_black_list(1:i64 type, 2:string target, 3:string reason, 4:string expire_date, 5:string token) throws (1:gen_exp e),
    bool proc_del_black_list(1:i64 type, 2:string target, 3:string token) throws (1:gen_exp e),
    bool proc_add_base_info(1:push_base_req _req, 2:string token) throws (1:gen_exp e),
    bool proc_del_base_info(1:push_base_req _req, 2:string token) throws (1:gen_exp e),
    bool proc_push_balance(1:push_balance_req _req, 2:string token) throws (1:gen_exp e),
    bool proc_push_zone_change(1:push_zone_change_req _req, 2:string token) throws (1:gen_exp e),
    bool proc_push_manual_permit(1:push_manual_permit_req _req, 2:string token) throws (1:gen_exp e),
    ticket_detail get_vehicle_info_by_id(1:string id) throws (1:gen_exp e),
    bool modify_vehicle_info_from_ticket(1:string ssid, 2:ticket_detail ticket) throws (1:gen_exp e),
    list<push_balance_req> get_all_customer_balance(1:string token) throws (1:gen_exp e),
    string export_balance_audit_log(1:string token, 2:string company_name, 3:string begin_date, 4:string end_date) throws (1:gen_exp e),
    today_plan_brief_info get_today_brief_info(1:string token) throws (1:gen_exp e),
    bool undo_vehicle_weight(1:string token, 2:string id) throws (1:gen_exp e),
    bool record_p_weight(1:string token, 2:string id) throws (1:gen_exp e),
    string proc_create_plan(1:string token, 2:create_plan_req _req) throws (1:gen_exp e),
    bool proc_cancel_plan(1:string token, 2:string _order_number) throws (1:gen_exp e),
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
    14:string driver_name,
    15:string driver_phone,
    16:string driver_id,
    17:string transfor_company,
    18:string p_time,
    19:string m_time,
    20:double p_weight,
    21:double m_weight,
    22:double j_weight,
    23:double price,
    24:bool can_enter,
    25:bool upload_permit,
    26:string driver_silent_id,
}

struct silent_user_info {
    1:string name,
    2:string phone,
}

struct vichele_team_member {
    1:string main_vichele_number,
    2:string behind_vichele_number,
    3:string driver_name,
    4:string driver_phone,
    5:string driver_id,
}

struct vichele_team {
    1:list<vichele_team_member> members,
    2:string name,
    3:i64 id,
}

struct supplier_basic_info {
    1:string name,
    2:double reserves,
    3:i64 max_vichele,
    4:i64 id,
    5:string bound_stuff_name,
}

struct single_vichele_brief {
    1:list<string> stuff_names,
    2:list<string> supplier_names,
}

struct vichele_stay_alone_statistics {
    1:i64 yestarday_left,
    2:i64 yestarday_total,
    3:i64 today_finish,
    4:i64 today_total,
}

service vichele_management {
    bool create_vichele_info(1:string open_id, 2:list<vichele_stay_alone> vichele_info) throws (1:gen_exp e),
    bool delete_vichele_info(1:string open_id, 2:i64 vichele_id) throws (1:gen_exp e),
    bool update_vichele_info(1:string open_id, 2:vichele_stay_alone vichele_info) throws (1:gen_exp e),
    list<vichele_stay_alone> get_created_vichele_info(1:string open_id, 2:i64 ancher, 3:string query_key) throws (1:gen_exp e),
    string silent_login(1:string code) throws (1:gen_exp e),
    bool verify_login(1:string open_id) throws (1:gen_exp e),
    silent_user_info get_silent_user_info(1:string open_id) throws (1:gen_exp e),
    void set_silent_user_info(1:string open_id, 2:silent_user_info info) throws (1:gen_exp e),
    list<string> get_input_history(1:string open_id, 2:vichele_stay_alone search_key) throws (1:gen_exp e),
    list<vichele_stay_alone> get_company_vichele_info(1:string ssid, 2:i64 anchor, 3:i64 status, 4:string enter_date, 5:string stuff_name, 6:string supplier_name, 7:string vichele_number) throws (1:gen_exp e),
    bool confirm_vichele(1:string ssid, 2:list<vichele_stay_alone> info, 3:list<string> company_for_select, 4:bool all_select, 5:string enter_date, 6:string stuff_name, 7:string supplier_name) throws (1:gen_exp e),
    bool cancel_vichele(1:string ssid, 2:list<vichele_stay_alone> info, 3:bool all_select, 4:string enter_date, 5:string stuff_name, 6:string supplier_name) throws (1:gen_exp e),
    bool create_vichele_team(1:string open_id, 2:vichele_team team_info) throws (1:gen_exp e),
    bool update_vichele_team(1:string open_id, 2:vichele_team team_info) throws (1:gen_exp e),
    bool del_vichele_team(1:string open_id, 2:i64 team_id) throws (1:gen_exp e),
    list<vichele_team> get_all_vichele_team(1:string open_id) throws (1:gen_exp e),
    vichele_team get_vichele_team(1:string open_id, 2:i64 team_id) throws (1:gen_exp e),
    bool change_company_name(1:string ssid, 2:i64 vichele_id, 3:string company_name) throws (1:gen_exp e),
    bool fill_company_name(1:string open_id, 2:i64 vichele_id, 3:string company_name) throws (1:gen_exp e),
    list<string> company_history(1:string ssid) throws (1:gen_exp e),
    bool add_supplier(1:string ssid, 2:supplier_basic_info supplier_info) throws (1:gen_exp e),
    bool update_supplier(1:string ssid, 2:supplier_basic_info supplier_info) throws (1:gen_exp e),
    bool del_supplier(1:string ssid, 2:i64 supplier_id) throws (1:gen_exp e),
    list<supplier_basic_info> get_all_supplier(1:string ssid) throws (1:gen_exp e),
    string smart_assign(1:string ssid, 2:list<vichele_stay_alone> vichele_info) throws (1:gen_exp e),
    i64 get_max_vichele_by_supplier(1:string supplier, 2:string company) throws (1:gen_exp e),
    bool fill_tmd(1:string open_id, 2:i64 vichele_id, 3:string tmd_no) throws (1:gen_exp e),
    bool fill_enter_weight(1:string open_id, 2:i64 vichele_id, 3:double enter_weight) throws (1:gen_exp e),
    bool fill_weight_attach(1:string open_id, 2:i64 vichele_id, 3:string weight_attach) throws (1:gen_exp e),
    bool add_exception(1:string ssid, 2:string stuff_name) throws (1:gen_exp e),
    bool del_exception(1:string ssid, 2:string stuff_name) throws (1:gen_exp e),
    list<string> get_all_exceptions(1:string ssid) throws (1:gen_exp e),
    single_vichele_brief get_company_brief(1:string ssid) throws (1:gen_exp e),
    vichele_stay_alone_statistics get_statistics(1:string ssid) throws (1:gen_exp e),
    bool change_price(1:string ssid, 2:list<vichele_stay_alone> info, 3:bool all_select, 4:string enter_date, 5:string stuff_name, 6:string supplier_name, 7:double new_price) throws (1:gen_exp e),
    string get_bound_stuff(1:string company_name, 2:string destination_company) throws (1:gen_exp e),
}