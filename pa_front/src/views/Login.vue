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
            vue_this.$get_client("user_management").user_login(vue_this.$route.query.code).then(function (resp) {
                vue_this.$cookies.set('pa_ssid', resp);
                window.location.replace("/pa_web/");
            }).catch(function (err) {
                console.log(err);
            });
        }
    },
}
</script>

<style>

</style>
