<template>
<dev class="welcome">
    <van-image width="100" height="100" :src="$remote_url + '/company_logo/logo01707_1877.jpg'" />
    <h1>掌易助手</h1>
    <div class="notice_show">
        登陆后即可参与公司流程
    </div>
    <van-button type="primary" block round @click="trigger_wx_login">授权微信登陆</van-button>
</dev>
</template>

<script>
import Vue from 'vue';
import {
    Image as VanImage
} from 'vant';
import {
    Button
} from 'vant';

Vue.use(Button);
Vue.use(VanImage);
export default {
    name: 'Welcome',
    methods: {
        trigger_wx_login: function () {
            var company_from_url = this.$route.query.company;
            window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web&response_type=code&scope=snsapi_userinfo&state=" + company_from_url + "#wechat_redirect"
        },
    },
}
</script>

<style>

</style>
