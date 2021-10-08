<template>
<div class="company_extra_vichele_show">
    <van-tabs v-model="active" @change="refresh_all_records">
        <van-tab title="未确认"></van-tab>
        <van-tab title="已确认"></van-tab>
        <van-tab title="已完成"></van-tab>
    </van-tabs>
    <van-dropdown-menu>
        <van-dropdown-item v-model="date_filter" :options="date_condition" @change="refresh_all_records" />
        <van-dropdown-item v-model="stuff_filter" :options="stuff_condition" @change="refresh_all_records" />
        <van-dropdown-item v-model="supplier_filter" :options="supplier_condition" @change="refresh_all_records" />
    </van-dropdown-menu>
    <van-search v-model="vichele_search_filter" placeholder="过滤车号" @input="refresh_all_records" />
    <van-sticky>
        <van-row type="flex" align="center" v-if="select_pool.length != 0">
            <van-col :span="8">
                <van-button v-if="select_pool.length != items_need_show.length" block type="info" @click="select_pool = items_need_show">全选</van-button>
                <van-button v-else block type="info" @click="select_pool = []">取消全选</van-button>
            </van-col>
            <van-col :span="8">
                <van-button block type="primary" @click="show_confirm_diag = true">确认车辆</van-button>
            </van-col>
            <van-col :span="8">
                <van-button block type="danger" @click="cancel_vichele">取消车辆</van-button>
            </van-col>
        </van-row>
    </van-sticky>
    <van-checkbox-group v-model="select_pool">
        <van-list v-model="loading" :finished="finished" finished-text="没有更多了" @load="onLoad" ref="all_record">
            <div v-for="(single_vichele, index) in items_need_show" :key="index" class="one_record_show">
                <van-cell center :label="single_vichele.stuff_name + '(' + (single_vichele.company_name?single_vichele.company_name:'未指定') + ')'">
                    <template #title>
                        <div>{{single_vichele.main_vichele_number}}</div>
                        <div>{{single_vichele.behind_vichele_number}}</div>
                    </template>
                    <div>单价：{{single_vichele.price}}</div>
                    <div>发货净重：{{single_vichele.count}}吨</div>
                    <div v-if="single_vichele.status == 2">收货净重：{{single_vichele.j_weight}}吨</div>
                    <div v-if="single_vichele.comment">备注：{{single_vichele.comment}}</div>
                    <div v-if="single_vichele.repeated">
                        <van-tag plain type="warning">多次进厂</van-tag>
                    </div>
                    <template #icon>
                        <div style="margin-right: 10px;">
                            <van-checkbox :name="single_vichele"></van-checkbox>
                        </div>
                    </template>
                </van-cell>
                <van-divider>司机信息</van-divider>
                <van-cell :title="single_vichele.driver_name" :value="single_vichele.driver_phone" :label="single_vichele.driver_id"></van-cell>
                <div class="extra_info_show">
                    <van-row type="flex" align="center" :gutter="10">
                        <van-col>
                            <van-tag plain type="primary">进厂时间:{{single_vichele.date}}</van-tag>
                        </van-col>
                        <van-col>
                            <van-tag plain type="warning">{{single_vichele.creator_name}} : {{single_vichele.creator_phone}}</van-tag>
                        </van-col>
                        <van-col>
                            <van-tag v-if="single_vichele.status == 0" plain type="danger">未确认</van-tag>
                            <van-tag v-else-if="single_vichele.status == 1" plain type="success">已确认</van-tag>
                            <van-tag v-else plain type="primary">已完成</van-tag>
                        </van-col>
                        <van-col v-if="single_vichele.status == 1">
                            <van-button type="primary" size="mini" @click="change_company_name(single_vichele.id)">改派</van-button>
                        </van-col>
                    </van-row>
                    <van-row v-if="single_vichele.status == 2" type="flex" align="center" :gutter="10">
                        <van-col>皮重</van-col>
                        <van-col>{{single_vichele.p_weight}}吨</van-col>
                        <van-col>毛重</van-col>
                        <van-col>{{single_vichele.m_weight}}吨</van-col>
                    </van-row>
                </div>
            </div>
        </van-list>
    </van-checkbox-group>
    <van-dialog v-model="show_confirm_diag" close-on-click-overlay title="是否改派" :show-confirm-button="false">
        <van-form @submit="confirm_vichele">
            <van-field v-if="!catch_free" v-model="smart_company" label="改派地点" placeholder="点击智能改派获取最佳提货地">
                <template #button>
                    <van-button native-type="button" type="primary" size="small" @click="smart_assign">智能改派</van-button>
                </template>
            </van-field>
            <van-field name="switch" label="自由拉货">
                <template #input>
                    <van-switch v-model="catch_free" size="20" />
                </template>
            </van-field>
            <van-field name="checkboxGroup" label="提货公司池" v-if="catch_free">
                <template #input>
                    <van-checkbox-group v-model="company_selected">
                        <van-checkbox v-for="(single_company_name, index) in company_for_select" :key="index" :name="single_company_name">{{single_company_name}}</van-checkbox>
                    </van-checkbox-group>
                </template>
            </van-field>
            <van-button round block>确认</van-button>
        </van-form>
    </van-dialog>
    <van-dialog v-model="change_diag_show" close-on-click-overlay title="改派" :show-confirm-button="false">
        <van-form @submit="change_company_name_2_server">
            <history-input search_key="company_name" v-model="new_company_name" :rules="[{ required: true, message: '请填写公司名' }]"></history-input>
            <van-button round block>确认</van-button>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    List
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Tag
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Checkbox,
    CheckboxGroup
} from 'vant';
import {
    Button
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    DropdownMenu,
    DropdownItem
} from 'vant';
import {
    Search
} from 'vant';
import PinyinMatch from 'pinyin-match';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Switch
} from 'vant';
import HistoryInput from '../components/HistoryInput.vue'
import {
    Divider
} from 'vant';
import {
    Sticky
} from 'vant';
import {
    Tab,
    Tabs
} from 'vant';

Vue.use(Tab);
Vue.use(Tabs);
Vue.use(Sticky);
Vue.use(Divider);
Vue.use(Switch);
Vue.use(Form);
Vue.use(Field);
Vue.use(Search);
Vue.use(DropdownMenu);
Vue.use(DropdownItem);
Vue.use(Button);
Vue.use(Checkbox);
Vue.use(CheckboxGroup);
Vue.use(Col);
Vue.use(Row);
Vue.use(Tag);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(List);
Vue.use(Dialog);
export default {
    name: 'CompanyExtraVichele',
    components: {
        "history-input": HistoryInput,
    },
    data: function () {
        return {
            active: 0,
            smart_company: '',
            new_company_name: '',
            change_diag_show: false,
            change_id: 0,
            company_for_select: [],
            company_selected: [],
            catch_free: false,
            show_confirm_diag: false,
            vichele_search_filter: '',
            items: [],
            loading: false,
            finished: false,
            select_pool: [],
            date_filter: -2,
            date_condition: [{
                text: '所有进厂日期',
                value: -2,
            }, {
                text: '昨日进厂',
                value: -1,
            }, {
                text: '今日进厂',
                value: 0,
            }, {
                text: '明日进厂',
                value: 1,
            }],
            stuff_filter: -1,
            stuff_condition: [{
                text: '所有货物',
                value: -1,
            }, ],
            supplier_filter: -1,
            supplier_condition: [{
                text: '所有供应商',
                value: -1,
            }, ],
        };
    },
    computed: {
        items_need_show: function () {
            var ret = [];
            var vue_this = this;

            this.items.forEach(element => {
                if (vue_this.vichele_search_filter.length == 0) {
                    ret.push(element);
                } else {
                    if (PinyinMatch.match(element.main_vichele_number, vue_this.vichele_search_filter) ||
                        PinyinMatch.match(element.behind_vichele_number, vue_this.vichele_search_filter)) {
                        ret.push(element);
                    }
                }
            });

            return ret;
        },
    },
    methods: {
        smart_assign: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "smart_assign", [vue_this.$cookies.get('pa_ssid'), vue_this.select_pool]).then(function (resp) {
                vue_this.smart_company = resp;
            });
        },
        change_company_name_2_server: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "change_company_name", [vue_this.$cookies.get('pa_ssid'), vue_this.change_id, vue_this.new_company_name]).then(function (resp) {
                if (resp) {
                    vue_this.change_diag_show = false;
                    vue_this.change_id = 0;
                    vue_this.new_company_name = "";
                    vue_this.finished = false;
                    vue_this.items = [];
                    vue_this.select_pool = [];
                    vue_this.$refs.all_record.check();
                }
            });
        },
        change_company_name: function (_id) {
            this.change_id = _id;
            this.change_diag_show = true;
        },
        refresh_all_records: function () {
            this.finished = false;
            this.items = [];
            this.$refs.all_record.check();
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            return y + '-' + m + '-' + d;
        },
        is_all_select: function () {
            var ret = false;
            if (this.select_pool.length == this.items_need_show.length && !this.finished) {
                ret = true;
            }

            return ret;
        },
        cancel_vichele: function () {
            var vue_this = this;
            Dialog.confirm({
                    title: '确认取消',
                    message: '确认取消所选的车辆进厂吗？',
                })
                .then(() => {
                    vue_this.$call_remote_process("vichele_management", 'cancel_vichele', [vue_this.$cookies.get('pa_ssid'), vue_this.select_pool, vue_this.is_all_select()]).finally(function () {
                        vue_this.finished = false;
                        vue_this.items = [];
                        vue_this.select_pool = [];
                        vue_this.$refs.all_record.check();

                    });
                });

        },
        confirm_vichele: function () {
            var vue_this = this;
            vue_this.select_pool.forEach(element => {
                if (vue_this.catch_free) {
                    element.company_name = "";
                } else if (vue_this.smart_company) {
                    element.company_name = vue_this.smart_company;
                }
            });
            vue_this.$call_remote_process("vichele_management", 'confirm_vichele', [vue_this.$cookies.get('pa_ssid'), vue_this.select_pool, vue_this.company_selected, vue_this.is_all_select()]).then(function (resp) {
                if (resp) {
                    vue_this.show_confirm_diag = false;
                    vue_this.finished = false;
                    vue_this.items = [];
                    vue_this.select_pool = [];
                    vue_this.catch_free = false;
                    vue_this.company_selected = [];
                    vue_this.$refs.all_record.check();
                }
            });
        },
        onLoad: function () {
            var vue_this = this;
            var status = vue_this.active;
            var cur_date = new Date();
            cur_date.setDate(cur_date.getDate() + vue_this.date_filter)
            var date_condition = '';
            if (vue_this.date_filter != -2) {
                date_condition = vue_this.formatDateTime(cur_date);
            }
            var stuff_condition = '';
            if (vue_this.stuff_filter != -1) {
                stuff_condition = vue_this.stuff_condition.find(value => {
                    return value.value == vue_this.stuff_filter
                }).text;
            }
            var supplier_condition = '';
            if (vue_this.supplier_filter != -1) {
                supplier_condition = vue_this.supplier_condition.find(value => {
                    return value.value == vue_this.supplier_filter
                }).text;
            }

            vue_this.$call_remote_process("vichele_management", 'get_company_vichele_info', [vue_this.$cookies.get('pa_ssid'), vue_this.items.length, status, date_condition, stuff_condition, supplier_condition, '']).then(function (resp) {
                resp.forEach(element => {
                    vue_this.items.push(element);
                });
                if (resp.length < 15) {
                    vue_this.finished = true;
                }
                vue_this.loading = false;
            });
        },
        init_company_for_select: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "company_history", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.company_for_select = [];
                resp.forEach(element => {
                    if (element) {
                        vue_this.company_for_select.push(element);
                    }
                });
            });
        },
        init_brief_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "get_company_brief", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.stuff_names.forEach(element => {
                    vue_this.stuff_condition.push({
                        text: element,
                        value: vue_this.stuff_condition.length
                    });
                });
                resp.supplier_names.forEach(element => {
                    vue_this.supplier_condition.push({
                        text: element.length > 0 ? element : '未指定',
                        value: vue_this.supplier_condition.length
                    });
                });
            });
        },
    },
    beforeMount: function () {
        this.init_company_for_select();
        this.init_brief_data();
    },
}
</script>

<style scoped>
.extra_info_show {
    margin-left: 15px;
}

.one_record_show {
    border: 1px solid gray;
    border-radius: 15px;
    padding-top: 8px;
    padding-bottom: 8px;
    margin-bottom: 10px;
}
</style>
