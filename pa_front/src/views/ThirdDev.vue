<template>
<div class="third_dev_show">
    <van-form @submit="set_dev_info">
        <van-field v-model="key" label="key" />
        <van-field v-model="token" label="token" />
        <van-field v-model="ctrl_url" label="ctrl_url" />
        <van-field v-model="dms_url" label="dms_url" />
        <van-field v-model="zc_url" label="zc_url" />
        <van-field v-model="zh_ssid" label="zh_ssid" />
        <van-field v-model="remote_event_url" label="事件服务url" />
        <van-field v-model="zc_rpc_url" label="zc_rpc_url" />
        <van-field v-model="zczh_back_end" label="zczh_back_end" />
        <van-field v-model="zczh_back_token" label="zczh_back_token" />
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">提交</van-button>
        </div>
    </van-form>
    <van-row type="flex" align="center" justify="space-between">
        <van-col :span="20">
            <van-divider>订阅事件</van-divider>
        </van-col>
        <van-col :span="4">
            <van-button block type="primary" size="small" @click="add_event_pop_show = true">增加</van-button>
        </van-col>
    </van-row>
    <van-cell v-for="(single_event, index) in event_types" :key="index" :title="single_event">
        <template #right-icon>
            <van-button type="danger" size="small" @click="del_event(single_event)">删除</van-button>
        </template>
    </van-cell>
    <van-popup v-model="add_event_pop_show" position="bottom">
        <van-picker show-toolbar :columns="['p_weight', 'm_weight','deliever']" @confirm="add_event" @cancel="add_event_pop_show = false" />
    </van-popup>

    <van-row type="flex" align="center" justify="space-between">
        <van-col :span="20">
            <van-divider>GPS定位物料</van-divider>
        </van-col>
        <van-col :span="4">
            <van-button block type="primary" size="small" @click="show_new_stuff_diag = true">增加</van-button>
        </van-col>
    </van-row>
    <van-cell v-for="(single_stuff, index) in gps_stuff" :key="index" :title="single_stuff">
        <template #right-icon>
            <van-button type="danger" size="small" @click="del_gps_stuff(single_stuff)">删除</van-button>
        </template>
    </van-cell>
    <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="show_new_stuff_diag" title="新增GPS物料">
        <van-form @submit="add_gps_stuff">
            <van-field v-model="new_stuff" name="物料名" label="物料名" placeholder="请输入物料名" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">确认</van-button>
            </div>
        </van-form>
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
    Button
} from 'vant';
import {
    Divider
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Dialog
} from 'vant';

// 全局注册
Vue.use(Dialog);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Col);
Vue.use(Row);
Vue.use(Divider);
Vue.use(Button);
Vue.use(Form);
Vue.use(Field);
export default {
    name: 'ThirdDev',
    data: function () {
        return {
            key: '',
            ctrl_url: '',
            dms_url: '',
            token: '',
            zh_ssid: "",
            remote_event_url: "",
            zc_url: "",
            zc_rpc_url: "",
            zczh_back_end:"",
            zczh_back_token:"",
            gps_stuff: [],
            new_stuff: '',
            show_new_stuff_diag: false,
            add_event_pop_show: false,
            event_types: [],
        };
    },
    methods: {
        add_event: function (_event_name) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "add_event_sub", [vue_this.$cookies.get('pa_ssid'), _event_name]).then(function (resp) {
                if (resp) {
                    vue_this.init_event_types();
                    vue_this.add_event_pop_show = false;
                }
            });
        },
        del_event: function (_event_name) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "del_event_sub", [vue_this.$cookies.get('pa_ssid'), _event_name]).then(function (resp) {
                if (resp) {
                    vue_this.init_event_types();
                }
            });
        },
        init_dev_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'get_third_info', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.key = resp.key;
                vue_this.ctrl_url = resp.ctrl_url;
                vue_this.dms_url = resp.dms_url;
                vue_this.token = resp.token;
                vue_this.zc_url = resp.zc_url;
                vue_this.zc_rpc_url = resp.zc_rpc_url;
                vue_this.zczh_back_end = resp.zczh_back_end;
                vue_this.zczh_back_token = resp.zczh_back_token;
                vue_this.zh_ssid = resp.zh_ssid;
                vue_this.remote_event_url = resp.remote_event_url;
            });
        },
        set_dev_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'set_third_info', [{
                key: vue_this.key,
                ctrl_url: vue_this.ctrl_url,
                dms_url: vue_this.dms_url,
                token: vue_this.token,
                zh_ssid: vue_this.zh_ssid,
                zc_url: vue_this.zc_url,
                zc_rpc_url: vue_this.zc_rpc_url,
                zczh_back_end: vue_this.zczh_back_end,
                zczh_back_token: vue_this.zczh_back_token,
                remote_event_url: vue_this.remote_event_url,
            }, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_dev_info();
                    vue_this.$toast("操作成功");
                }
            });
        },
        add_gps_stuff: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "add_gps_stuff", [vue_this.$cookies.get('pa_ssid'), vue_this.new_stuff]).then(function (resp) {
                if (resp) {
                    vue_this.show_new_stuff_diag = false;
                    vue_this.init_gps_stuff();
                }
            });
        },
        del_gps_stuff: function (_stuff) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "del_gps_stuff", [vue_this.$cookies.get('pa_ssid'), _stuff]).then(function (resp) {
                if (resp) {
                    vue_this.init_gps_stuff();
                }
            });
        },
        init_gps_stuff: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_gps_stuff", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.gps_stuff = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.gps_stuff, index, element);
                });
            });
        },
        init_event_types: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_event_sub", [vue_this.$store.state.userinfo.company]).then(function (resp) {
                vue_this.event_types = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.event_types, index, element);
                });
            });
        },
    },
    beforeMount: function () {
        this.init_dev_info();
        this.init_gps_stuff();
        this.init_event_types();
    },
}
</script>

<style>

</style>
