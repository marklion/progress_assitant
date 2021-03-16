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
    bool remove_user(1:i64 user_id, 2:string ssid)
}

struct stuff_detail {
    1:string name, 
    2:string last,
    3:i64 price,
    4:string company,
    5:i64 type_id,
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
}

service stuff_plan_management {
    i64 create_plan(1:stuff_plan plan, 2:string ssid),
    list<stuff_plan> get_created_plan(1:string ssid),
    stuff_plan get_plan(1:i64 plan_id),
    bool update_plan(1:stuff_plan plan, 2:string ssid),
}
