<template>
<div class="sec_check_diag_show">
    <van-notice-bar text="请完善信息，并等待审批" />
    <sec-check-cell v-for="(single_lr, index) in  lrs" :key="index" :lr="single_lr" :mv="mv" :bv="bv" :driver="driver"></sec-check-cell>
</div>
</template>

<script>
import SecCheckCell from '@/components/SecCheckCell'
export default {
    name: "SecCheckDiag",
    components: {
        "sec-check-cell": SecCheckCell,
    },
    data: function () {
        return {
            lrs: [],
        };
    },
    props: {
        company: String,
        mv: String,
        bv: String,
        driver: String,
    },
    methods: {
        init_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_license_require", [vue_this.company]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.lrs, index, element);
                });
            });
        },
    },
    beforeMount: function () {
        this.init_data();
    },
}
</script>

<style scoped>
.sec_check_diag_show {
    overflow: auto;
    height: 80vh;
}
</style>
