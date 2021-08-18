<template>
<div class="contract_cell_show">
    <van-cell center :label="contract.number?'合同编号：' + contract.number:''">
        <template #title>
            <van-row :gutter="10" type="flex" align="center">
                <van-col>合同</van-col>
                <van-col>
                    <van-tag :type="contract_status[contract.status].type">{{contract_status[contract.status].text}}</van-tag>
                </van-col>
            </van-row>
        </template>
        <template #right-icon v-if="!contract.number && !$store.state.userinfo.buyer">
            <van-button type="primary" size="small" @click="add_contract_show = true">增加</van-button>
        </template>
        <div v-if="contract.number">
            <div>开始日期：{{contract.start_time}}</div>
            <div>到期日期：{{contract.end_time}}</div>
            <div v-if="!$store.state.userinfo.buyer">余额：{{contract.balance}}</div>
        </div>
    </van-cell>
    <van-row type="flex" align="center" justify="center" v-if="!$store.state.userinfo.buyer">
        <van-col :span="12">
            <van-button block size="small" plain square type="info" text="复制公司名" @click="$copyText(a_side)" />
        </van-col>
        <van-col :span="12">
            <van-button block size="small" plain square type="primary" text="复制编码" @click="$copyText(contract.customer_code)" />
        </van-col>
    </van-row>

    <van-dialog v-model="add_contract_show" title="添加合同" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="add_contract">
            <van-field v-model="submit_contract.a_side_company" name="甲方" label="甲方" placeholder="请输入甲方公司名" :rules="[{ required:true, message:'请输入甲方公司'}]" />
            <van-field v-model="submit_contract.number" name="编号" label="合同编号" placeholder="请输入合同编号" :rules="[{ required:true, message:'请输入合同编号'}]" />
            <van-field name="calendar1" v-model="submit_contract.start_time" label="开始日期" placeholder="请输入开始日期yyyy/mm/dd" format-trigger="onBlur" :formatter="formatter_input_date" :rules="[{ required:true, message:'请输入开始日期'}]" />
            <van-field name="calendar2" v-model="submit_contract.end_time" label="到期日期" placeholder="请输入到期日期yyyy/mm/dd" format-trigger="onBlur" :formatter="formatter_input_date" :rules="[{ required:true, message:'请输入到期日期'}]" />
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
    Tag
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Button
} from 'vant';
import {
    SwipeCell
} from 'vant';
import VueClipboard from 'vue-clipboard2'

Vue.use(VueClipboard)
Vue.use(SwipeCell);
Vue.use(Button);
Vue.use(Dialog);
Vue.use(Form);
Vue.use(Field);
Vue.use(Col);
Vue.use(Row);
Vue.use(Tag);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'ContractCell',
    props: {
        a_side: String,
        b_side: String,
    },
    data: function () {
        return {
            submit_contract: {
                number: '',
                a_side_company: '',
                start_time: '',
                end_time: '',
            },
            add_contract_show: false,
            contract: {
                start_time: '',
                end_time: '',
                number: '',
                status: 3,
            },
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
        };
    },
    methods: {
        add_contract: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "add_contract", [vue_this.$cookies.get('pa_ssid'), vue_this.submit_contract]).then(function (resp) {
                if (resp) {
                    vue_this.get_contract(vue_this.a_side, vue_this.b_side);
                    vue_this.add_contract_show = false;
                }
            });
        },
        get_contract: function (_a_side, _b_side) {
            var vue_this = this;
            vue_this.$call_remote_process_no_toast("company_management", "get_contract", [_a_side, _b_side]).then(function (resp) {
                vue_this.contract = resp;
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
    },
    watch: {
        "a_side": function () {
            this.get_contract(this.a_side, this.b_side);
        },
        "b_side": function () {
            this.get_contract(this.a_side, this.b_side);
        },
    },
    beforeMount: function () {
        this.get_contract(this.a_side, this.b_side);

        this.submit_contract.a_side_company = this.a_side;
    }

}
</script>

<style scoped>
.swipe_button {
    height: 100%;
}
</style>
