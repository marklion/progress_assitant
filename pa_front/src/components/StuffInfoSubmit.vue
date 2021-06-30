<template>
<div class="stuff_info_submit_show">
    <van-form @submit="submit_plan" scroll-to-error @failed="$toast('填入信息有误，请检查')">
        <van-field center readonly clickable name="datetimePicker" :value="plan_time" label="到厂时间" placeholder="点击选择时间" @click="show_time_picker = true">
            <template #right-icon>
                <van-tag type="primary">{{plan_time_easy}}</van-tag>
            </template>
        </van-field>
        <van-field center name="comment" v-model="comment" label="备注" placeholder="请输入备注">
        </van-field>
        <van-popup v-model="show_time_picker" position="bottom">
            <van-datetime-picker type="datehour" :formatter="date_formatter" title="请选择时间" :min-date="min_time" @cancel="show_time_picker = false" @confirm="confirm_time" />
        </van-popup>
        <van-row type="flex" justify="center" align="center">
            <van-col :span="20">
                <van-divider>运输车辆: 已添加{{vichele_info.length}}辆</van-divider>
            </van-col>
            <van-col :span="4">
                <van-button round size="small" type="primary" icon="plus" native-type="button" @click="add_vichele_info"></van-button>
            </van-col>
        </van-row>
        <div class="single_vichele_show" v-for="(single_vichele, index) in vichele_info" :key="index" ref="vichele_ref">
            <div class="vichele_index_show">{{index + 1}}</div>
            <vichele-in-plan :vichele_info="single_vichele"></vichele-in-plan>
            <van-divider />
            <van-button plain round block type="danger" icon="delete-o" native-type="button" @click="remove_vichele_info(index)">移除</van-button>
        </div>
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit" :disabled="!stuff_info_change">{{action_name}}: 共{{vichele_info.length}}辆车</van-button>
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
        proxy_company: String,
    },
    components: {
        "vichele-in-plan": VicheleInPlan,
    },
    data: function () {
        return {
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
        plan_time_easy: function () {
            var setted_time = new Date(/\d{4}-\d{1,2}-\d{1,2}/g.exec(this.plan_time)[0]);
            var current_time = new Date();
            current_time.setHours(0,0,0,0);
            var ret = "";
            var bigger_than = parseInt((setted_time.getTime() - current_time.getTime()) / (1000*60*60*24));
            if (bigger_than == 0) {
                ret = "今天";
            } else if (bigger_than == 1) {
                ret = "明天";
            } else {
                ret = bigger_than.toString() + "天后";
            }

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
            } else if (this.plan_time != this.orig_plan_time || JSON.stringify(a) != JSON.stringify(b) || this.orig_comment != this.comment) {
                ret = true;
            }
            return ret;
        },
    },
    watch: {
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
            var date = new Date();
            date.setDate(date.getDate() + 1);
            this.plan_time = this.formatDateTime(date);
        } else {
            this.plan_time = this.orig_plan_time;
            this.vichele_info = JSON.parse(JSON.stringify(this.orig_vichele_info));
            this.comment = this.orig_comment;
        }
    },
    methods: {
        date_formatter(type, val) {
            if (type === 'hour') {
                return val + '点';
            }

            return val;
        },
        add_vichele_info: function () {
            this.vichele_info.splice(0, 0, {
                main_vichele: '',
                behind_vichele: '',
                driver_name: '',
                driver_phone: '',
                drop_address: '',
                use_for: '',
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
            return y + '-' + m + '-' + d + ' ' + h + '点';
        },
        confirm_time: function (_time) {
            this.plan_time = this.formatDateTime(_time);
            this.show_time_picker = false;
        },
        submit_plan_to_server: function () {
            var vue_this = this;
            if (this.is_create) {
                this.$call_remote_process("stuff_plan_management", 'create_plan', [{
                    type_id: this.type_id,
                    plan_time: this.plan_time,
                    vichele_info: this.vichele_info,
                    name: this.orig_name,
                    price: this.orig_price,
                    comment: this.comment,
                }, this.$cookies.get("pa_ssid"), vue_this.proxy_company]).then(function (resp) {
                    if (resp > 0) {
                        vue_this.$router.push({
                            name: 'CompanyOrder',
                        });
                    }
                });
            } else {
                vue_this.$call_remote_process("stuff_plan_management", 'update_plan', [{
                    plan_id: vue_this.plan_id,
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
        find_dup: function () {
            var ret = -1;
            var vue_this = this;
            for (let i = 0; i < this.vichele_info.length; i++) {
                const pro = this.vichele_info[i];
                ret = -1;
                this.vichele_info.forEach((element, index) => {
                    if (index != i) {
                        if (element.main_vichele == pro.main_vichele || element.behind_vichele == pro.behind_vichele || (element.driver_name == pro.driver_name && element.driver_phone == pro.driver_phone)) {
                            ret = i;
                            vue_this.$refs.vichele_ref[index].style["border-color"] = 'red';
                            vue_this.$refs.vichele_ref[i].style["border-color"] = 'red';
                        }
                    }
                });
                if (ret != -1) {
                    break;
                }
            }
            return ret;

        },
        submit_plan: function () {
            var vue_this = this;
            if (vue_this.vichele_info.length <= 0) {
                vue_this.$toast("请添加车辆信息");
            } else {
                vue_this.$refs.vichele_ref.forEach((element) => {
                    element.style["border-color"] = 'gray';
                });
                if (-1 != vue_this.find_dup()) {
                    var dup_id = vue_this.find_dup() + 1;
                    vue_this.$toast("车辆" + dup_id + "重复了");
                    return;
                }
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
                            confirmButtonText: '继续上报',
                            cancelButtonText: '再调整下'
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
    border-radius: 25px;
    margin-left: 10px;
    margin-right: 10px;
    margin-top: 10px;
    position: relative;
}

.vichele_index_show {
    position: absolute;
    top: 30%;
    left: 60%;
    font-size: 100px;
    z-index: 20;
    color: rgb(167, 167, 238);
    opacity: 0.5;
}
</style>
