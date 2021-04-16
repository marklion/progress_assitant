<template>
<div class="stuff_info_submit_show">
    <van-form @submit="submit_plan">
        <van-field center readonly name="stepper" label="总计划量(吨)" :value="all_count">
        </van-field>
        <van-field center readonly clickable name="datetimePicker" :value="plan_time" label="到厂时间" placeholder="点击选择时间" @click="show_time_picker = true">
        </van-field>
        <van-field center name="comment" v-model="comment" label="备注" placeholder="请输入备注">
        </van-field>
        <van-popup v-model="show_time_picker" position="bottom">
            <van-datetime-picker type="datehour" title="请选择时间" :min-date="min_time" @cancel="show_time_picker = false" @confirm="confirm_time" />
        </van-popup>
        <van-row type="flex" justify="center" align="center">
            <van-col :span="20">
                <van-divider>运输车辆</van-divider>
            </van-col>
            <van-col :span="4">
                <van-button round size="small" type="primary" icon="plus" native-type="button" @click="add_vichele_info"></van-button>
            </van-col>
        </van-row>
        <div class="single_vichele_show" v-for="(single_vichele, index) in vichele_info" :key="index">
            <vichele-in-plan :vichele_info="single_vichele"></vichele-in-plan>
            <van-button plain round block icon="delete-o" native-type="button" @click="remove_vichele_info(index)"></van-button>
        </div>
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit" :disabled="!stuff_info_change">{{action_name}}</van-button>
        </div>
    </van-form>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Stepper
} from 'vant';
import {
    Button
} from 'vant';
import {
    DatetimePicker
} from 'vant';
import {
    Popup
} from 'vant';
import {
    Tag
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Divider
} from 'vant';
import {
    Picker
} from 'vant';
import {
    Toast
} from 'vant';
import {
    Dialog
} from 'vant';

Vue.use(Dialog);
Vue.use(Toast);
Vue.use(Picker);
Vue.use(Divider);
Vue.use(Col);
Vue.use(Row);
Vue.use(Tag);
Vue.use(Popup);
Vue.use(DatetimePicker);
Vue.use(Button);
Vue.use(Stepper);
Vue.use(Field);
Vue.use(Form);
import VicheleInPlan from './VicheleInPlan.vue'
export default {
    name: 'StuffInfoSubmit',
    props: {
        orig_plan_count: Number,
        orig_vichele_info: {
            type: Array,
            default: function () {
                return []
            }
        },
        orig_plan_time: String,
        min_time: Date,
        is_create: Boolean,
        plan_id: Number,
        type_id: Number,
        orig_name: String,
        orig_price: Number,
        orig_comment: String,
        proxy_company:String,
    },
    components: {
        "vichele-in-plan": VicheleInPlan,
    },
    data: function () {
        return {
            plan_count: 10.0,
            vichele_info: [],
            plan_time: '',
            show_time_picker: false,
            bound_vichele: [],
            show_vichele_picker: false,
            show_add_vichele_diag: false,
            new_vichele: '',
            comment: '',
        };
    },
    computed: {
        all_count: function () {
            var ret = 0;
            this.vichele_info.forEach((element) => {
                ret += parseFloat(element.count);
            });

            return ret;
        },
        action_name: function () {
            if (this.is_create) {
                return "提交";
            } else {
                return "修改"
            }
        },
        stuff_info_change: function () {
            var a = JSON.parse(JSON.stringify(this.orig_vichele_info));
            var b = JSON.parse(JSON.stringify(this.vichele_info));
            a.sort();
            b.forEach((element) => {
                element.count = parseFloat(element.count);
            });
            b.sort();
            var ret = false;
            if (this.is_create) {
                ret = true;
            } else if (this.plan_time != this.orig_plan_time || this.plan_count != this.orig_plan_count || JSON.stringify(a) != JSON.stringify(b) || this.orig_comment != this.comment) {
                ret = true;
            }
            return ret;
        },
    },
    watch: {
        orig_plan_count(_new_value) {
            this.plan_count = _new_value;
        },
        orig_plan_time(_new_value) {
            this.plan_time = _new_value;
        },
        orig_vichele_info(_new_value) {
            var vue_this = this;
            vue_this.vichele_info = JSON.parse(JSON.stringify(_new_value));
        },
        orig_comment(_new_value) {
            this.comment = _new_value;
        },
    },
    beforeMount: function () {
        if (this.is_create) {
            this.plan_time = this.formatDateTime(new Date());
        } else {
            this.plan_time = this.orig_plan_time;
            this.plan_count = this.orig_plan_count;
            this.vichele_info = JSON.parse(JSON.stringify(this.orig_vichele_info));
            this.comment = this.orig_comment;
        }
    },
    methods: {
        add_vichele_info: function () {
            this.vichele_info.splice(0, 0, {
                main_vichele: '',
                behind_vichele: '',
                driver_name: '',
                driver_phone: '',
                drop_address: '',
                use_for: '',
                count: 20,
            });
        },
        remove_vichele_info: function (_index) {
            this.vichele_info.splice(_index, 1);
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            var h = date.getHours();
            h = h < 10 ? ('0' + h) : h;
            return y + '-' + m + '-' + d + ' ' + h;
        },
        confirm_time: function (_time) {
            this.plan_time = this.formatDateTime(_time);
            this.show_time_picker = false;
        },
        submit_plan_to_server: function () {
            var vue_this = this;
            if (this.is_create) {
                this.$call_remote_process("stuff_plan_management", 'create_plan', [{
                    count: this.plan_count,
                    type_id: this.type_id,
                    plan_time: this.plan_time,
                    vichele_info: this.vichele_info,
                    name: this.orig_name,
                    price: this.orig_price,
                    comment: this.comment,
                }, this.$cookies.get("pa_ssid"),vue_this.proxy_company]).then(function (resp) {
                    if (resp > 0) {
                        vue_this.$router.push({
                            name: 'CompanyOrder',
                        });
                    }
                });
            } else {
                vue_this.$call_remote_process("stuff_plan_management", 'update_plan', [{
                    plan_id: vue_this.plan_id,
                    count: this.plan_count,
                    plan_time: this.plan_time,
                    vichele_info: this.vichele_info,
                    comment: this.comment,
                }, this.$cookies.get('pa_ssid')]).then(function (resp) {
                    if (resp) {
                        vue_this.$router.back(-1);
                    }
                });
            }
        },
        submit_plan: function () {
            var vue_this = this;
            if (vue_this.vichele_info.length <= 0) {
                vue_this.$toast("请添加车辆信息");
            } else {
                vue_this.$call_remote_process("stuff_plan_management", "verify_plan", [{
                    plan_time: vue_this.plan_time,
                    vichele_info: vue_this.vichele_info,
                    plan_id: vue_this.plan_id,
                }, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                    if (resp.length > 0) {
                        console.log(resp);
                        Dialog.confirm({
                            title: '计划车辆冲突',
                            message: resp,
                            confirmButtonText:'继续上报',
                            cancelButtonText:'再调整下'
                        }).then(function () {
                            vue_this.submit_plan_to_server();
                        }).catch(function () {
                            console.log();
                        });
                    } else {
                        vue_this.submit_plan_to_server();
                    }
                });

            }
        }
    },
}
</script>

<style scoped>
.single_vichele_show {
    border: 1px solid gray;
    border-radius: 16px;
    margin-left: 10px;
    margin-right: 10px
}
</style>
