<template>
<div class="order_show">
    <van-tabs v-model="active" sticky>
        <van-tab title="全部" >
            <plan-brief v-for="(single_plan, index) in orders" :key="index" :plan_id="single_plan.plan_id" :company_view="false"></plan-brief>
        </van-tab>
        <van-tab title="待确认" :badge="numbers_of_tab(0)">
            <plan-brief v-for="(single_plan, index) in order_need_show" :key="index" :plan_id="single_plan" :company_view="false"></plan-brief>
        </van-tab>
        <van-tab title="待付款" :badge="numbers_of_tab(1)">
            <plan-brief v-for="(single_plan, index) in order_need_show" :key="index" :plan_id="single_plan" :company_view="false"></plan-brief>
        </van-tab>
        <van-tab title="待收款" :badge="numbers_of_tab(2)">
            <plan-brief v-for="(single_plan, index) in order_need_show" :key="index" :plan_id="single_plan" :company_view="false"></plan-brief>
        </van-tab>
        <van-tab title="待提货" :badge="numbers_of_tab(3)">
            <plan-brief v-for="(single_plan, index) in order_need_show" :key="index" :plan_id="single_plan" :company_view="false"></plan-brief>
        </van-tab>
        <van-tab title="已完成">
            <plan-brief v-for="(single_plan, index) in order_need_show" :key="index" :plan_id="single_plan" :company_view="false"></plan-brief>
        </van-tab>
    </van-tabs>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Tab,
    Tabs
} from 'vant'

Vue.use(Tab);
Vue.use(Tabs);
import PlanBrief from '../components/PlanBrief.vue'
export default {
    name: 'Order',
    data: function () {
        return {
            orders: [],
            active: 0,
            numbers_of_tab: function (_status) {
                var ret = 0;
                this.orders.forEach((element) => {
                    if (element.status == _status) {
                        ret++;
                    }
                });
                if (ret > 0)
                    return ret;
                else
                    return ''
            },
        };
    },
    computed: {
        order_need_show: function () {
            var ret = [];
            var vue_this = this;
            vue_this.orders.forEach((element) => {
                if (element.status == vue_this.active - 1) {
                    ret.push(element.plan_id);
                }
            });

            return ret;
        },
    },
    components: {
        "plan-brief": PlanBrief,
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$call_remote_process("stuff_plan_management",'get_created_plan', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            resp.forEach((element, index) => {
                vue_this.$set(vue_this.orders, index, element);
            });
        });
    },
}
</script>

<style>

</style>
