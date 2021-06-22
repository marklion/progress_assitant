<template>
<div class="plan_operate_show">
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>当前状态</van-col>
                <van-col>
                    <van-row type="flex" align="center" :gutter="10">
                        <van-col v-if="user_is_buyer">
                            <van-button v-if="can_change_to(0)" size="small" type="warning" :to="{name:'PlanUpdate', params:{plan_id:plan_id}}">修改计划</van-button>
                        </van-col>
                        <van-col v-if="!$store.state.userinfo.buyer && status == 2">
                            <van-button size="small" type="warning" @click="push_buyer">催款</van-button>
                        </van-col>
                    </van-row>
                </van-col>
            </van-row>
        </template>
        <van-cell :title="prompt" center>
            <template #right-icon>
                <div v-if="status > 0">
                    <van-row type="flex" align="center" justify="end" :gutter="10">
                        <van-col v-if="!$store.state.userinfo.buyer && can_change_to(status-1)">
                            <van-button block size="small" type="danger" @click="show_reject_reason_diag = true">驳回</van-button>
                        </van-col>
                        <van-col v-if="!$store.state.userinfo.buyer && can_change_to(status + 1)">
                            <van-button v-if="status == 1" block size="small" type="primary" @click="submit_confirm">确认计划</van-button>
                            <van-button v-if="status == 2" block size="small" type="primary" @click="submit_confirm_pay">确认收款</van-button>
                        </van-col>
                        <van-col v-if="status >= 3">
                            <van-button block size="small" type="primary" :to="{name:'DeliverPlan', params:{plan_id:plan_id}}">出货详情</van-button>
                        </van-col>
                    </van-row>
                </div>
            </template>
            <div v-if="!$store.state.userinfo.buyer && can_change_to(status + 1) && (status == 1 || status == 2)">
                <van-field v-model="confirm_comment" placeholder="输入确认附言（可选）"></van-field>
            </div>
        </van-cell>
    </van-cell-group>

    <van-dialog v-model="show_reject_reason_diag" title="确认驳回" closeOnClickOverlay :showConfirmButton="false">
        <van-form @submit="reject_plan">
            <van-field v-model="reject_reason" name="驳回原因" label="驳回原因" placeholder="请输入驳回原因" :rules="[{ required: true, message: '请填写驳回原因' }]" />
            <van-button plain block>确认</van-button>
        </van-form>
    </van-dialog>
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
import {
    Uploader
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

Vue.use(Dialog);
Vue.use(Field);
Vue.use(Form);
Vue.use(Uploader);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Button);
export default {
    name: 'PlanOperate',
    data: function () {
        return {
            show_qr: false,
            show_reject_reason_diag: false,
            reject_reason: '',
            status_change_rule: [],
            can_change_to: function (_index) {
                var ret = false;
                if (_index >= 0 && _index < this.status_change_rule.length) {
                    ret = this.status_change_rule[_index];
                }

                return ret;
            },
            confirm_comment: "",
        };
    },
    props: {
        plan_id: Number,
        status: Number,
        is_proxy: Boolean,
        prompt: String,
    },
    components: {},
    computed: {
        user_is_buyer: function () {
            var ret = false;
            if (this.$store.state.userinfo.buyer || this.is_proxy) {
                ret = true;
            }

            return ret;
        },
    },
    watch: {
        plan_id(_id) {
            this.get_change_rule(_id);
        },
    },
    methods: {
        push_buyer: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "push_user_pay", [vue_this.$cookies.get('pa_ssid'), vue_this.plan_id]).then(function (resp) {
                if (resp == true) {
                    vue_this.$toast("已发送催款信息");
                }
            });
        },
        reject_plan: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "reject_plan", [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), vue_this.reject_reason]).then(function (resp) {
                if (resp) {
                    vue_this.show_reject_reason_diag = false;
                    vue_this.$router.go(0);
                }
            });
        },
        randomString: function (len) {
            len = len || 32;
            var $chars = 'ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678'; /****默认去掉了容易混淆的字符oOLl,9gq,Vv,Uu,I1****/
            var maxPos = $chars.length;
            var pwd = '';
            for (var i = 0; i < len; i++) {
                pwd += $chars.charAt(Math.floor(Math.random() * maxPos));
            }
            return pwd;
        },
        get_change_rule: function (_id) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'get_change_rule', [vue_this.$cookies.get('pa_ssid'), _id]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.status_change_rule, index, element);
                });
            });
        },
        submit_confirm: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'confirm_plan', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), vue_this.confirm_comment]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },
        submit_confirm_pay: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'confirm_pay', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), vue_this.confirm_comment]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },

    },
    beforeMount: function () {
        this.get_change_rule(this.plan_id);
    }
}
</script>

<style>

</style>
