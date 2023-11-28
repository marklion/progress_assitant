<template>
<div id="app">
    <router-view v-if="route_in && $route.meta.isPC" />
    <div v-if="route_in && !$route.meta.isPC">
        <van-nav-bar v-if="!$route.meta.no_title" class="nav_bar_show" :left-arrow="has_go_back" :left-text="get_left_text" :title="bar_title" @click-left="onClickLeft" @click-right="onClickRight">
            <template #right>
                <van-icon name="share" size="20"></van-icon>
            </template>
        </van-nav-bar>
        <van-pull-refresh v-model="isLoading" @refresh="onRefresh">
            <keep-alive>
                <router-view v-if='$route.meta.keepAlive' />
            </keep-alive>
            <router-view v-if='!$route.meta.keepAlive' />
        </van-pull-refresh>
        <div style="height: 6rem;"></div>
        <van-tabbar route fixed v-if="!$route.meta.extra_page">
            <van-tabbar-item v-if="$store.state.userinfo.buyer" replace :to="{name:'Home'}" icon="home-o">主页</van-tabbar-item>
            <van-tabbar-item v-else replace :to="{name:'CompanyHome'}" icon="home-o">主页</van-tabbar-item>
            <van-tabbar-item v-if="(!$store.state.userinfo.buyer && $store.state.userinfo.groupid == 1) || $store.state.userinfo.buyer" replace :to="{name:'CompanyOrder'}" icon="orders-o">订单</van-tabbar-item>
            <van-tabbar-item v-if="!$store.state.userinfo.buyer" replace :to="{name:'AppPanel'}" icon="apps-o">工作台</van-tabbar-item>
            <van-tabbar-item v-if="(!$store.state.userinfo.buyer && $store.state.need_sec_check == 1)" replace :to="{name:'SecCheckMobile'}" icon="eye-o">安检</van-tabbar-item>
            <van-tabbar-item replace :to="{name:'Myself'}" icon="user-o">我的</van-tabbar-item>
        </van-tabbar>
        <van-dialog v-model="show_share" title="长按图片发送给联系人">
            <van-row type="flex" justify="center" align="center">
                <img src="https://www.d8sis.cn/pa_web/logo_res/sub_qr.jpg" />
            </van-row>
        </van-dialog>
    </div>
</div>
</template>

<script>
export default {
    data: function () {
        return {
            bar_title: '',
            has_go_back: false,
            buyer: true,
            need_info: false,
            show_share: false,
            isLoading: false,
            route_in: false
        }
    },
    computed: {
        get_left_text: function () {
            var ret = "";
            if (this.has_go_back) {
                ret = "返回";
            }

            return ret;
        },
    },
    beforeMount: function () {
        this.$router.onReady(() => {
            this.route_in = true;
            this.get_userinfo();
        });
    },
    watch: {
        $route: function (to) {
            this.bar_title = to.meta.private_title;
            this.has_go_back = to.meta.has_go_back;
        },
    },
    methods: {
        onRefresh: function () {
            this.$router.go(0);
        },
        onClickRight: function () {
            this.show_share = true;
        },
        onClickLeft() {
            this.$router.back(-1);
        },
        get_userinfo() {
            if (this.$route.meta.extra_page && !this.$route.meta.permit_change) {
                return;
            }
            var vue_this = this;
            var ssid = vue_this.$cookies.get('pa_ssid');
            var req_func = vue_this.$call_remote_process;
            if (this.$route.meta.permit_change) {
                req_func = vue_this.$call_remote_process_no_toast;
            }
            req_func('user_management', 'get_user_info', [ssid]).then(function (resp) {
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
                    vue_this.$call_remote_process("company_management", "get_customize", [vue_this.$store.state.userinfo.company]).then(function (resp) {
                        vue_this.$store.commit('set_sec_check', resp.need_sec_check);
                        vue_this.$store.commit('set_need_buy_feature', resp.need_buy_feature);
                    });
                    vue_this.$call_remote_process("company_management", 'get_third_info', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                        vue_this.$store.commit('set_zc_rpc_url', resp.zc_rpc_url);
                        vue_this.$store.commit('set_zczh_back_end', resp.zczh_back_end);
                        vue_this.$store.commit('set_zczh_back_token', resp.zczh_back_token);
                    });
                } else {
                    vue_this.$store.commit('set_userinfo', {
                        buyer: true,
                    });
                    vue_this.$store.commit('set_login', false);
                }
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

<style scoped>
.nav_bar_show {
    background-color: #43dbba;
}

.nav_bar_show /deep/ .van-nav-bar__title {
    font-size: 20px;
    font-weight: bold;
}
</style>
