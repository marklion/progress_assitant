<template>
<div class="company_order_show">
    <van-tabs v-model="active" sticky @change="tab_change">
        <van-tab v-for="(status_in_map, _si_index) in status_name_map" :key="_si_index" :title="status_in_map.name" :badge="status_in_map.count">
            <van-row type="flex" justify="center" align="center">
                <van-col :span="20">
                    <van-dropdown-menu>
                        <van-dropdown-item v-model="date_filter" :options="date_option" @close="recheck_list" />
                        <van-dropdown-item v-model="cancel_filter" :options="cancel_option" @change="recheck_list" />
                    </van-dropdown-menu>
                    <van-dropdown-menu>
                        <van-dropdown-item v-model="company_filter" :options="company_option" @change="recheck_list" />
                        <van-dropdown-item v-model="stuff_type_filter" :options="stuff_type_option" @change="recheck_list" />
                    </van-dropdown-menu>
                </van-col>
                <van-col :span="4">
                    <van-button type="primary" block :to="{name:'Statistics'}">导出</van-button>
                </van-col>
            </van-row>
            <van-notice-bar left-icon="info-o" :text="'今日计划 ' + company_plan_brief.today_plan_count + '单 ' + company_plan_brief.today_vichele_count + '辆车  明日计划 ' + company_plan_brief.tomorrow_plan_count + '单 ' + company_plan_brief.tomorrow_vichele_count + '辆车'" />
            <van-search v-model="vichele_number_search" label="车牌号" placeholder="请输入车牌号搜索当天计划" @search="search_plan_by_vichele_number" />
            <div v-if="show_export">
                <van-row v-if="$store.state.userinfo.buyer">
                    <van-col :span="8">
                        <van-button type="info" block @click="select_all_plan">全选</van-button>
                    </van-col>
                    <van-col :span="8">
                        <van-button type="danger" block @click="cancle_select">取消多选</van-button>
                    </van-col>
                    <van-col :span="8">
                        <van-button type="primary" block @click="show_export_prompt">导出所选{{item_need_export.length}}项</van-button>
                    </van-col>
                </van-row>
                <van-row v-else>
                    <van-col :span="6">
                        <van-button type="info" block @click="select_all_plan">全选</van-button>
                    </van-col>
                    <van-col :span="6">
                        <van-button type="danger" block @click="cancle_select">取消多选</van-button>
                    </van-col>
                    <van-col :span="6">
                        <van-button type="primary" block @click="show_export_prompt">导出所选{{item_need_export.length}}项</van-button>
                    </van-col>
                    <van-col :span="6">
                        <van-button type="warning" block @click="multi_confirm">批量确认{{item_need_export.length}}项</van-button>
                    </van-col>
                </van-row>

            </div>
        </van-tab>
    </van-tabs>
    <van-list ref="order_list" v-model="lazy_loading" :finished="lazy_finished" finished-text="没有更多了" @load="get_orders_by_ancher">
        <plan-brief @select_trigger="proc_select" ref="single_plan" v-for="(single_plan, index) in order_need_show" :key="index" :conflict_reason="single_plan.conflict_reason" :plan_id="single_plan.plan_id" :company_view="!$store.state.userinfo.buyer" :status_prompt="single_plan.status_prompt"></plan-brief>
    </van-list>
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

    <export-file :remote_file="download_url" v-model="show_export_file"></export-file>
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
            download_url: '',
            show_export_file: false,
            show_export: false,
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
                count: '',
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
            stuff_type_option: [{
                text: '所有货品',
                value: 0,
            }],
            company_option: [{
                text: '所有公司',
                value: 0,
            }],
            cancel_option: [{
                text: '只看有效计划',
                value: 0,
            }, {
                text: '查看所有计划',
                value: 1,
            }],
            date_filter: 0,
            stuff_type_filter: 0,
            company_filter: 0,
            show_export_email: false,
            export_file_path: '',
            cancel_filter: 0,
            search_result: [],
            item_need_export: [],
            item_selected: function () {
                var ret = [];
                if (this.$refs.single_plan) {
                    this.$refs.single_plan.forEach(element => {
                        if (element.current_is_selected()) {
                            if (ret.indexOf(element.get_plan_id()) == -1) {
                                ret.push(element.get_plan_id());
                            }
                        }
                    });
                }
                this.item_need_export = ret;
            },
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
            var stuff_filter_ret = filter_ret;
            filter_ret = [];
            stuff_filter_ret.forEach(element => {
                if (vue_this.stuff_type_filter == 0) {
                    filter_ret.push(element);
                } else {
                    if (vue_this.stuff_type_option.find(itr => {
                            return itr.value == vue_this.stuff_type_filter
                        }).text == element.stuff_type) {
                        filter_ret.push(element);
                    }
                }
            });
            var company_fitler_ret = filter_ret;
            filter_ret = [];
            company_fitler_ret.forEach(element => {
                if (vue_this.company_filter == 0) {
                    filter_ret.push(element);
                } else {
                    if (vue_this.company_option.find(itr => {
                            return itr.value == vue_this.company_filter
                        }).text == element.company) {
                        filter_ret.push(element);
                    }
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
        proc_select: function () {
            this.item_selected();
            if (this.item_need_export.length > 0) {
                this.show_export = true;
            } else {
                this.show_export = false;
            }
        },
        select_all_plan: function () {
            this.$refs.single_plan.forEach(element => {
                element.select_it();
            });
        },
        cancle_select: function () {
            this.$refs.single_plan.forEach(element => {
                element.unselect_it();
            });
        },

        show_export_prompt: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "export_plan", [vue_this.$cookies.get('pa_ssid'), vue_this.item_need_export]).then(function (resp) {
                vue_this.download_url = vue_this.$remote_url + resp;
                vue_this.show_export_file = true;
            });
        },
        multi_confirm: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "multi_confirm_plan", [vue_this.$cookies.get('pa_ssid'), vue_this.item_need_export]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },
        tab_change: function () {
            this.recheck_list();
        },
        recheck_list: function () {
            this.lazy_finished = false;
            this.orders = [];
            this.$refs.order_list.check();
        },
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
        get_orders_by_ancher: function (_init) {
            var vue_this = this;
            var func = "get_company_plan";
            if (vue_this.$store.state.userinfo.buyer) {
                func = "get_created_plan";
            }
            var stuff_name = vue_this.stuff_type_option[vue_this.stuff_type_filter].text;
            if (vue_this.stuff_type_filter == 0) {
                stuff_name = "";
            }
            var company_search = vue_this.company_option[vue_this.company_filter].text;
            if (vue_this.company_filter == 0) {
                company_search = "";
            }
            vue_this.$call_remote_process("stuff_plan_management", func, [vue_this.$cookies.get('pa_ssid'), vue_this.orders.length, vue_this.status_name_map[vue_this.active].status, stuff_name, company_search]).then(function (resp) {
                if (_init && _init == true) {
                    vue_this.orders = [];
                    vue_this.lazy_finished = false;
                }
                resp.forEach(element => {
                    vue_this.orders.push(element)
                });
                if (resp.length < 15) {
                    vue_this.lazy_finished = true;
                }
                vue_this.lazy_loading = false;
            });
        },
        init_orders: function () {
            var vue_this = this;
            vue_this.orders = [];
            vue_this.$call_remote_process("stuff_plan_management", "get_status_rule", [1]).then(function (resp) {
                vue_this.status_name_map = [{
                    name: "全部",
                    status: -1,
                    count: '',
                }];
                resp.forEach(element => {
                    if (element.index < 4) {
                        vue_this.$call_remote_process("stuff_plan_management", 'get_count_by_status', [vue_this.$cookies.get('pa_ssid'), element.index]).then(function (resp) {
                            var status_element = vue_this.status_name_map.find(single_element => {
                                return single_element.status == element.index;
                            });
                            status_element.count = resp > 0 ? resp : '';
                        });
                    }
                    vue_this.status_name_map.push({
                        name: element.prompt,
                        status: element.index,
                        count: '',
                    });
                });
            });

            vue_this.$call_remote_process("stuff_plan_management", "get_company_brief", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.company_plan_brief = resp;
            });
        },
    },
    watch: {
        "$store.state.userinfo.buyer": function () {
            this.lazy_finished = false;
            this.orders = [];
            this.get_orders_by_ancher(true);
        },
    },
    beforeMount: function () {
        this.init_orders();
        var vue_this = this;
        vue_this.$call_remote_process("stuff_info", 'get_related_stuff', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            resp.forEach(element => {
                vue_this.stuff_type_option.push({
                    text: element,
                    value: vue_this.stuff_type_option.length
                });
            });
        });

        vue_this.$call_remote_process("company_management", 'get_related_company', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            resp.forEach(element => {
                vue_this.company_option.push({
                    text: element,
                    value: vue_this.company_option.length
                });
            });
        });
    },
    activated() {
        const scrollTop = this.$route.meta.scrollTop;
        const $content = document.querySelector('.content');
        if (scrollTop && $content) {
            $content.scrollTop = scrollTop;
        }
    },
}
</script>

<style scoped>
.multi_button {
    width: 100%;
}
</style>
