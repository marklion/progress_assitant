<template>
<div class="deliver_show">
    <van-cell-group title="计划内容">
        <van-cell :title="buy_company" :label="name" :value="'已提货' + count + '吨'" />
        <van-cell title="提货人" :value="user_name"></van-cell>
    </van-cell-group>
    <van-checkbox-group v-model="pre_deliver_vichele_index">
        <van-cell-group title="未出货车辆">
            <div class="single_vichele_show" v-for="(item, index) in undelivered_vichele" :key="index">
                <van-cell center :title="item.main_vichele + '-' + item.behind_vichele" :label="item.driver_name + '-' + item.driver_phone">
                    <template #icon>
                        <van-checkbox style="margin-right:10px" :name="index" ref="checkboxes" v-if="can_change_to(4)" />
                    </template>
                    <van-row type="flex" align="center" justify="end" v-if="can_change_to(4)">
                        <van-col>
                            <van-stepper v-model="item.count" />
                        </van-col>
                        <van-col>
                            吨
                        </van-col>
                    </van-row>
                </van-cell>
                <div v-if="item.register_number" class="register_info_show">
                    <van-row type="flex" align="center" justify="space-between">
                        <van-col :span="8">
                            进厂序号：{{item.register_number}}
                        </van-col>
                        <van-col :span="8">
                            进厂位置：{{item.enter_location}}
                        </van-col>
                        <van-col :span="8">
                            {{item.register_timestamp}}
                        </van-col>
                    </van-row>
                </div>
                <div v-else-if="!$store.state.userinfo.buyer">
                    <van-button type="primary" size="small" @click="open_change_driver(item.vichele_id)">换司机</van-button>
                    <van-button v-if="item.driver_silent_id" type="danger" size="small" @click="reset_driver_info(item.driver_silent_id)">重置信息</van-button>
                </div>
            </div>
        </van-cell-group>
    </van-checkbox-group>
    <van-dialog v-model="focus_driver_change" title="更换司机" close-on-click-overlay :show-confirm-button="false">
        <van-form @submit="change_driver">
            <van-field v-model="new_driver_name" name="姓名" label="姓名" placeholder="请输入司机姓名" :rules="[{ required: true, message: '请输入司机姓名' }]" />
            <van-field v-model="new_driver_phone" name="电话" label="电话" placeholder="请输入司机电话" :rules="[{ required: true, message: '请输入司机电话' , pattern: phone_pattern}]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
    <van-row :gutter="10" type="flex" justify="center" align="center" v-if="can_change_to(4)">
        <van-col :span="8">
            <van-button block type="info" size="small" :disabled="pre_deliver_vichele_index.length == 0" @click="submit_confirm_deliver">确认出货选中车辆</van-button>
        </van-col>
        <van-col :span="8">
            <van-button block type="primary" size="small" @click="deliver_all">出货所有车辆</van-button>
        </van-col>
        <van-col :span="8">
            <van-button block type="danger" size="small" @click="fource_reason_diag = true;">完成出货</van-button>
        </van-col>
    </van-row>

    <van-cell-group title="已出货车辆">
        <van-collapse v-model="expend_weight">
            <van-collapse-item name="1" v-for="(item, index) in delivered_vichele" center :value="item.count + '吨'" :key="index" :title="item.main_vichele + '-' + item.behind_vichele" :label="item.driver_name + '-' + item.driver_phone">
                <van-cell :title="'皮重：' + item.p_weight" :value="item.p_time"></van-cell>
                <van-cell :title="'毛重：' + item.m_weight" :value="item.deliver_timestamp"></van-cell>
                <van-cell title="查看磅单" is-link :to="{name:'Ticket', params:{id:item.vichele_id + 'S'}}"></van-cell>
            </van-collapse-item>
        </van-collapse>
    </van-cell-group>
    <van-dialog v-model="fource_reason_diag" title="有未出货车辆" close-on-click-overlay :show-confirm-button="false">
        <van-form @submit="fource_close">
            <van-field v-model="close_reason" name="原因" label="原因" placeholder="请输入未出货全部车辆的原因" :rules="[{ required: true, message: '请输入未出货全部车辆的原因' }]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
</div>
</template>

<script>

import {getPlanInfo} from '@/api/plan';

export default {
    name: 'DeliverPlan',
    data: function () {
        return {
            focus_driver_change: false,
            status: 0,
            plan_id: 0,
            user_name: '',
            count: 0,
            name: '',
            vichele_info: [],
            buy_company: '',
            pre_deliver_vichele_index: [],

            status_change_rule: [],
            can_change_to: function (_index) {
                var ret = false;
                if (_index >= 0 && _index < this.status_change_rule.length) {
                    ret = this.status_change_rule[_index];
                }

                return ret;
            },
            close_reason: '',
            fource_reason_diag: false,
            expend_weight: ['1'],
            phone_pattern: /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/,
            focus_change_id: 0,
            new_driver_name:'',
            new_driver_phone:'',
        };
    },
    computed: {
        delivered_vichele: function () {
            var ret = [];
            this.vichele_info.forEach((element) => {
                if (element.finish) {
                    ret.push(element);
                }
            });

            return ret;
        },
        undelivered_vichele: function () {
            var ret = [];
            this.vichele_info.forEach((element) => {
                if (!element.finish) {
                    ret.push(element);
                }
            });

            return ret;
        },
    },
    methods: {
        reset_driver_info: function (_silent_id) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "driver_silent_reset", [vue_this.$cookies.get('pa_ssid'), _silent_id]).then(function(){
                vue_this.$toast("重置成功");
            });
        },
        open_change_driver:function(_vichele_id) {
            this.focus_change_id = _vichele_id;
            this.focus_driver_change = true;
            this.new_driver_name = "";
            this.new_driver_phone = "";
        },
        change_driver: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "change_driver", [vue_this.$cookies.get('pa_ssid'), vue_this.focus_change_id, vue_this.new_driver_name, vue_this.new_driver_phone]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                    vue_this.focus_driver_change = false;
                }
            });
        },
        submit_confirm_deliver: function () {
            var vue_this = this;
            var pre_deliver_vichele = [];
            vue_this.undelivered_vichele.forEach((element, index) => {
                if (vue_this.pre_deliver_vichele_index.indexOf(index) != -1) {
                    pre_deliver_vichele.push({
                        id: element.vichele_id,
                        count: element.count
                    });
                }
            });
            vue_this.$call_remote_process("stuff_plan_management", 'confirm_deliver', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), pre_deliver_vichele]).then(function (resp) {
                if (resp) {
                    vue_this.$router.back();
                }
            });
        },
        deliver_all: function () {
            var vue_this = this;
            var all_vichele = [];
            vue_this.undelivered_vichele.forEach(element => {
                all_vichele.push({
                    id: element.vichele_id,
                    count: element.count
                });
            });
            vue_this.$call_remote_process("stuff_plan_management", 'confirm_deliver', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), all_vichele]).then(function (resp) {
                if (resp) {
                    vue_this.$router.back();
                }
            });
        },
        fource_close: function () {
            var vue_this = this;

            vue_this.$call_remote_process("stuff_plan_management", 'confirm_deliver', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), [], vue_this.close_reason]).then(function (resp) {
                if (resp) {
                    vue_this.fource_reason_diag = false;
                    vue_this.$router.back();
                }
            });
        },
        confirm_close: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'confirm_close', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },
        get_change_rule: function (_id) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'get_change_rule', [vue_this.$cookies.get('pa_ssid'), _id]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.status_change_rule, index, element);
                });
            });
        },
    },
    beforeMount: async function () {
        this.plan_id = parseInt(this.$route.params.plan_id)
        let planInfo = await getPlanInfo(this.plan_id);

        this.status = planInfo.status;
        this.count = planInfo.count;
        this.name = planInfo.name;
        this.user_name = planInfo.created_user_name;
        planInfo.vichele_info.forEach((element, index) => {
            if (!element.finish) {
                element.count = 20;
            }
            this.$set(this.vichele_info, index, element);
        });
        this.buy_company = planInfo.buy_company;
        this.get_change_rule(this.plan_id);
    },
}
</script>

<style scoped>
.single_vichele_show {
    border: 1px solid gray;
    margin-bottom: 5px;
}

.register_info_show {
    font-size: 12px;
    padding-left: 5px;
}
</style>
<!--behind_vichele:"晋FH051挂"-->
<!--count:25-->
<!--deliver_timestamp:""-->
<!--driver_id:""-->
<!--driver_name:"赵贵德"-->
<!--driver_phone:"17803460765"-->
<!--driver_silent_id:""-->
<!--drop_address:"北京市/北京市"-->
<!--enter_location:""-->
<!--finish:false-->
<!--m_weight:0-->
<!--main_vichele:"蒙KH5583"-->
<!--p_time:""-->
<!--p_weight:0-->
<!--register_number:""-->
<!--register_timestamp:""-->
<!--use_for:"气化"-->
<!--vichele_id:34980-->
