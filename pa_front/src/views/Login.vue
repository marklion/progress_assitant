<template>
<div class="login_show">

</div>
</template>

<script>
import wx from 'weixin-js-sdk'
export default {
    name: 'Login',
    beforeMount() {
        wx.miniProgram.navigateTo({
            url: '/pages/index/index?company_id=' + this.$route.query.company
        });
    },
}
</script>

<style>

</style>
