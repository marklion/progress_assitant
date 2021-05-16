#include <iostream>
#include "pa_user_suite.h"
int main(int argc, char const *argv[])
{
    pa_test_framework f;
    f.m_all["user management"] = new pa_user_suite();
    f.run_test();
    return 0;
}
