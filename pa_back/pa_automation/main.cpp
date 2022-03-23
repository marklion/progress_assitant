#include <iostream>
#include "pa_user_suite.h"
#include "pa_company_suite.h"
#include "pa_stuff_plan_suite.h"
int main(int argc, char const *argv[])
{
    pa_test_framework f;
    // f.m_all["user management"] = new pa_user_suite();
    f.m_all["company management"] = new pa_company_suite();
    f.m_all["stuff plan"] = new pa_stuff_plan_suite();
    f.run_test();
    return 0;
}
