<template>
<div class="admin_show">
    <van-tabs v-model="active">
        <van-tab title="待审批">
            <van-cell v-for="(single_apply, index) in all_apply " :key="index" :label="'手机号:' + single_apply.phone" :title="single_apply.name + ' 加入公司申请'">
                <template #right-icon>
                    <van-row v-if="single_apply.status == 0" type="flex" justify="center" align="center" :gutter="5">
                        <van-col>
                            <van-button type="primary" @click="submit_approve(single_apply, true)">批准</van-button>
                        </van-col>
                        <van-col>
                            <van-button type="danger" @click="submit_approve(single_apply, false)">驳回</van-button>
                        </van-col>
                    </van-row>
                    <div v-else-if="single_apply.status == 1">
                        已批准
                    </div>
                    <div v-else>
                        已驳回
                    </div>
                </template>
                <template #icon>
                    <van-image round width="40px" height="40px" :src="$remote_url + single_apply.logo" />
                </template>
            </van-cell>
        </van-tab>
        <van-tab title="所有员工">
            <van-cell v-for="(single_user, index) in all_user" :key="index" :label="'手机号' + single_user.phone" :title="single_user.name">
                <template #right-icon>
                    <van-button type="danger" plain size="small" @click="remove_user(single_user)">移除用户</van-button>
                </template>
            </van-cell>
        </van-tab>
        <van-tab title="营业时间">
            <van-field readonly clickable :value="work_start_time" label="上班时间" placeholder="点击选择时间" @click="show_start_Picker = true" />
            <van-popup v-model="show_start_Picker" position="bottom">
                <van-datetime-picker type="time" @confirm="on_start_Confirm" @cancel="show_start_Picker = false" />
            </van-popup>
            <van-field readonly clickable :value="work_end_time" label="下班时间" placeholder="点击选择时间" @click="show_end_Picker = true" />
            <van-popup v-model="show_end_Picker" position="bottom">
                <van-datetime-picker type="time" @confirm="on_end_Confirm" @cancel="show_end_Picker = false" />
            </van-popup>
        </van-tab>
    </van-tabs>

</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Button
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Tab,
    Tabs
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Field
} from 'vant';
import {
    Popup
} from 'vant';
import {
    DatetimePicker
} from 'vant';

Vue.use(DatetimePicker);
Vue.use(Popup);
Vue.use(Field);
Vue.use(Tab);
Vue.use(Tabs);
Vue.use(Col);
Vue.use(Row);
Vue.use(Button);
Vue.use(VanImage);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'Admin',
    data: function () {
        return {
            all_apply: [],
            active: 0,
            all_user: [],
            work_start_time: 0,
            show_start_Picker: false,
            work_end_time: 0,
            show_end_Picker: false,
        };
    },
    methods: {
        on_start_Confirm: function (_time) {
            var start_time = parseInt(_time.split(":")[0]) * 60 + parseInt(_time.split(":")[1]);
            var end_time = parseInt(this.work_end_time.split(":")[0]) * 60 + parseInt(this.work_end_time.split(":")[1]);
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "set_work_time", [vue_this.$cookies.get('pa_ssid'), start_time, end_time]).then(function (resp) {
                if (resp) {
                    vue_this.init_work_time();
                    vue_this.show_start_Picker = false;
                }
            })
        },
        on_end_Confirm: function (_time) {
            var start_time = parseInt(this.work_start_time.split(":")[0]) * 60 + parseInt(this.work_start_time.split(":")[1]);
            var end_time = parseInt(_time.split(":")[0]) * 60 + parseInt(_time.split(":")[1]);
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "set_work_time", [vue_this.$cookies.get('pa_ssid'), start_time, end_time]).then(function (resp) {
                if (resp) {
                    vue_this.init_work_time();
                    vue_this.show_end_Picker = false;
                }
            })
        },
        remove_user: function (_user) {
            var vue_this = this;
            Dialog.confirm({
                message: '确定要移除' + _user.name + '吗',
            }).then(() => {
                vue_this.$call_remote_process("company_management", "remove_user_from_company", [vue_this.$cookies.get('pa_ssid'), _user.user_id]).then(function (resp) {
                    if (resp) {
                        vue_this.init_all_user();
                    }
                });
            });
        },
        submit_approve: function (_apply, approve) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'approve_apply', [_apply.apply_id, vue_this.$cookies.get('pa_ssid'), approve]).then(function (resp) {
                if (resp) {
                    vue_this.init_apply_info();
                }
            });
        },
        init_apply_info: function () {
            var vue_this = this;
            vue_this.all_apply = [];
            vue_this.$call_remote_process("company_management", 'get_all_apply', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_apply, index, element);
                });
            });
        },
        init_all_user: function () {
            var vue_this = this;
            vue_this.all_user = [];
            vue_this.$call_remote_process("company_management", "get_all_compay_user", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_user, index, element);
                });
            });
        },
        init_work_time: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_work_time", [vue_this.$store.state.userinfo.company]).then(function (resp) {
                vue_this.work_start_time = parseInt(resp.start_time / 60) + ':' + resp.start_time % 60;
                vue_this.work_end_time = parseInt(resp.end_time / 60) + ':' + resp.end_time % 60;
            });
        },
    },
    beforeMount: function () {
        this.init_apply_info();
        this.init_all_user();
        this.init_work_time();
    },
}
</script>

<style>

</style>
