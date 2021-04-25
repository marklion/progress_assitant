<template>
<div class="myself_show">
    <div v-if="$store.state.is_login" class="login_show">
        <div class="login_user_show">
            <van-row type="flex" align="center" :gutter="5">
                <van-col :offset="2">
                    <van-image round width="80px" height="80px" fit="cover" :src="$remote_url +  $store.state.userinfo.logo" />
                </van-col>
                <van-col>
                    <h3>{{$store.state.userinfo.name}}</h3>
                </van-col>
                <van-col>
                    <div class="role_show">{{$store.state.userinfo.company}}</div>
                </van-col>
                <van-col>
                    <van-button icon="edit" plain round size="small" :to="{name: 'SelfInfo'}" />
                </van-col>
            </van-row>
        </div>
        <van-divider />
        <van-cell-group>
            <van-cell v-if="is_admin" icon="manager-o" is-link :to="{name:'Admin'}" title="管理员菜单"></van-cell>
            <van-cell icon="records" is-link :to="{name:'Statistics'}" title="交易统计"></van-cell>
            <van-cell icon="info-o" is-link :to="{name:'BoundInfo'}" title="公司数据"></van-cell>
        </van-cell-group>
        <van-divider />
        <van-button type="danger" plain @click="logoff" block>退出登录</van-button>
    </div>
    <div v-else class="unlogin_show">
        <van-row type="flex" justify="center" align="center">
            <van-image round width="80px" height="80px" :src="$remote_url +  '/logo_res/logo.jpg'" />
        </van-row>
        <van-row type="flex" justify="center" align="center">
            <h1>掌易助理</h1>
        </van-row>
        <div style="margin: 16px;">
            <van-button type="primary" @click="login" block>微信登录</van-button>
        </div>
    </div>
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
import {
    Icon
} from 'vant';
import {
    Divider
} from 'vant';

Vue.use(Divider);
Vue.use(Icon);
Vue.use(Button);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Col);
Vue.use(Row);
Vue.use(VanImage);
export default {
    name: 'Myself',
    data: function () {
        return {
            is_admin: false,
        };
    },
    methods: {
        logoff: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", 'logff_user', [vue_this.$cookies.get('pa_ssid')]).finally(function () {
                history.go(0);
            });
        },
        login: function () {
            window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web%2flogin_mp&response_type=code&scope=snsapi_userinfo&state=STATE#wechat_redirect"
        }
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$call_remote_process("user_management", 'is_admin', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            vue_this.is_admin = resp;
        });
    },
}
</script>

<style scoped>
.unlogin_show {
    margin-top: 100px;
}

.login_user_show {
    background-color: #43dbba;
}
</style>>
