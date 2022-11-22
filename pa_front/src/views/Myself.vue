<template>
<div class="myself_show">
    <div v-if="$store.state.is_login" class="login_show">
        <div class="login_user_show">
            <van-row type="flex" align="center" :gutter="5">
                <van-col :offset="2" :span="6">
                    <van-image round width="80px" height="80px" fit="cover" :src="$remote_url +  $store.state.userinfo.logo" />
                </van-col>
                <van-col :span="13">
                    <van-row type="flex" align="center">
                        <h3>{{$store.state.userinfo.name}}</h3>
                        <div>{{$store.state.userinfo.company}}</div>
                    </van-row>
                </van-col>
                <van-col :span="3">
                    <van-button icon="edit" plain round size="small" :to="{name: 'SelfInfo'}" />
                </van-col>
            </van-row>
        </div>
        <van-divider />
        <van-cell-group>
            <van-cell v-if="is_admin" icon="manager-o" is-link :to="{name:'Admin'}" title="管理员菜单"></van-cell>
            <van-cell icon="records" is-link :to="{name:'Statistics'}" title="交易统计"></van-cell>
            <van-cell icon="info-o" is-link :to="{name:'BoundInfo'}" title="公司数据"></van-cell>
            <van-cell icon="sign" is-link :to="{name:'Contract'}" title="合同管理"></van-cell>
            <van-cell icon="aim" is-link :to="{name:'BiddingList'}" title="竞价管理"></van-cell>
            <van-cell v-if="!$store.state.userinfo.buyer" icon="flag-o" is-link @click="show_auth_code" title="授权码"></van-cell>
            <van-cell v-if="!$store.state.userinfo.buyer" icon="notes-o" is-link :to="{name:'CompanyExtraVichele'}" title="采购进厂"></van-cell>
            <van-cell v-if="!$store.state.userinfo.buyer" icon="hotel-o" is-link :to="{name:'SupplierInfo'}" title="供应商"></van-cell>
            <van-cell v-if="!$store.state.userinfo.buyer" icon="apps-o" is-link :to="{name:'ThirdDev'}" title="开发选项"></van-cell>
        </van-cell-group>
        <van-divider />

        <van-cell title="联系客服" icon="service-o" is-link url="https://work.weixin.qq.com/kfid/kfc2be35e14aff69f5f"></van-cell>
        <van-button type="danger" plain @click="logoff" block>退出登录</van-button>
    </div>
    <div v-else class="unlogin_show">
        <van-row type="flex" justify="center" align="center">
            <van-image round width="80px" height="80px" :src="$remote_url +  '/logo_res/logo.jpg'" />
        </van-row>
        <van-row type="flex" justify="center" align="center">
            <h1>掌易助理</h1>
        </van-row>
        <van-cell title="联系客服" icon="service-o" is-link url="https://work.weixin.qq.com/kfid/kfc2be35e14aff69f5f"></van-cell>
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
import {
    Dialog
} from 'vant';

import VueClipboard from 'vue-clipboard2'

Vue.use(VueClipboard)
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
        show_auth_code: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", "exchange_enc_text", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.$dialog.alert({
                    message: resp,
                    confirmButtonText: '复制'
                }).then(function () {
                    vue_this.$copyText(resp);
                });
            });
        },
        show_service_diag: function () {
            Dialog({
                message: '杨经理\n电话/微信: 18547707666'
            });
        },
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
