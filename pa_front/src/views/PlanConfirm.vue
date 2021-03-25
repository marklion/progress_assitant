<template>
<div class="plan_confirm_show">
    <van-cell-group title="计划内容">
        <van-cell :title="plan_detail.name" :value="plan_detail.count + '吨'" />
        <van-cell title="单价" :value="plan_detail.unit_price" />
        <van-cell title="总价" :value="plan_detail.total_price" />
        <van-cell title="计划到厂" :value="plan_detail.plan_time" />
    </van-cell-group>
    <van-cell-group title="提交人信息">
        <van-cell title="提交人" :value="plan_owner_info.name"></van-cell>
        <van-cell title="公司" :value="plan_owner_info.company"></van-cell>
    </van-cell-group>
    <van-cell-group title="车辆信息">
        <van-cell v-for="(single_vichele, index) in plan_detail.vichele_info" :key="index" :title="single_vichele"></van-cell>
    </van-cell-group>

    <div style="margin: 16px;" v-if="plan_detail.need_confirm == 0">
        <van-row :gutter="16">
            <van-col :span="12">
                <van-button round block type="danger" @click="submit_confirm(false)">驳回</van-button>
            </van-col>
            <van-col :span="12">
                <van-button round block type="primary" @click="submit_confirm(true)">确认</van-button>
            </van-col>
        </van-row>
    </div>
    <div v-else-if="plan_detail.need_confirm == 1">
        <h2>已确认</h2>
    </div>
    <div v-else>
        <h2>已驳回</h2>
    </div>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Button
} from 'vant';
import {
    Col,
    Row
} from 'vant';

Vue.use(Col);
Vue.use(Row);
Vue.use(Button);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'PlanConfirm',
    data: function () {
        return {
            plan_detail: {
                plan_id: 0,
                name: '',
                count: 0.0,
                unit_price: 0.0,
                total_price: 0.0,
                plan_time: '',
                need_confirm: 0,
                vichele_info: [],
            },
            plan_owner_info: {
                name: '',
                company: '',
            },
        };
    },
    methods: {
        submit_confirm: function (_confirm) {
            var vue_this = this;
            vue_this.$get_client("stuff_plan_management").confirm_plan(vue_this.plan_detail.plan_id, vue_this.$cookies.get('pa_ssid'), _confirm).then(function (resp) {
                if (resp) {
                    vue_this.$router.back();
                }
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
            });
        },
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$get_client("stuff_plan_management").get_plan(parseInt(vue_this.$route.params.plan_id)).then(function (resp) {
            vue_this.plan_detail.plan_id = resp.plan_id;
            vue_this.plan_detail.name = resp.name;
            vue_this.plan_detail.count = resp.count;
            vue_this.plan_detail.unit_price = resp.price;
            vue_this.plan_detail.total_price = vue_this.plan_detail.unit_price * vue_this.plan_detail.count;
            vue_this.plan_detail.plan_time = resp.plan_time;
            vue_this.plan_detail.need_confirm = resp.status;
            resp.vichele_info.forEach((element, index) => {
                vue_this.$set(vue_this.plan_detail.vichele_info, index, element);
            });
            vue_this.$get_client("user_management").get_customer_info(resp.created_by).then(function (resp) {
                vue_this.plan_owner_info.company = resp.split('(')[0];
                vue_this.plan_owner_info.name = resp.split('(')[1].split(')')[0];
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
            });
        }).catch(function (err) {
            console.log(err);
            vue_this.$toast(err.msg);
        });
    },
}
</script>

<style>

</style>
