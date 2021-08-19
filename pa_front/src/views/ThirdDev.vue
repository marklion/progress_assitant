<template>
<div class="third_dev_show">
    <van-form @submit="set_dev_info">
        <van-field v-model="key" label="key" />
        <van-field v-model="token" label="token" />
        <van-field v-model="ctrl_url" label="ctrl_url" />
        <van-field v-model="dms_url" label="dms_url" />
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">提交</van-button>
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
import { Button } from 'vant';

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
        };
    },
    methods: {
        init_dev_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'get_third_info', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.key = resp.key;
                vue_this.ctrl_url = resp.ctrl_url;
                vue_this.dms_url = resp.dms_url;
                vue_this.token = resp.token;
            });
        },
        set_dev_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'set_third_info', [{
                key:vue_this.key,
                ctrl_url: vue_this.ctrl_url,
                dms_url: vue_this.dms_url,
                token: vue_this.token,
            }, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_dev_info();
                    vue_this.$toast("操作成功");
                }
            });
        },
    },
    beforeMount: function () {
        this.init_dev_info();
    },
}
</script>

<style>

</style>
