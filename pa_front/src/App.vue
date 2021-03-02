<template>
<div id="app">
    <van-nav-bar v-if="has_go_back" :title="bar_title" left-text="返回" left-arrow @click-left="onClickLeft" />
    <van-nav-bar v-else :title="bar_title" />
    <router-view />
    <van-tabbar route>
        <van-tabbar-item replace :to="{name:'Home'}" icon="home-o">主页</van-tabbar-item>
        <van-tabbar-item replace :to="{name:'Application'}" icon="apps-o">应用</van-tabbar-item>
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
        var vue_this = this;
        this.$router.onReady(function () {
            console.log(vue_this.$route);
            console.log(vue_this.$store.state);
            if (vue_this.$route.query.code) {
                vue_this.$axios.post('/wechat_login', {
                    code: vue_this.$route.query.code
                }).then(function (resp) {
                    vue_this.$cookies.set('pa_ssid', resp.data.result);
                    vue_this.get_userinfo();
                    vue_this.$router.replace({
                        name: 'Home',
                        query: {
                            company: vue_this.$route.query.state
                        }
                    });
                }).catch(function (err) {
                    console.log(err);
                });
            } else {
                vue_this.get_userinfo();
            }
        });
        this.config_with_wx();
    },
    watch: {
        $route: function (to) {
            this.bar_title = to.meta.private_title;
            this.has_go_back = to.meta.has_go_back;
        },
        "$store.state.userinfo.company": function() {
            this.config_with_wx();
        },
    },
    methods: {
        onClickLeft() {
            this.$router.back(-1);
        },
        get_userinfo() {
            var vue_this = this;
            var ssid = vue_this.$cookies.get('pa_ssid');
            vue_this.$axios.get('userinfo/' + ssid).then(function (resp) {
                if (resp.data.result.online == true) {
                    vue_this.is_login = true;
                    vue_this.$store.commit('set_userinfo', {
                        name: resp.data.result.name,
                        company: resp.data.result.company,
                        role: resp.data.result.role,
                        logo: vue_this.$remote_url + resp.data.result.logo,
                    });
                } else {
                    var company_from_url = vue_this.$route.query.company;
                    window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=http%3a%2f%2fwww.d8sis.cn%2fpa_web&response_type=code&scope=snsapi_userinfo&state=" + company_from_url + "#wechat_redirect"
                }
            }).catch(function (err) {
                console.log(err);
                var company_from_url = vue_this.$route.query.company;
                window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=http%3a%2f%2fwww.d8sis.cn%2fpa_web&response_type=code&scope=snsapi_userinfo&state=" + company_from_url + "#wechat_redirect"
            });
        },
        randomString: function (len) {
            len = len || 32;
            var $chars = 'ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678'; /****默认去掉了容易混淆的字符oOLl,9gq,Vv,Uu,I1****/
            var maxPos = $chars.length;
            var pwd = '';
            for (var i = 0; i < len; i++) {
                pwd += $chars.charAt(Math.floor(Math.random() * maxPos));
            }
            return pwd;
        },
        config_with_wx: function () {
            var vue_this = this;
            var timestamp = (new Date()).getTime();
            var nonceStr = this.randomString(32);
            this.$axios.post('/pa_wx_sign', {
                timestamp: timestamp,
                nonceStr: nonceStr,
                url: window.location.href,
            }).then(function (resp) {
                wx.config({
                    debug: false,
                    appId: 'wxa390f8b6f68e9c6d',
                    timestamp: timestamp,
                    nonceStr: nonceStr,
                    signature: resp.data.result,
                    jsApiList: ['updateAppMessageShareData']
                });
                wx.ready(function () {
                    console.log('success to config wx');
                    vue_this.$axios.get('/company_id/' + vue_this.$store.state.userinfo.company).then(function (resp) {
                        wx.updateAppMessageShareData({
                            title: '流程助手',
                            desc: vue_this.$store.state.userinfo.company,
                            imgUrl: '',
                            link: 'http://' + window.location.host + window.location.pathname + '?company=' + resp.data.result,
                            success: function () {
                                console.log('success to set share btn');
                            }
                        });

                    }).catch(function (err) {
                        console.log(err);
                    });
                });
                wx.error(function (err) {
                    console.log('fail to config wx');
                    console.log(err);
                });
            }).catch(function (err) {
                console.log(err);
            });
        },
    },
}
</script>

<style>
</style>
