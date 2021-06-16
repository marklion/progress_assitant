<template>
<div class="company_order_show">
    <van-tabs v-model="active" sticky @change="init_orders($store.state.userinfo.buyer)">
        <van-tab v-for="(status_in_map, _si_index) in status_name_map" :key="_si_index" :title="status_in_map.name" :badge="numbers_of_tab(status_in_map.status)">
            <van-row type="flex" justify="center" align="center">
                <van-col :span="20">
                    <van-dropdown-menu>
                        <van-dropdown-item v-model="date_filter" :options="date_option" />
                        <van-dropdown-item v-model="cancel_filter" :options="cancel_option" />
                    </van-dropdown-menu>
                </van-col>
                <van-col :span="4">
                    <van-button type="primary" block :to="{name:'Statistics'}">导出</van-button>
                </van-col>
            </van-row>
            <van-notice-bar left-icon="info-o" :text="'今日计划 ' + company_plan_brief.today_plan_count + '单 ' + company_plan_brief.today_vichele_count + '辆车  明日计划 ' + company_plan_brief.tomorrow_plan_count + '单 ' + company_plan_brief.tomorrow_vichele_count + '辆车'" />
            <van-search v-model="vichele_number_search" label="车牌号" placeholder="请输入车牌号搜索当天计划" @search="search_plan_by_vichele_number" />
            <van-list :immediate-check="false" v-model="lazy_loading" :finished="lazy_finished" finished-text="没有更多了" @load="get_orders_by_ancher">
                <plan-brief v-for="(single_plan, index) in order_need_show" :key="index" :conflict_reason="single_plan.conflict_reason" :plan_id="single_plan.plan_id" :company_view="!$store.state.userinfo.buyer" :status_prompt="single_plan.status_prompt"></plan-brief>
            </van-list>
        </van-tab>
    </van-tabs>
    <export-file :remote_file="export_file_path" v-model="show_export_email"></export-file>
    <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="show_search_result" title="搜索结果">
        <vxe-table size="small" stripe align="center" :data="search_result" max-height="400">
            <vxe-table-column field="plan_info.number" title="计划单号">
                <template #default="{ row }">
                    <van-button plain size="small" type="info" :to="{name:'PlanDetail', params:{plan_id:row.plan_info.id}}">{{row.plan_info.number}}</van-button>
                </template>
            </vxe-table-column>
            <vxe-table-column field="vichele_numbers" title="车号"></vxe-table-column>
            <vxe-table-column field="plan_time" title="计划进厂时间"></vxe-table-column>
            <vxe-table-column field="status" title="状态"></vxe-table-column>
        </vxe-table>
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
import {
    List
} from 'vant';
import {
    Search
} from 'vant';
import {
    NoticeBar
} from 'vant';

Vue.use(NoticeBar);
Vue.use(Search);
Vue.use(List);
Vue.use(Field);
Vue.use(Dialog);
Vue.use(Button);
Vue.use(Col);
Vue.use(Row);
Vue.use(DropdownMenu);
Vue.use(DropdownItem);
Vue.use(Tab);
Vue.use(Tabs);
import PlanBrief from '../components/PlanBrief.vue'
import ExportFile from '../components/ExportFile.vue'
export default {
    name: 'CompanyOrder',
    data: function () {
        return {
            company_plan_brief: {
                today_plan_count: 0,
                tomorrow_plan_count: 0,
                today_vichele_count: 0,
                tomorrow_vichele_count: 0,
            },
            show_search_result: false,
            vichele_number_search: '',
            lazy_loading: false,
            lazy_finished: false,
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
            cancel_option: [{
                text: '只看有效计划',
                value: 0,
            }, {
                text: '查看所有计划',
                value: 1,
            }],
            date_filter: 0,
            show_export_email: false,
            export_file_path: '',
            cancel_filter: 0,
            search_result: [],
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
            var cancel_filter_ret = filter_ret;
            filter_ret = [];
            cancel_filter_ret.forEach(element => {
                if (vue_this.cancel_filter == 1) {
                    filter_ret.push(element);
                } else if (!element.is_cancel) {
                    filter_ret.push(element);
                }
            });
            return filter_ret;
        },
    },
    components: {
        "plan-brief": PlanBrief,
        "export-file": ExportFile,
    },
    methods: {
        search_plan_by_vichele_number: function () {
            var vue_this = this;
            vue_this.search_result = [];
            vue_this.$call_remote_process("stuff_plan_management", "search_plan_by_vichele_number", [vue_this.$cookies.get('pa_ssid'), vue_this.vichele_number_search]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.search_result, index, element);
                });
                vue_this.show_search_result = true;
            });
        },
        get_orders_by_ancher: function () {
            var vue_this = this;
            if (vue_this.orders.length <= 0) {
                return;
            }
            var func = "get_company_plan";
            if (vue_this.$store.state.userinfo.buyer) {
                func = "get_created_plan";
            }
            vue_this.$call_remote_process("stuff_plan_management", func, [vue_this.$cookies.get('pa_ssid'), vue_this.orders.length]).then(function (resp) {
                resp.forEach(element => {
                    vue_this.orders.push(element)
                });
                if (resp.length < 15) {
                    vue_this.lazy_finished = true;
                }
                vue_this.lazy_loading = false;
            });
        },
        init_orders: function (_is_buyer) {
            var vue_this = this;
            var func = "get_company_plan";
            if (_is_buyer) {
                func = "get_created_plan";
            }
            vue_this.$call_remote_process("stuff_plan_management", func, [vue_this.$cookies.get('pa_ssid'), 0]).then(function (resp) {
                vue_this.orders = [];
                var some_plan_id = 0;
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.orders, index, element);
                    some_plan_id = element.plan_id;
                });
                if (resp.length < 15) {
                    vue_this.lazy_finished = true;
                } else {
                    vue_this.lazy_finished = false;
                }

                if (0 != some_plan_id) {
                    vue_this.$call_remote_process("stuff_plan_management", "get_status_rule", [some_plan_id]).then(function (resp) {
                        vue_this.status_name_map = [{
                            name: "全部",
                            status: -1
                        }];
                        resp.forEach(element => {
                            vue_this.status_name_map.push({
                                name: element.prompt,
                                status: element.index
                            });
                        });
                    });

                }
            });
            vue_this.$call_remote_process("stuff_plan_management", "get_company_brief", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.company_plan_brief = resp;
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

    }
}
</script>

<style>

</style>
