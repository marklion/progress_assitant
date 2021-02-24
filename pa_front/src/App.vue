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

Vue.use(Tabbar);
Vue.use(TabbarItem);

Vue.use(NavBar);
export default {
    data: function () {
        return {
            bar_title: '',
            has_go_back: false,
        }
    },
    beforeMount: function () {
    },
    watch: {
        $route: function(to, from) {
            this.bar_title = to.meta.private_title;
            this.has_go_back = to.meta.has_go_back;
            console.log(from);
        }
    },
    methods: {
        onClickLeft() {
            this.$router.back(-1);
        },
    },
}
</script>

<style>
</style>
