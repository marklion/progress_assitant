<template>
<div class="close_plan_show">
    <div v-if="status == 3">
        <van-cell-group title="提货信息">
            <van-cell :title="name" :value="count + '吨'" />
            <van-cell title="提货人" :value="user_name"></van-cell>
        </van-cell-group>
        <div style="margin: 16px;">
            <van-button round type="primary" block @click="confirm_close">确认已提货</van-button>
        </div>
    </div>
    <div v-if="status == 4">
        <van-cell-group title="提货信息">
            <van-cell :title="name" :value="count + '吨'" />
            <van-cell title="提货人" :value="user_name"></van-cell>
            <van-cell value="已确认提货"></van-cell>
        </van-cell-group>
    </div>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Button
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';

Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Button);
export default {
    name: 'ClosePlan',
    data: function () {
        return {
            status: 0,
            plan_id: 0,
            user_name: '',
            count: 0,
            name: '',
        };
    },
    methods: {
        confirm_close: function () {
            var vue_this = this;
            vue_this.$get_client("stuff_plan_management").confirm_close(vue_this.plan_id, vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
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
            vue_this.status = resp.status;
            vue_this.plan_id = resp.plan_id;
            vue_this.count = resp.count;
            vue_this.name = resp.name;
            vue_this.$get_client("user_management").get_customer_info(resp.created_by).then(function (resp) {
                vue_this.user_name = resp;
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
