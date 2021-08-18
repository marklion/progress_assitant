<template>
<div class="third_dev_show">
    <van-form @submit="set_dev_info">
        <van-field v-model="key" label="key" />
        <van-field v-model="url" label="url" />
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
            url: '',
        };
    },
    methods: {
        init_dev_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'get_third_info', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.key = resp.key;
                vue_this.url = resp.url;
            });
        },
        set_dev_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'set_third_info', [vue_this.key, vue_this.url, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
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
