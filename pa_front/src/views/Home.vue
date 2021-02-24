<template>
<div class="home">
    <van-button type="primary" @click="to_login">登陆</van-button>
</div>
</template>

<script>
import Vue from 'vue';
import { Button } from 'vant';

Vue.use(Button);
export default {
    name: 'Home',
    methods:{
        to_login() {
            this.$router.push({name:'Login'});
        }
    },
}
</script>
