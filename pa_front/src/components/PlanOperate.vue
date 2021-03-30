<template>
<div class="plan_operate_show">
    <van-cell-group title="当前状态">
        <van-cell :value="cur_status" />
    </van-cell-group>
    <div style="margin: 16px;" v-if="has_pri">
        <van-button v-if="status == 0" round block type="primary" @click="submit_confirm">确认计划</van-button>
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
    name: 'PlanOperate',
    data: function () {
        return {
            has_pri: false,
        };
    },
    props: {
        plan_id: Number,
        status: Number,
    },
    computed: {
        cur_status: function () {
            var ret = "等待商家确认计划";
            switch (this.status) {
                case 1:
                    ret = "等待买方付款";
                    break;
                case 2:
                    ret = "等待商家确认付款";
                    break;
                case 3:
                    ret = "等待买方提货";
                    break;
                default:
                    break;
            }

            return ret;
        }
    },
    watch: {
        plan_id(_id) {
            this.get_priv(_id);
        },
    },
    methods: {
        get_priv: function (_id) {
            var vue_this = this;
            vue_this.$get_client("stuff_plan_management").has_priv_edit(_id, vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                vue_this.has_pri = resp;
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
            });
        },
        submit_confirm: function () {
            var vue_this = this;
            vue_this.$get_client("stuff_plan_management").confirm_plan(vue_this.plan_id, vue_this.$cookies.get('pa_ssid')).then(function (resp) {
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
        this.get_priv(this.plan_id);
    }
}
</script>

<style>

</style>
