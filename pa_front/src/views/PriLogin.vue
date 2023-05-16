<template>
<div class="pri_login_show">
    <van-field v-model="value" label="手机号">
    </van-field>
    <van-button block type="primary" @click="do_mock">切换</van-button>
</div>
</template>

<script>
export default {
    name: "PriLogin",
    data() {
        return {
            value: ''
        }
    },
    methods: {
        do_mock: function () {
            var vue_this = this;
            vue_this.$call_remote_process('user_management', "query_ssid_by_phone", [vue_this.value]).then(function (resp) {
                if (resp) {
                    vue_this.$cookies.set('pa_ssid', resp);
                    vue_this.$dialog.alert({
                        title: '提示',
                        message: '切换成功'
                    }).then(() => {
                        vue_this.$router.go(0);
                    });
                }
            });
        },
    }
}
</script>

<style>

</style>
