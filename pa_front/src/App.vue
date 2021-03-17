<template>
<div id="app">
    <van-nav-bar :left-arrow="has_go_back" :title="bar_title" @click-left="onClickLeft" @click-right="onClickRight">
        <template #right>
            <van-icon name="share-o" size="20"></van-icon>
        </template>
    </van-nav-bar>
    <router-view />
    <van-tabbar route>
        <van-tabbar-item replace :to="{name:'Home'}" icon="home-o">主页</van-tabbar-item>
        <van-tabbar-item replace :to="{name:'Order'}" icon="orders-o">订单</van-tabbar-item>
        <van-tabbar-item replace :to="{name:'Myself'}" icon="user-o">我的</van-tabbar-item>
    </van-tabbar>
</div>
</template>

<script>
import Vue from 'vue';
import {
    NavBar,
    Tabbar,
    TabbarItem
} from 'vant';
import wx from 'weixin-js-sdk'

import {
    Icon
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Toast
} from 'vant';

Vue.use(Toast);
Vue.use(VanImage);
Vue.use(Icon);
Vue.use(Tabbar);
Vue.use(TabbarItem);

Vue.use(NavBar);
export default {
    data: function () {
        return {
            bar_title: '',
            has_go_back: false,
            is_login: false,
        }
    },
    beforeMount: function () {
        this.get_userinfo();
    },
    watch: {
        $route: function (to) {
            this.bar_title = to.meta.private_title;
            this.has_go_back = to.meta.has_go_back;
        },
    },
    methods: {
        onClickRight: function () {
            wx.miniProgram.navigateTo({
                url: '/pages/share/share'
            });
        },
        onClickLeft() {
            this.$router.back(-1);
        },
        get_userinfo() {
            var vue_this = this;
            vue_this.$toast.loading({
                message: '加载中...',
                forbidClick: true,
                duration: 0,
            });
            var ssid = vue_this.$cookies.get('pa_ssid');
            vue_this.$get_client('user_management').get_user_info(ssid).then(function (resp) {
                console.log(resp);
                if (resp.user_id != 0) {
                    vue_this.$store.commit('set_userinfo', {
                        is_login: true,
                        name: resp.name,
                        buyer: resp.buyer,
                        company: resp.company,
                        logo: resp.logo,
                        phone: resp.phone,
                    });
                } else {
                    vue_this.$store.commit('set_userinfo', {
                        is_login: false,
                    });
                }
            }).catch(function (err) {
                console.log(err);
            }).finally(function () {
                vue_this.$toast.clear();
            });

        },
    },
}
</script>

<style>
</style>
