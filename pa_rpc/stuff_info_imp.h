#include "gen_code/stuff_info.h"

class stuff_info_handler: virtual public stuff_infoIf {
public:
    virtual void get_today(std::vector<stuff_detail> & _return)
    {
        stuff_detail tmp;
        tmp.company = "汇能煤化工";
        tmp.last = "充足";
        tmp.name = "LNG";
        tmp.price = 3000;
        tmp.type_id = 1;
        _return.push_back(tmp);
    }
    virtual void get_stuff_detail(stuff_detail &_return, const int64_t type_id)
    {
        if (type_id == 1)
        {
            _return.company = "汇能煤化工";
            _return.last = "充足";
            _return.name = "LNG";
            _return.price = 3000;
            _return.type_id = 1;
        }
    }
};