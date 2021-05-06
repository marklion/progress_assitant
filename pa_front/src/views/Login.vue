<template>
<div class="login_show">

</div>
</template>

<script>
export default {
    name: 'Login',
    beforeMount() {
        var vue_this = this;
        if (vue_this.$route.query.code) {
            vue_this.$call_remote_process("user_management",'user_login', [vue_this.$route.query.code]).then(function (resp) {
                vue_this.$cookies.set('pa_ssid', resp, '5d');
                window.location.replace("/pa_web/");
            });
        }
    },
}
</script>

<style>

</style>
