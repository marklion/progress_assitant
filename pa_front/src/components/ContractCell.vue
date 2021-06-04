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
        <div v-if="contract.number">
            <div>开始日期：{{contract.start_time}}</div>
            <div>到期日期：{{contract.end_time}}</div>
        </div>
    </van-cell>
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
        get_contract: function (_a_side, _b_side) {
            var vue_this = this;
            vue_this.$call_remote_process_no_toast("company_management", "get_contract", [_a_side, _b_side]).then(function (resp) {
                vue_this.contract = resp;
            });
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
    }

}
</script>

<style>

</style>
