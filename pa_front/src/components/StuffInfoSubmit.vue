<template>
<div class="stuff_info_submit_show">
    <van-form @submit="submit_plan">
        <van-field center name="stepper" label="计划量(吨)">
            <template #input>
                <van-stepper decimal-length="1" v-model="plan_count" />
            </template>
        </van-field>
        <van-field center readonly clickable name="datetimePicker" :value="plan_time" label="到厂时间" placeholder="点击选择时间" @click="show_time_picker = true">
        </van-field>
        <van-popup v-model="show_time_picker" position="bottom">
            <van-datetime-picker type="datetime" title="请选择时间" :min-date="min_time" @cancel="show_time_picker = false" @confirm="confirm_time" />
        </van-popup>
        <van-row type="flex" justify="center" align="center">
            <van-col :span="20">
                <van-divider>运输车辆</van-divider>
            </van-col>
            <van-col :span="4">
                <van-button round size="small" type="primary" icon="plus" native-type="button" @click="show_vichele_picker = true"></van-button>
            </van-col>
        </van-row>
        <van-popup v-model="show_vichele_picker" round position="bottom">
            <van-picker show-toolbar :columns="bound_vichele" @cancel="show_vichele_picker = false" @confirm="add_vichele">
                <template #title>
                    <van-button type="info" native-type="button" @click="show_add_vichele_diag = true">添加车辆</van-button>
                </template>
            </van-picker>
        </van-popup>
        <span style="margin-left:16px" v-for="(single_vichele, index) in vichele_info" :key="index">
            <van-tag closeable size="large" type="primary" @close="remove_vichele(index)">
                {{single_vichele}}
            </van-tag>
        </span>
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit" :disabled="!stuff_info_change">{{action_name}}</van-button>
        </div>

    </van-form>
    <van-dialog v-model="show_add_vichele_diag" title="添加车辆" show-cancel-button @confirm="submit_new_vichele">
        <van-field v-model="new_vichele" placeholder="请输入车牌号" />
    </van-dialog>
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
        };
    },
    computed: {
        action_name: function () {
            if (this.is_create) {
                return "提交";
            } else {
                return "修改"
            }
        },
        stuff_info_change: function () {
            var ret = false;
            if (this.is_create) {
                ret = true;
            } else if (this.plan_time != this.orig_plan_time || this.plan_count != this.orig_plan_count || this.vichele_info.toString() != this.orig_vichele_info.toString()) {
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
            vue_this.vichele_info = _new_value.slice(0);
        },
    },
    beforeMount: function () {
        if (this.is_create) {
            this.plan_time = this.formatDateTime(new Date());
        } else {
            this.plan_time = this.orig_plan_time;
            this.plan_count = this.orig_plan_count;
            this.vichele_info = this.orig_vichele_info;
            this.vichele_info = this.orig_vichele_info.slice(0);
        }
        this.fetch_current_vichele();
    },
    methods: {
        fetch_current_vichele: function () {
            var vue_this = this;
            vue_this.$get_client("user_management").get_bound_vichele(vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.bound_vichele, index, element);
                });
            }).catch(function (err) {
                console.log(err);
            });
        },
        submit_new_vichele: function () {
            var vue_this = this;
            vue_this.$get_client("user_management").bind_new_vichele(vue_this.$cookies.get('pa_ssid'), vue_this.new_vichele).then(function (resp) {
                if (resp) {
                    vue_this.fetch_current_vichele();
                }
            }).catch(function (err) {
                console.log(err);
            });
        },
        add_vichele: function (_vichele) {
            if (this.vichele_info.indexOf(_vichele) == -1) {
                this.vichele_info.push(_vichele);
                this.show_vichele_picker = false;
            } else {
                this.$toast.fail('已添加');
            }
        },
        remove_vichele(_index) {
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
            var minute = date.getMinutes();
            minute = minute < 10 ? ('0' + minute) : minute;
            return y + '-' + m + '-' + d + ' ' + h + ':' + minute;
        },
        confirm_time: function (_time) {
            this.plan_time = this.formatDateTime(_time);
            this.show_time_picker = false;
        },
        submit_plan: function () {
            var vue_this = this;
            if (this.is_create) {
                this.$get_client("stuff_plan_management").create_plan({
                    count: this.plan_count,
                    type_id: this.type_id,
                    plan_time: this.plan_time,
                    vichele_info: this.vichele_info,
                    name: this.orig_name,
                    price:this.orig_price,
                }, this.$cookies.get("pa_ssid")).then(function (resp) {
                    vue_this.$router.push({
                        name: 'PlanDetail',
                        params: {
                            plan_id: resp
                        }
                    });
                }).catch(function (err) {
                    console.log(err);
                });
            } else {
                vue_this.$get_client("stuff_plan_management").update_plan({
                    plan_id: vue_this.plan_id,
                    count: this.plan_count,
                    plan_time: this.plan_time,
                    vichele_info: this.vichele_info,
                }, this.$cookies.get('pa_ssid')).then(function (resp) {
                    if (resp) {
                        vue_this.$router.back(-1);
                    }
                }).catch(function (err) {
                    console.log(err);
                });
            }
        }
    },
}
</script>

<style>

</style>
