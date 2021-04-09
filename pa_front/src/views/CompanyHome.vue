<template>
<div class="company_home_show">
    <div v-if="!enter_company">
        <van-notice-bar scrollable text="请等待管理员批准加入公司" />
        <van-row type="flex" justify="center" align="center">
            <van-button round type="info" icon="replay" @click="refresh_page">刷新</van-button>
        </van-row>
    </div>
    <van-button plain round icon="scan" class="scan_button_show" @click="confirm_close"></van-button>
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>今日报价</van-col>
                <van-col>
                    <van-button type="primary" round icon="plus" @click="show_add_stuff = true">增加商品</van-button>
                </van-col>
            </van-row>
        </template>
        <van-cell v-for="(single_type, index) in all_type" center :key="index" :icon="stuff_status(single_type)" :value="'¥' + single_type.price" :title="single_type.name">
            <template #extra>
                <van-row type="flex" justify="center" align="center" :gutter="5">
                    <van-col>
                        <van-button size="small" type="info" @click="show_out_edit_diag(single_type)">编辑</van-button>
                    </van-col>
                    <van-col>
                        <van-button v-if="single_type.saling == 1" size="small" type="danger" @click="show_out_remove_diag(single_type)">下架</van-button>
                        <van-button v-else size="small" type="primary" @click="readd_stuff(single_type)">上架</van-button>
                    </van-col>
                </van-row>
            </template>
            <template #label>
                <van-tag plain type="danger">{{single_type.last}}</van-tag>
            </template>
        </van-cell>
    </van-cell-group>
    <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="show_add_stuff" title="添加商品">
        <van-form @submit="add_stuff">
            <van-field v-model="add_stuff_name" name="商品名" label="商品名" placeholder="请输入商品名" :rules="[{ required: true, message: '请填写商品名' }]" />
            <van-field v-model="add_stuff_price" type="number" name="价格" label="价格" placeholder="请输入价格" :rules="[{ required: true, message: '请填写价格' }]" />
            <van-field v-model="add_stuff_last" name="存量" label="存量" placeholder="请输入存量信息" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">确认</van-button>
            </div>
        </van-form>
    </van-dialog>
    <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="show_edit_stuff" title="修改商品">
        <van-form @submit="edit_stuff">
            <van-field v-model="stuff_in_edit.name" name="商品名" label="商品名" readonly />
            <van-field v-model="stuff_in_edit.price" name="价格" label="价格" placeholder="请输入价格" :rules="[{ required: true, message: '请填写价格' }]" />
            <van-field v-model="stuff_in_edit.last" name="存量" label="存量" placeholder="请输入存量信息" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">确认</van-button>
            </div>
        </van-form>
    </van-dialog>

</div>
</template>

<script>
import Vue from 'vue';
import {
    Col,
    Row
} from 'vant';
import {
    Field
} from 'vant';
import {
    Button
} from 'vant';
import {
    Popup
} from 'vant';
import {
    Form
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    NoticeBar
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Tag
} from 'vant';
import {
    Icon
} from 'vant';

Vue.use(Icon);
Vue.use(Tag);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(NoticeBar);
Vue.use(Dialog);
Vue.use(Form);
Vue.use(Popup);
Vue.use(Button);
Vue.use(Field);
Vue.use(Col);
Vue.use(Row);
import wx from 'weixin-js-sdk'
export default {
    name: 'CompanyHome',
    data: function () {
        return {
            all_type: [],
            show_add_stuff: false,
            add_stuff_name: '',
            add_stuff_price: '',
            add_stuff_last: '',
            stuff_in_edit: {
                name: '',
                price: 0,
                last: '',
            },
            show_edit_stuff: false,
            show_remove_stuff: false,
            stuff_status: function (_stuff) {
                var ret = 'shop-o';
                if (_stuff.saling == 0) {
                    ret = 'closed-eye';
                }
                return ret;
            },
        };
    },
    computed: {
        enter_company: function () {
            var ret = false;
            if (this.$store.state.userinfo.company && this.$store.state.userinfo.buyer == false) {
                ret = true;
            }
            return ret;
        },

    },
    methods: {
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
        readd_stuff(_stuff) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'readd_type', [_stuff, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            });
        },
        refresh_page: function () {
            this.$router.go(0);
        },
        edit_stuff: function () {
            var vue_this = this;
            var stuff = Object.create(vue_this.stuff_in_edit);
            stuff.price = parseInt(stuff.price);
            vue_this.$call_remote_process("company_management", 'edit_type', [stuff, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            }).finally(function () {
                vue_this.show_edit_stuff = false;
            });
        },
        show_out_edit_diag: function (_stuff) {
            this.stuff_in_edit = _stuff;
            this.show_edit_stuff = true;
        },
        show_out_remove_diag: function (_stuff) {
            var vue_this = this;
            var stuff = Object.create(_stuff);
            Dialog.confirm({
                    title: '确认下架',
                    message: '确定要下架' + _stuff.name + '吗？',
                })
                .then(() => {
                    stuff.price = parseInt(stuff.price);
                    vue_this.$call_remote_process("company_management", 'remove_type', [stuff, vue_this.$cookies.get('pa_ssid')]).then(function () {
                        vue_this.init_company_data();
                    });
                });
        },
        add_stuff: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'add_type', [vue_this.add_stuff_name, parseInt(vue_this.add_stuff_price), vue_this.add_stuff_last, this.$cookies.get("pa_ssid")]).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            }).finally(function () {
                vue_this.show_add_stuff = false;
            });
        },
        get_type_detail: function (_id) {
            var vue_this = this;
            vue_this.all_type = [];
            this.$call_remote_process("stuff_info", 'get_stuff_detail', [_id]).then(function (resp) {
                vue_this.$set(vue_this.all_type, vue_this.all_type.length, resp);
                vue_this.all_type.sort((a, b) => {
                    return b.saling - a.saling;
                });
            });
        },
        init_company_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'get_all_type', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach(function (element) {
                    vue_this.get_type_detail(element);
                });
            });

        }
    },
    beforeMount: function () {
        this.init_company_data();
        this.config_with_wx();
    },
}
</script>

<style scoped>
.scan_button_show {
    position: fixed;
    bottom: 10%;
    left: 10%;
    color: rgb(183, 196, 64);
    background-color: rgb(133, 228, 220);
}
</style>
