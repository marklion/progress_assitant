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
        </template>
        <van-cell v-for="(single_contract, index) in contract" :key="index" center :label="'合同编号：' + single_contract.number">
            <template #title>
                <span>{{my_side(single_contract)}}</span>
                <span class="status_show">
                    <van-tag :type="contract_status[single_contract.status].type">{{contract_status[single_contract.status].text}}</van-tag>
                </span>
            </template>
            <template #right-icon v-if="!$store.state.userinfo.buyer">
                <div class="del_show">
                    <van-button plain size="small" type="danger" icon="delete-o" @click="del_contract(single_contract.id)"></van-button>
                </div>
            </template>
            <div>开始日期：{{single_contract.start_time}}</div>
            <div>到期日期：{{single_contract.end_time}}</div>
        </van-cell>
    </van-cell-group>
    <van-dialog v-model="add_contract_show" title="添加合同" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="add_contract">
            <van-field v-model="submit_contract.a_side_company" name="甲方" label="甲方" placeholder="请输入甲方公司名" :rules="[{ required:true, message:'请输入甲方公司'}]" />
            <van-field v-model="submit_contract.number" name="编号" label="合同编号" placeholder="请输入合同编号" :rules="[{ required:true, message:'请输入合同编号'}]" />
            <van-field readonly clickable name="calendar1" :value="submit_contract.start_time" label="开始日期" placeholder="点击选择日期" @click="show_start_time_picker= true" :rules="[{ required:true, message:'请选择合同起始日期'}]" />
            <van-calendar :min-date="min_date" :max-date="max_date" get-container="body" v-model="show_start_time_picker" @confirm="confirm_start_time" />
            <van-field readonly clickable name="calendar2" :value="submit_contract.end_time" label="到期日期" placeholder="点击选择日期" @click="show_end_time_picker = true" :rules="[{ required:true, message:'请选择合同起始日期'}]" />
            <van-calendar :min-date="min_date" :max-date="max_date" get-container="body" v-model="show_end_time_picker" @confirm="confirm_end_time" />
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
    data: function () {
        return {
            submit_contract: {
                number: '',
                a_side_company: '',
                start_time: '',
                end_time: '',
            },
            show_start_time_picker: false,
            show_end_time_picker: false,
            add_contract_show: false,
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
        };
    },
    methods: {
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
        confirm_start_time: function (_date) {
            this.submit_contract.start_time = this.formatDateTime(_date)
            this.show_start_time_picker = false;
        },
        confirm_end_time: function (_date) {
            this.submit_contract.end_time = this.formatDateTime(_date)
            this.show_end_time_picker = false;
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
                    vue_this.$set(vue_this.contract, index, element);
                });
            });
            var cur_date = new Date();
            vue_this.min_date.setFullYear(cur_date.getFullYear() - 1);
            vue_this.max_date.setFullYear(cur_date.getFullYear() + 3);
        },
    },
    beforeMount: function () {
        this.init_contract_data();
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
</style>
