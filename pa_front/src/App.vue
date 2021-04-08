<template>
<div id="app">
    <van-nav-bar :left-arrow="has_go_back" :title="bar_title" @click-left="onClickLeft" @click-right="onClickRight">
        <template #right>
            <van-icon name="share-o" size="20"></van-icon>
        </template>
    </van-nav-bar>
    <van-pull-refresh v-model="isLoading" @refresh="onRefresh">
        <router-view />
    </van-pull-refresh>
    <van-tabbar route fixed placeholder>
        <van-tabbar-item v-if="$store.state.userinfo.buyer" replace :to="{name:'Home'}" icon="home-o">主页</van-tabbar-item>
        <van-tabbar-item v-else replace :to="{name:'CompanyHome'}" icon="home-o">主页</van-tabbar-item>
        <van-tabbar-item v-if="$store.state.userinfo.buyer" replace :to="{name:'Order'}" icon="orders-o">订单</van-tabbar-item>
        <van-tabbar-item v-else replace :to="{name:'CompanyOrder'}" icon="orders-o">订单</van-tabbar-item>
        <van-tabbar-item replace :to="{name:'Myself'}" icon="user-o">我的</van-tabbar-item>
    </van-tabbar>
    <van-dialog v-model="show_share" title="长按图片发送给联系人">
        <van-row type="flex" justify="center" align="center">
            <img src="https://www.d8sis.cn/pa_web/logo_res/sub_qr.jpg" />
        </van-row>
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    NavBar,
    Tabbar,
    TabbarItem
} from 'vant';

import {
    Icon
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Toast
} from 'vant';
import {
    Dialog
} from 'vant';

import {
    Col,
    Row
} from 'vant';
import {
    PullRefresh
} from 'vant';

Vue.use(PullRefresh);
Vue.use(Col);
Vue.use(Row);
Vue.use(Dialog);
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
            buyer: true,
            need_info: false,
            show_share: false,
            isLoading:false,
        }
    },
    beforeMount: function () {
        var vue_this = this;
        this.$router.onReady(function () {
            vue_this.get_userinfo();
        });
    },
    watch: {
        $route: function (to) {
            this.bar_title = to.meta.private_title;
            this.has_go_back = to.meta.has_go_back;
        },
    },
    methods: {
        onRefresh: function() {
            this.$router.go(0);
        },
        onClickRight: function () {
            this.show_share = true;
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
                    vue_this.$store.commit('set_userinfo', resp);
                    vue_this.$store.commit('set_login', true);
                    if (!resp.phone) {
                        vue_this.need_info = true;
                        vue_this.$router.replace({
                            name: 'SelfInfo',
                            query: {
                                from: 'auto'
                            }
                        })
                    }
                } else {
                    vue_this.$store.commit('set_userinfo', {
                        buyer: true,
                    });
                    vue_this.$store.commit('set_login', false);
                }
                vue_this.$toast.clear();
            }).catch(function (err) {
                vue_this.$toast.clear();
                console.log(err);
                vue_this.$toast(err.msg);
            }).finally(function () {
                if (vue_this.$route.path == '/' && vue_this.need_info == false) {
                    if (vue_this.$store.state.userinfo.buyer) {
                        vue_this.$router.replace({
                            name: 'Home'
                        });
                    } else {
                        vue_this.$router.replace({
                            name: 'CompanyHome'
                        });
                    }

                }
            });

        },
    },
}
</script>

<style>
</style>
