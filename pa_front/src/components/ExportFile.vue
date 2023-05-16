<template>
<div class="export_file_show">
    <van-dialog v-model="ask_email_diag" title="请输入邮箱" show-cancel-button @confirm="send_email">
        <van-field v-model="export_email" label="邮箱" placeholder="请输入邮箱" />
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Dialog
} from 'vant';
import {
    Field
} from 'vant';
import VueClipboard from 'vue-clipboard2'

Vue.use(VueClipboard)
Vue.use(Field);
Vue.use(Dialog);
export default {
    name: 'ExportFile',
    data: function () {
        return {
            export_email: '',
        };
    },
    computed: {
        ask_email_diag: {
            set: function (_val) {
                this.$emit('change_display', _val);
            },
            get: function () {
                return this.show;
            },
        },
    },
    props: {
        show: Boolean,
        remote_file: String,
    },
    model: {
        prop: 'show',
        event: 'change_display',
    },
    methods: {
        send_email: function () {
            var vue_this = this;
            this.$copyText(this.remote_file).then(function () {
                Dialog.alert({
                    message: '链接已复制到剪贴板，请粘贴到浏览器下载',
                }).then(() => {
                    vue_this.$call_remote_process("stuff_plan_management", "send_file_via_email", [vue_this.$cookies.get('pa_ssid'), vue_this.remote_file, vue_this.export_email]).then(function (resp) {
                        if (resp) {
                            vue_this.$toast("邮件已发送");
                        }
                    });
                    vue_this.ask_email_diag = false;
                });
            });
        },
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$call_remote_process("user_management", "get_user_email", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            vue_this.export_email = resp;
        });
    },
}
</script>

<style>

</style>
