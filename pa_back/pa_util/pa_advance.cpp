#include "pa_advance.h"
#include "pa_utils.h"

#define SALE_CONFIG_FILE "/conf/data_config.json"

static bool pa_advance_can_access_all(pa_sql_userinfo &_user)
{
    bool ret = false;

    std::ifstream config_file(SALE_CONFIG_FILE, std::ios::in);
    std::istreambuf_iterator<char> beg(config_file), end;
    std::string config_string(beg, end);
    neb::CJsonObject config(config_string);

    auto company = _user.get_parent<pa_sql_company>("belong_company");
    if (company)
    {
        for (size_t i = 0; i < config.GetArraySize(); i++)
        {
            auto company_config = config[i];
            if (company_config("name") == company->name)
            {
                if (company_config("can_access_all") == "true")
                {
                    ret = true;
                }
                break;
            }
        }
    }

    return ret;
}

typedef bool (*permission_handler)(pa_sql_userinfo &_user);

static permission_handler g_all_handler[] = {
    [PA_ADVANCE_NOTHING] = nullptr,
    [PA_ADVANCE_GET_ALL_ACCESS_RECORD] = pa_advance_can_access_all,
};

bool pa_advance_has_permission(pa_sql_userinfo &_user, pa_advance_type _type)
{
    return g_all_handler[_type](_user);
}