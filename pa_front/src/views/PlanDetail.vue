<template>
<div class="plan_confirm_show">
    <van-cell-group title="计划内容">
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>
                    计划内容
                </van-col>
                <van-col v-if="plan_detail.status != 5 && plan_detail.status != 4">
                    <van-button type="danger" size="small" @click="reason_diag = true">撤销计划</van-button>
                </van-col>
            </van-row>
        </template>
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
        <van-collapse v-for="(single_vichele, index) in plan_detail.vichele_info" :key="index" v-model="vichele_panel[index]">
            <van-collapse-item :title="single_vichele.driver_name + '-' + single_vichele.driver_phone" name="1">
                <van-cell title="主车" :value="single_vichele.main_vichele"></van-cell>
                <van-cell title="挂车" :value="single_vichele.behind_vichele"></van-cell>
                <van-cell title="卸车地" :value="single_vichele.drop_address"></van-cell>
                <van-cell title="数量" :value="single_vichele.count"></van-cell>
                <van-cell title="用途" :value="single_vichele.use_for"></van-cell>
            </van-collapse-item>
        </van-collapse>
    </van-cell-group>

    <plan-operate :plan_id="plan_detail.plan_id" :status="plan_detail.status"></plan-operate>

    <van-steps direction="vertical" :active="plan_detail.status">
        <van-step>
            <h3>创建计划</h3>
            <p>{{plan_detail.created_time}}</p>
            <p>备注：{{plan_detail.comment}}</p>
        </van-step>
        <van-step>
            <h3>计划确认</h3>
            <p>{{plan_detail.plan_confirm_timestamp}}</p>
            <p>{{plan_detail.plan_confirm_by}}</p>
        </van-step>
        <van-step>
            <h3>付款</h3>
            <p>{{plan_detail.pay_timestamp}}</p>
            <div v-if="plan_detail.pay_timestamp">
                <p>付款信息：</p>
                <van-image width="100" height="100" :src="$remote_url +  plan_detail.pay_info" @click="preview_pay" />
            </div>
        </van-step>
        <van-step>
            <h3>收款确认</h3>
            <p>{{plan_detail.pay_confirm_timestamp}}</p>
            <p>{{plan_detail.pay_confirm_by}}</p>
        </van-step>
        <van-step>
            <h3>提货结束</h3>
            <p>{{plan_detail.close_timestamp}}</p>
            <p>{{plan_detail.close_by}}</p>
        </van-step>
        <van-step>
            <h3>撤销</h3>
            <p>{{plan_detail.except_close_timestamp}}</p>
            <p>撤销原因：{{plan_detail.except_close_reason}}</p>
            <p>撤销人：{{plan_detail.except_close_by}}</p>
        </van-step>
    </van-steps>
    <van-dialog v-model="reason_diag" title="确认撤销" closeOnClickOverlay :showConfirmButton="false">
        <van-form @submit="except_close">
            <van-field v-model="reason_input" name="关闭理由" label="撤销原因" placeholder="请输入撤销原因" :rules="[{ required: true, message: '请填写撤销原因' }]" />
            <van-button plain block>确认</van-button>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import PlanOperate from '../components/PlanOperate.vue'
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
import {
    Step,
    Steps
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    ImagePreview
} from 'vant';
import {
    Collapse,
    CollapseItem
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

Vue.use(Field);
Vue.use(Form);
Vue.use(Dialog);
Vue.use(Collapse);
Vue.use(CollapseItem);
Vue.use(VanImage);
Vue.use(Step);
Vue.use(Steps);
Vue.use(Col);
Vue.use(Row);
Vue.use(Button);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'PlanDetail',
    components: {
        "plan-operate": PlanOperate,
    },
    data: function () {
        return {
            vichele_panel: [
                []
            ],
            plan_detail: {
                plan_id: 0,
                name: '',
                count: 0.0,
                unit_price: 0.0,
                total_price: 0.0,
                plan_time: '',
                created_time: '',
                status: 0,
                vichele_info: [],
                comment: '',
                plan_confirm_by: '',
                plan_confirm_timestamp: '',
                pay_info: '',
                pay_timestamp: '',
                pay_confirm_by: '',
                pay_confirm_timestamp: '',
                close_timestamp: '',
                close_by: '',
                except_close_by:'',
                except_close_timestamp: '',
                except_close_reason: '',
            },
            plan_owner_info: {
                name: '',
                company: '',
            },
            reason_diag: false,
            reason_input:''
        };
    },
    methods: {
        except_close: function () {
            console.log('close');
            this.reason_diag = false;
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management",'except_close', [vue_this.plan_detail.plan_id, vue_this.$cookies.get('pa_ssid'), vue_this.reason_input]).then(function(resp) {
                if (resp) {
                    vue_this.$toast("计划已撤销");
                    vue_this.$router.back();
                }
            });
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            var h = date.getHours();
            h = h < 10 ? ('0' + h) : h;
            var minute = date.getMinutes();
            minute = minute < 10 ? ('0' + minute) : minute;
            var second = date.getSeconds();
            second = second < 10 ? ('0' + second) : second;
            return y + '-' + m + '-' + d + ' ' + h + ':' + minute + ':' + second;
        },
        preview_pay: function () {
            ImagePreview([this.$remote_url + this.plan_detail.pay_info]);
        },
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$call_remote_process("stuff_plan_management", 'get_plan', [parseInt(vue_this.$route.params.plan_id)]).then(function (resp) {
            vue_this.plan_detail.plan_id = resp.plan_id;
            vue_this.plan_detail.name = resp.name;
            vue_this.plan_detail.count = resp.count;
            vue_this.plan_detail.unit_price = resp.price;
            vue_this.plan_detail.total_price = vue_this.plan_detail.unit_price * vue_this.plan_detail.count;
            vue_this.plan_detail.plan_time = resp.plan_time;
            vue_this.plan_detail.created_time = vue_this.formatDateTime(new Date(resp.created_time * 1000));
            vue_this.plan_detail.status = resp.status;
            vue_this.plan_detail.comment = resp.comment;
            vue_this.plan_detail.plan_confirm_by = resp.plan_confirm.name;
            vue_this.plan_detail.plan_confirm_timestamp = resp.plan_confirm.timestamp;
            vue_this.plan_detail.pay_info = resp.pay_info;
            vue_this.plan_detail.pay_confirm_by = resp.pay_confirm.name;
            vue_this.plan_detail.pay_confirm_timestamp = resp.pay_confirm.timestamp;
            vue_this.plan_detail.pay_timestamp = resp.pay_timestamp;
            vue_this.plan_detail.close_timestamp = resp.close_timestamp;
            vue_this.plan_detail.close_by = resp.close_by;
            vue_this.plan_detail.except_close_by= resp.except_close_by;
            vue_this.plan_detail.except_close_timestamp= resp.except_close_timestamp;
            vue_this.plan_detail.except_close_reason= resp.except_close_reason;
            resp.vichele_info.forEach((element, index) => {
                vue_this.$set(vue_this.plan_detail.vichele_info, index, element);
                vue_this.$set(vue_this.vichele_panel, index, ['0']);
            });
            vue_this.$call_remote_process("user_management", 'get_customer_info', [resp.created_by]).then(function (resp) {
                vue_this.plan_owner_info.company = resp.split('(')[0];
                vue_this.plan_owner_info.name = resp.split('(')[1].split(')')[0];
            });
        });
    },
}
</script>

<style>

</style>
