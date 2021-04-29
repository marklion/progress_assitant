<template>
<div class="plan_operate_show">
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>当前状态</van-col>
                <van-col v-if="$store.state.userinfo.buyer || is_proxy">
                    <van-button v-if="status == 0" size="small" type="warning" :to="{name:'PlanUpdate', params:{plan_id:plan_id}}">修改计划</van-button>
                    <van-uploader v-else-if="status == 2 || status == 1" :max-count="1" :after-read="upload_payinfo">
                        <van-button icon="plus" type="primary" size="small">上传付款凭证</van-button>
                    </van-uploader>
                    <van-button v-else-if="status == 3" size="small" type="info" icon="qr" @click="show_qr = true">展示提货码</van-button>
                </van-col>
                <van-col v-else>
                    <van-button v-if="status == 3" size="small" type="info" icon="scan" @click="confirm_close">确认提货</van-button>
                </van-col>
            </van-row>
        </template>

        <van-cell :value="cur_status" />
    </van-cell-group>
    <div style="margin: 16px;" v-if="has_pri">
        <van-row type="flex" justify="center" align="center" :gutter="10" v-if="status == 0">
            <van-col :span="12">
                <van-button round block type="danger" @click="show_reject_reason_diag = true">驳回计划</van-button>
            </van-col>
            <van-col :span="12">
                <van-button round block type="primary" @click="submit_confirm">确认计划</van-button>
            </van-col>
        </van-row>
        <van-row type="flex" justify="center" align="center" :gutter="10" v-if="status == 2">
            <van-col :span="12">
                <van-button round block type="danger" @click="show_reject_reason_diag = true">驳回付款</van-button>
            </van-col>
            <van-col :span="12">
                <van-button round block type="primary" @click="submit_confirm_pay">确认收款</van-button>
            </van-col>
        </van-row>
        <van-button v-if="status == 3 && is_proxy" round block type="primary" :to="{name:'ClosePlan', params:{plan_id:plan_id}}">确认收货</van-button>
    </div>
    <van-dialog v-model="show_qr" title="提货码">
        <van-row type="flex" justify="center" align="center">
            <vue-qr :text="$remote_url + '/close_plan/' + plan_id" :margin="0" colorDark="#f67b29" colorLight="#fff" :logoScale="0.3" :size="200"></vue-qr>
        </van-row>
    </van-dialog>
    <van-dialog v-model="show_reject_reason_diag" title="确认驳回" closeOnClickOverlay :showConfirmButton="false">
        <van-form @submit="reject_plan">
            <van-field v-model="reject_reason" name="驳回原因" label="驳回原因" placeholder="请输入驳回原因" :rules="[{ required: true, message: '请填写驳回原因' }]" />
            <van-button plain block>确认</van-button>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import vueQr from 'vue-qr'
import Vue from 'vue';
import {
    Button
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Uploader
} from 'vant';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Dialog
} from 'vant';

// 全局注册
import {
    compressAccurately
} from 'image-conversion';
Vue.use(Dialog);
Vue.use(Field);
Vue.use(Form);
Vue.use(Uploader);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Button);
import wx from 'weixin-js-sdk'
export default {
    name: 'PlanOperate',
    data: function () {
        return {
            has_pri: false,
            show_qr: false,
            show_reject_reason_diag: false,
            reject_reason: '',
        };
    },
    props: {
        plan_id: Number,
        status: Number,
        is_proxy: Boolean,
    },
    components: {
        "vue-qr": vueQr,
    },
    computed: {
        cur_status: function () {
            var ret = "等待商家确认计划";
            switch (this.status) {
                case 1:
                    ret = "等待买方付款";
                    break;
                case 2:
                    ret = "等待商家确认付款";
                    break;
                case 3:
                    ret = "等待买方提货";
                    break;
                case 4:
                    ret = "已提货";
                    break;
                case 5:
                    ret = "已撤销";
                    break;
                default:
                    break;
            }

            return ret;
        }
    },
    watch: {
        plan_id(_id) {
            this.get_priv(_id);
        },
    },
    methods: {
        reject_plan: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "reject_plan", [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), vue_this.reject_reason]).then(function (resp) {
                if (resp) {
                    vue_this.show_reject_reason_diag = false;
                    vue_this.$router.go(0);
                }
            });
        },
        randomString: function (len) {
            len = len || 32;
            var $chars = 'ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678'; /****默认去掉了容易混淆的字符oOLl,9gq,Vv,Uu,I1****/
            var maxPos = $chars.length;
            var pwd = '';
            for (var i = 0; i < len; i++) {
                pwd += $chars.charAt(Math.floor(Math.random() * maxPos));
            }
            return pwd;
        },
        config_with_wx: function () {
            var timestamp = (new Date()).getTime();
            var nonceStr = this.randomString(32);
            var vue_this = this;
            vue_this.$call_remote_process("user_management", 'get_wx_api_signature', [timestamp, nonceStr, window.location.href]).then(function (resp) {
                wx.config({
                    debug: false,
                    appId: 'wxa390f8b6f68e9c6d',
                    timestamp: timestamp,
                    nonceStr: nonceStr,
                    signature: resp,
                    jsApiList: ['scanQRCode']
                });
                wx.ready(function () {
                    console.log('success to config wx');
                });
                wx.error(function (err) {
                    console.log('fail to config wx');
                    console.log(err);
                });
            });
        },
        confirm_close: function () {
            wx.scanQRCode({
                needResult: 1,
                success: function (res) {
                    var dest_url = res.resultStr;
                    window.location.href = dest_url;
                }
            });
        },
        upload_payinfo: function (_file) {
            var vue_this = this;
            compressAccurately(_file.file, 400).then(function (res) {
                var reader = new FileReader();
                reader.readAsDataURL(res);
                reader.onloadend = function () {
                    vue_this.postImg(this.result);
                };
            });
        },
        postImg(base64) {
            var file_content = base64.split(';base64,')[1];
            var vue_this = this;
            this.$call_remote_process("stuff_plan_management", 'upload_payinfo', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), file_content]).
            then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },
        get_priv: function (_id) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'has_priv_edit', [_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.has_pri = resp;
            });
        },
        submit_confirm: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'confirm_plan', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },
        submit_confirm_pay: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'confirm_pay', [vue_this.plan_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },
    },
    beforeMount: function () {
        this.get_priv(this.plan_id);
        this.config_with_wx();
    }
}
</script>

<style>

</style>
