struct user_info {
    1:i64 user_id,
    2:string name,
    3:string logo,
    4:string company,
    5:bool buyer,
    6:string phone
}
service user_management{
    user_info get_user_info(1:string ssid),
    string user_login(1:string code),
    bool update_user_info(1:user_info info, 2:string ssid),
    void logff_user(1:string ssid),
    list<string> get_bound_vichele(1:string ssid)
    bool bind_new_vichele(1:string ssid, 2:string vichele),
    void remove_vichele(1:string ssid, 2:string vichele),
    bool update_logo(1:string content, 2:string ssid),
    string get_customer_info(1:i64 user_id),
    bool is_admin(1:string ssid),
}

struct stuff_detail {
    1:string name, 
    2:string last,
    3:i64 price,
    4:string company,
    5:i64 type_id,
}

struct user_apply {
    1:string name, 
    2:string phone,
    3:string logo,
    4:i64 apply_id,
    5:i64 status,
}

service company_management {
    list<i64> get_all_type(1:string ssid),
    i64 add_type(1:string name, 2:i64 price, 3:string ssid),
    bool edit_type(1:stuff_detail stuff, 2:string ssid),
    void remove_type(1:stuff_detail stuff, 2:string ssid),
    list<user_apply> get_all_apply(1:string ssid),
    bool approve_apply(1:i64 apply_id, 2:string ssid, 3:bool approve),
}

service stuff_info {
    list<stuff_detail> get_today(),
    stuff_detail get_stuff_detail(1:i64 type_id),
}

struct stuff_plan {
    1:i64 type_id,
    2:double count,
    3:list<string> vichele_info,
    4:i64 plan_id,
    5:i64 created_by,
    6:string plan_time,
    7:i64 created_time,
    8:string name, 
    9:double price,
    10:i64 status,
}

service stuff_plan_management {
    i64 create_plan(1:stuff_plan plan, 2:string ssid),
    list<i64> get_created_plan(1:string ssid),
    list<i64> get_company_plan(1:string ssid),
    stuff_plan get_plan(1:i64 plan_id),
    bool update_plan(1:stuff_plan plan, 2:string ssid),
    bool confirm_plan(1:i64 plan_id, 2:string ssid, 3:bool confirm),
}
