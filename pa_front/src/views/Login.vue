<template>
<div class="login_show">

</div>
</template>

<script>
import {
    get_client
} from '@/plugins/rpc_helper.js'
export default {
    name: 'Login',
    beforeMount() {
        var vue_this = this;
        if (vue_this.$route.query.code) {
            vue_this.$get_client("user_management").user_login(vue_this.$route.query.code).then(function (resp) {
                vue_this.$cookies.set('pa_ssid', resp);
                vue_this.get_userinfo();
            }).catch(function (err) {
                console.log(err);
            });
        }
    },
    methods: {
        get_userinfo() {
            var vue_this = this;
            vue_this.$toast.loading({
                message: '加载中...',
                forbidClick: true,
                duration: 0,
            });
            var ssid = vue_this.$cookies.get('pa_ssid');
            get_client('user_management').get_user_info(ssid).then(function (resp) {
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
                vue_this.$router.replace({name:'Home'});
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
