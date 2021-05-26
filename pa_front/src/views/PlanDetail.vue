<template>
<div class="plan_confirm_show">
    <van-cell-group title="提交人信息">
        <van-cell title="提交人" :value="plan_owner_info.name"></van-cell>
        <van-cell title="公司" :value="plan_owner_info.company" center>
            <template #right-icon v-if="!is_proxy">
                <van-button class="preview_btn_show" size="small" type="info" plain @click="preview_buy_attach">查看买方资质</van-button>
            </template>
        </van-cell>
    </van-cell-group>
    <van-cell-group title="计划内容">
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>
                    计划内容
                </van-col>
                <van-col v-if="status_change_rule[4]">
                    <van-button type="danger" size="small" @click="reason_diag = true">撤销计划</van-button>
                </van-col>
            </van-row>
        </template>
        <van-collapse v-model="extern_company_info">
            <van-collapse-item :title="plan_detail.name" name="1" :label="plan_detail.sale_company">
                <template #value v-if="plan_detail.count != 0">
                    {{plan_detail.count}}吨
                </template>
                <van-field v-model="company_address" rows="1" autosize label="公司地址" type="textarea" readonly>
                </van-field>
                <van-field v-model="company_contact" rows="1" autosize label="联系方式" type="textarea" readonly>
                </van-field>
                <van-cell is-link @click="preview_sale_attach">查看卖方资质</van-cell>
            </van-collapse-item>
        </van-collapse>
        <van-cell title="单价" :value="plan_detail.unit_price" />
        <van-cell title="总价" v-if="plan_detail.total_price != 0" :value="plan_detail.total_price" />
        <van-cell title="计划到厂" :value="plan_detail.plan_time" />
    </van-cell-group>
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>车辆信息</van-col>
                <van-col>
                    <van-button type="info" plain size="small" @click="show_vichele_table = true">浏览</van-button>
                    <van-popup v-model="show_vichele_table" position="bottom" get-container="body">
                        <vxe-table size="small" stripe align="center" max-height="500" :data="plan_detail.vichele_info">
                            <vxe-table-column field="main_vichele" title="主车" width="22%"></vxe-table-column>
                            <vxe-table-column field="behind_vichele" title="挂车" width="23%"></vxe-table-column>
                            <vxe-table-column field="driver_name" title="司机" width="25%"></vxe-table-column>
                            <vxe-table-column field="driver_phone" title="电话" width="30%"></vxe-table-column>
                        </vxe-table>
                    </van-popup>
                </van-col>
            </van-row>
        </template>
        <van-collapse v-for="(single_vichele, index) in plan_detail.vichele_info" :key="index" v-model="vichele_panel[index]">
            <van-collapse-item :title="single_vichele.main_vichele+ '-' + single_vichele.behind_vichele" :value="vichele_status(single_vichele.finish)" name="1">
                <van-cell title="司机" :value="single_vichele.driver_name"></van-cell>
                <van-cell title="电话" :value="single_vichele.driver_phone"></van-cell>
                <van-cell v-if="single_vichele.finish" title="提货时间" :value="single_vichele.deliver_timestamp"></van-cell>
                <van-cell title="卸车地" :value="single_vichele.drop_address"></van-cell>
                <van-cell title="用途" :value="single_vichele.use_for"></van-cell>
            </van-collapse-item>
        </van-collapse>
    </van-cell-group>

    <plan-operate v-if="plan_detail.plan_id != 0" :is_proxy="is_proxy" :plan_id="plan_detail.plan_id" :status="plan_detail.status" :prompt="status_prompt"></plan-operate>

    <van-steps direction="vertical" :active="plan_detail.status">
        <van-step v-for="(single_status, index) in status_in_plan" :key="index">
            <h3>{{single_status.name}}</h3>
            <p v-if="single_status.timestamp">更新时间：{{single_status.timestamp}}</p>
            <p v-if="single_status.comment">备注：{{single_status.comment}}</p>
            <p v-if="single_status.author">提交人：{{single_status.author}}</p>
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
import {
    Notify
} from 'vant';
import {
    Popup
} from 'vant';

Vue.use(Popup);
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
            show_vichele_table: false,
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
                sale_company: ''
            },
            plan_owner_info: {
                name: '',
                company: '',
            },
            reason_diag: false,
            reason_input: '',
            is_proxy: false,
            extern_company_info: [],
            company_address: '',
            company_contact: '',
            status_in_plan: [],
            status_change_rule: [],
            vichele_status: function (_value) {
                var ret = "未提货";
                if (_value) {
                    ret = "已提货";
                }
                return ret;
            },
        };
    },
    computed: {
        status_prompt: function () {
            var ret = "";
            if (this.status_in_plan[this.plan_detail.status]) {
                ret = this.status_in_plan[this.plan_detail.status].prompt;
            }

            return ret;
        },

    },
    methods: {
        preview_buy_attach: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_attachment", [vue_this.plan_owner_info.company]).then(function (resp) {
                if (resp) {
                    ImagePreview([vue_this.$remote_url + resp]);
                }
            });
        },
        preview_sale_attach: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_attachment", [vue_this.plan_detail.sale_company]).then(function (resp) {
                if (resp) {
                    ImagePreview([vue_this.$remote_url + resp]);
                }
            });
        },
        except_close: function () {
            console.log('close');
            this.reason_diag = false;
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'except_close', [vue_this.plan_detail.plan_id, vue_this.$cookies.get('pa_ssid'), vue_this.reason_input]).then(function (resp) {
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
        get_status_in_plan: function () {
            var vue_this = this;
            vue_this.status_in_plan = [];
            vue_this.$call_remote_process("stuff_plan_management", "get_status_rule", [parseInt(vue_this.$route.params.plan_id)]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.status_in_plan, index, element);
                });
                if (vue_this.plan_detail.status == 0 && vue_this.status_in_plan[1].comment) {
                    Dialog({
                        message: '卖方驳回了您的计划\n原因：' + vue_this.status_in_plan[1].comment,
                    });
                }
            });
        },
        get_change_rule: function () {
            var vue_this = this;
            vue_this.status_change_rule = [];
            vue_this.$call_remote_process("stuff_plan_management", 'get_change_rule', [vue_this.$cookies.get('pa_ssid'), parseInt(vue_this.$route.params.plan_id)]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.status_change_rule, index, element);
                });
            });
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
            vue_this.plan_detail.sale_company = resp.sale_company;
            vue_this.$call_remote_process("company_management", "get_address_contact", [resp.sale_company]).then(function (company_resp) {
                vue_this.company_address = company_resp.address;
                vue_this.company_contact = company_resp.contact;
            });
            vue_this.vichele_info = [];
            resp.vichele_info.forEach((element, index) => {
                vue_this.$set(vue_this.plan_detail.vichele_info, index, element);
                vue_this.$set(vue_this.vichele_panel, index, ['0']);
            });
            var proxy_company = resp.proxy_company;
            vue_this.plan_owner_info.company = resp.buy_company;
            if (proxy_company.length > 0) {
                vue_this.plan_owner_info.name = resp.created_user_name + '(手工提单)';
                vue_this.is_proxy = true;
            } else {
                vue_this.plan_owner_info.name = resp.created_user_name;
            }
            if (vue_this.is_proxy) {
                Notify("此单为手工单，请自行确认买方资质");
            }

            vue_this.get_status_in_plan();
            vue_this.get_change_rule();
        });
    },
}
</script>

<style scoped>
.preview_btn_show {
    margin-left: 15px;
}
</style>
