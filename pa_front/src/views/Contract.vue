<template>
<div class="contract_show">
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>当前合同</van-col>
                <van-col v-if="!$store.state.userinfo.buyer">
                    <van-button size="small" type="primary" @click="add_contract_show = true">增加</van-button>
                </van-col>
            </van-row>

            <van-search v-model="access_search_key" placeholder="公司名\拼音首字母" />
        </template>
        <div class="single_contract_show" v-for="(single_contract, index) in contract_after_filter" :key="index">
            <van-cell center :label="'合同编号：' + single_contract.number">
                <template #title>
                    <span>{{my_side(single_contract)}}</span>
                    <span v-if="!$store.state.userinfo.buyer && single_contract.customer_code">({{single_contract.customer_code}})</span>
                    <span class="status_show">
                        <van-tag :type="contract_status[single_contract.status].type">{{contract_status[single_contract.status].text}}</van-tag>
                    </span>
                </template>
                <template #right-icon v-if="!$store.state.userinfo.buyer">
                    <div class="del_show">
                        <div>
                            <van-button plain size="small" type="info" icon="edit" @click="show_update_contract(single_contract)"></van-button>
                        </div>
                        <div>
                            <van-button plain size="small" type="danger" icon="delete-o" @click="pre_ask_del(single_contract)"></van-button>
                        </div>
                    </div>
                </template>
                <div>开始日期：{{single_contract.start_time}}</div>
                <div>到期日期：{{single_contract.end_time}}</div>
                <div v-if="!$store.state.userinfo.buyer">余额：{{single_contract.balance}}</div>
            </van-cell>
            <div class="follow_status_show">
                <van-row type="flex" :gutter="5" align="center">
                    <van-col>已关注：</van-col>
                    <van-col v-for="(single_stuff, stuff_index) in single_contract.follow_stuff" :key="stuff_index">
                        <van-tag plain type="primary">{{single_stuff}}</van-tag>
                    </van-col>
                    <van-col v-if="!$store.state.userinfo.buyer">
                        <van-button size="small" type="primary" icon="plus" round @click="focus_company = single_contract.a_side_company;popover_switch = true"></van-button>
                    </van-col>
                    <van-col v-if="!$store.state.userinfo.buyer">
                        <van-button size="small" type="danger" icon="minus" round @click="focus_company = single_contract.a_side_company;popover_switch_off = true"></van-button>
                    </van-col>
                </van-row>

            </div>
        </div>
    </van-cell-group>
    <van-action-sheet v-model="popover_switch" :actions="actions" @select="onSelect" />
    <van-action-sheet v-model="popover_switch_off" :actions="actions" @select="onSelect_off" />
    <van-dialog v-model="add_contract_show" title="添加合同" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="add_contract">
            <van-field v-model="submit_contract.a_side_company" name="客户" label="客户" placeholder="请输入客户公司名" :rules="[{ required:true, message:'请输入客户公司'}]" />
            <van-field v-model="submit_contract.number" name="编号" label="合同编号" placeholder="请输入合同编号" :rules="[{ required:true, message:'请输入合同编号'}]" />
            <van-field name="calendar1" v-model="submit_contract.start_time" label="开始日期" placeholder="请输入开始日期yyyy/mm/dd" format-trigger="onBlur" :formatter="formatter_input_date" :rules="[{ required:true, message:'请输入开始日期'}]" />
            <van-field name="calendar2" v-model="submit_contract.end_time" label="到期日期" placeholder="请输入到期日期yyyy/mm/dd" format-trigger="onBlur" :formatter="formatter_input_date" :rules="[{ required:true, message:'请输入到期日期'}]" />
            <van-field v-model="submit_contract.customer_code" name="客户编码" label="客户编码(可选)" placeholder="请输入客户编码" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
    <van-dialog v-model="update_contract_show" title="修改合同" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="update_contract">
            <van-field v-model="submit_contract.a_side_company" name="客户" label="客户" placeholder="请输入客户公司名" :rules="[{ required:true, message:'请输入客户公司'}]" />
            <van-field v-model="submit_contract.number" name="编号" label="合同编号" placeholder="请输入合同编号" :rules="[{ required:true, message:'请输入合同编号'}]" />
            <van-field name="calendar1" v-model="submit_contract.start_time" label="开始日期" placeholder="请输入开始日期yyyy/mm/dd" format-trigger="onBlur" :formatter="formatter_input_date" :rules="[{ required:true, message:'请输入开始日期'}]" />
            <van-field name="calendar2" v-model="submit_contract.end_time" label="到期日期" placeholder="请输入到期日期yyyy/mm/dd" format-trigger="onBlur" :formatter="formatter_input_date" :rules="[{ required:true, message:'请输入到期日期'}]" />
            <van-field v-model="submit_contract.customer_code" name="客户编码" label="客户编码(可选)" placeholder="请输入客户编码" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
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
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Calendar
} from 'vant';
import {
    Tag
} from 'vant';
import {
    Popover
} from 'vant';
import {
    ActionSheet
} from 'vant';
import {
    Search
} from 'vant';

Vue.use(Search);
import PinyinMatch from 'pinyin-match';
Vue.use(ActionSheet);
Vue.use(Popover);
Vue.use(Tag);
Vue.use(Calendar);
Vue.use(Form);
Vue.use(Field);
Vue.use(Dialog);
Vue.use(Button);
Vue.use(Col);
Vue.use(Row);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'Contract',
    computed: {
        contract_after_filter: function () {
            var ret = [];
            var vue_this = this;
            if (vue_this.access_search_key.length <= 0) {
                ret = vue_this.contract;
            } else {
                vue_this.contract.forEach(element => {
                    if (PinyinMatch.match(element.a_side_company, vue_this.access_search_key) || PinyinMatch.match(element.b_side_company, vue_this.access_search_key)) {
                        ret.push(element);
                    }
                });
            }

            return ret;
        },
    },
    data: function () {
        return {
            access_search_key: '',
            actions: [],
            popover_switch: false,
            popover_switch_off: false,
            submit_contract: {
                number: '',
                a_side_company: '',
                start_time: '',
                end_time: '',
                customer_code: '',
            },
            add_contract_show: false,
            update_contract_show: false,
            contract: [],
            my_side: function (single_contract) {
                var ret = single_contract.a_side_company;
                if (this.$store.state.userinfo.buyer) {
                    ret = single_contract.b_side_company;
                }
                return ret;
            },
            min_date: new Date(),
            max_date: new Date(),
            contract_status: [{
                text: '正常',
                type: "success",
            }, {
                text: '即将到期',
                type: "warning",
            }, {
                text: '过期',
                type: "danger",
            }, {
                text: '未签',
                type: "danger",
            }, ],
            focus_company: '',
        };
    },
    methods: {
        pre_ask_del: function (_contract) {
            var vue_this = this;
            Dialog({
                closeOnClickOverlay: true,
                title: '删除合同',
                message: '确定要删除 ' + _contract.a_side_company + ' 吗'
            }).then(function () {
                vue_this.del_contract(_contract.id);
            });
        },
        show_update_contract: function (_contract) {
            this.submit_contract = _contract;
            this.update_contract_show = true;
        },
        update_contract: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "update_contract", [vue_this.$cookies.get('pa_ssid'), vue_this.submit_contract]).then(function (resp) {
                if (resp) {
                    vue_this.init_contract_data();
                    vue_this.update_contract_show = false;
                }
            });
        },
        formatter_input_date: function (_value) {
            if (_value.length <= 0) {
                return _value;
            }
            return this.formatDateTime(new Date(_value));
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            return y + '-' + m + '-' + d;
        },
        del_contract: function (_id) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "del_contract", [vue_this.$cookies.get('pa_ssid'), _id]).then(function () {
                vue_this.init_contract_data();
            });
        },
        add_contract: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "add_contract", [vue_this.$cookies.get('pa_ssid'), vue_this.submit_contract]).then(function (resp) {
                if (resp) {
                    vue_this.init_contract_data();
                    vue_this.add_contract_show = false;
                }
            });
        },
        init_contract_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_all_contract", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.contract = [];
                resp.forEach((element, index) => {
                    var single_contract = element;
                    single_contract.follow_stuff = [];
                    vue_this.$call_remote_process("stuff_info", "get_follow_stuff_by_company", [element.a_side_company]).then(function (resp) {
                        resp.forEach(element => {
                            single_contract.follow_stuff.push(element.name);
                        });
                        vue_this.$set(vue_this.contract, index, single_contract);
                    });
                });
            });
            var cur_date = new Date();
            vue_this.min_date.setFullYear(cur_date.getFullYear() - 1);
            vue_this.max_date.setFullYear(cur_date.getFullYear() + 3);
        },
        get_type_detail: function (_id) {
            var vue_this = this;
            this.$call_remote_process("stuff_info", 'get_stuff_detail', [_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.$set(vue_this.actions, vue_this.actions.length, {
                    name: resp.name,
                    type_id: resp.type_id
                });
            });
        },
        onSelect: function (_action) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_info", 'add_company_follow_stuff', [vue_this.focus_company, _action.type_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_contract_data();
                    vue_this.popover_switch = false;
                }
            });
        },
        onSelect_off: function (_action) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_info", 'cancle_company_follow_stuff', [vue_this.focus_company, _action.type_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_contract_data();
                    vue_this.popover_switch_off = false;
                }
            });
        },
    },
    beforeMount: function () {
        this.init_contract_data();
        var vue_this = this;
        vue_this.$call_remote_process("company_management", 'get_all_type', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            vue_this.actions = [];
            resp.forEach(function (element) {
                vue_this.get_type_detail(element);
            });
        });
    }
}
</script>

<style scoped>
.del_show {
    margin-left: 10px;
}

.status_show {
    margin-left: 5px;
}

.follow_status_show {
    margin-left: 15px;
}

.single_contract_show {
    border: 1px solid gray;
    margin-bottom: 5px;
}
</style>
