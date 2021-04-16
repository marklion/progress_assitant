<template>
<div class="company_order_show">
    <van-tabs v-model="active" sticky>
        <van-tab v-for="(status_in_map, _si_index) in status_name_map" :key="_si_index" :title="status_in_map.name" :badge="numbers_of_tab(status_in_map.status)">
            <van-row type="flex" justify="center" align="center">
                <van-col :span="20">
                    <van-dropdown-menu>
                        <van-dropdown-item v-model="date_filter" :options="date_option" />
                    </van-dropdown-menu>
                </van-col>
                <van-col :span="4">
                    <van-button type="primary" block @click="show_export_plan(order_need_show)">导出</van-button>
                </van-col>
            </van-row>
            <plan-brief v-for="(single_plan, index) in order_need_show" :key="index" :plan_id="single_plan.plan_id" :company_view="!$store.state.userinfo.buyer"></plan-brief>
        </van-tab>
    </van-tabs>
    <van-dialog v-model="ask_email_diag" title="请输入邮箱" show-cancel-button @confirm="export_plan">
        <van-field v-model="export_email" label="邮箱" placeholder="请输入邮箱" />
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Tab,
    Tabs
} from 'vant';
import {
    DropdownMenu,
    DropdownItem
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Button
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Field
} from 'vant';

Vue.use(Field);
// 全局注册
Vue.use(Dialog);
Vue.use(Button);
Vue.use(Col);
Vue.use(Row);
Vue.use(DropdownMenu);
Vue.use(DropdownItem);
Vue.use(Tab);
Vue.use(Tabs);
import PlanBrief from '../components/PlanBrief.vue'
export default {
    name: 'CompanyOrder',
    data: function () {
        return {
            orders: [],
            active: 0,
            numbers_of_tab: function (_status) {
                var ret = 0;
                if (_status < 4) {
                    this.orders.forEach((element) => {
                        if (element.status == _status) {
                            ret++;
                        }
                    });

                }
                if (ret > 0)
                    return ret;
                else
                    return ''
            },
            status_name_map: [{
                name: '全部',
                status: -1,
            }, {
                name: '待确认',
                status: 0,
            }, {
                name: '待付款',
                status: 1,
            }, {
                name: '待收款',
                status: 2,
            }, {
                name: '待提货',
                status: 3,
            }, {
                name: '已完成',
                status: 4,
            }, {
                name: '已撤销',
                status: 5,
            }],
            date_option: [{
                text: '所有进厂时间',
                value: 0
            }, {
                text: '今日进厂',
                value: 1
            }, {
                text: '明日进厂',
                value: 2
            }, {
                text: '本周进厂',
                value: 3
            }],
            date_filter: 0,
            ask_email_diag: false,
            export_email: '',
            export_list: [],
        }
    },
    computed: {
        order_need_show: function () {
            var ret = [];
            var vue_this = this;
            if (vue_this.active == 0) {
                vue_this.orders.forEach((element) => {
                    if (element.status != 5) {
                        ret.push(element);
                    }
                });
            } else {
                vue_this.orders.forEach((element) => {
                    if (element.status == vue_this.active - 1) {
                        ret.push(element);
                    }
                });
            }
            var filter_ret = [];
            var filter_begin = 0;
            var filter_end = 0
            if (vue_this.date_filter == 0) {
                filter_ret = ret;
            } else {
                ret.forEach((element) => {
                    var tmp_date = new Date();
                    tmp_date.setHours(0);
                    tmp_date.setMinutes(0);
                    tmp_date.setSeconds(0);
                    switch (vue_this.date_filter) {
                        case 1:
                            filter_begin = tmp_date.valueOf() / 1000;
                            tmp_date.setDate(tmp_date.getDate() + 1);
                            filter_end = tmp_date.valueOf() / 1000;
                            break;
                        case 2:
                            tmp_date.setDate(tmp_date.getDate() + 1);
                            filter_begin = tmp_date.valueOf() / 1000;
                            tmp_date.setDate(tmp_date.getDate() + 1);
                            filter_end = tmp_date.valueOf() / 1000;
                            break;
                        case 3:
                            tmp_date.setDate(tmp_date.getDate() - ((tmp_date.getDay() + 6) % 7))
                            filter_begin = tmp_date.valueOf() / 1000;
                            tmp_date.setDate(tmp_date.getDate() + 7);
                            filter_end = tmp_date.valueOf() / 1000;
                            break;
                        default:
                            break;
                    }
                    if (element.plan_time >= filter_begin && element.plan_time <= filter_end) {
                        filter_ret.push(element);
                    }
                });
            }
            return filter_ret;
        },
    },
    components: {
        "plan-brief": PlanBrief,
    },
    methods: {
        show_export_plan: function (_plans) {
            var plan_ids = [];
            _plans.forEach((element) => {
                plan_ids.push(element.plan_id);
            });
            this.export_list = plan_ids;
            this.ask_email_diag = true;
        },
        export_plan: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "export_plan_to_email", [vue_this.$cookies.get('pa_ssid'), vue_this.export_list, vue_this.export_email]).then(function (resp) {
                if (resp) {
                    vue_this.$toast("邮件已发送");
                }
            });
        },
        init_orders: function (_is_buyer) {
            var vue_this = this;
            var func = "get_company_plan";
            if (_is_buyer) {
                func = "get_created_plan";
            }
            vue_this.$call_remote_process("stuff_plan_management", func, [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.orders, index, element);
                });
            });
        },
    },
    watch: {
        "$store.state.userinfo.buyer": function (_val) {
            this.init_orders(_val);
        },
    },
    beforeMount: function () {
        this.init_orders(this.$store.state.userinfo.buyer);
        var vue_this = this;
        vue_this.$call_remote_process("user_management", "get_user_email", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            vue_this.export_email = resp;
        });
    }
}
</script>

<style>

</style>
