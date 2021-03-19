<template>
<div class="myself_show">
    <van-cell-group>
        <van-cell is-link :to="{name: 'SelfInfo'}" value="修改信息">
            <template #title>
                <div>
                    <van-row align="center" type="flex">
                        <van-col>
                            <van-image round width="60px" height="60px" :src="$store.state.userinfo.logo" />
                        </van-col>
                        <van-col>
                            <div class="name_show">{{$store.state.userinfo.name}}</div>
                            <div class="role_show">{{$store.state.userinfo.company}}</div>
                        </van-col>
                    </van-row>
                </div>
            </template>
        </van-cell>
    </van-cell-group>
    <van-button v-if="$store.state.is_login" type="danger" @click="logoff" block>退出登录</van-button>
    <van-button v-else type="primary" @click="login" block>登录</van-button>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup,
    Col,
    Row
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Button
} from 'vant';

Vue.use(Button);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Col);
Vue.use(Row);
Vue.use(VanImage);
export default {
    name: 'Myself',
    methods: {
        logoff: function () {
            var vue_this = this;
            vue_this.$get_client("user_management").logff_user(vue_this.$cookies.get('pa_ssid')).finally(function () {
                history.go(0);
            });
        },
        login: function () {
            window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web%2flogin_mp&response_type=code&scope=snsapi_userinfo&state=STATE#wechat_redirect"
        }
    },
}
</script>

<style>

</style>
