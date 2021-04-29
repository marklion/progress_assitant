<template>
<div class="self_info_show">
    <van-form @submit="on_submit">
        <van-field v-model="userinfo.name" name="姓名" label="姓名" placeholder="推荐填入真实姓名" :rules="[{ required: true, message: '请填写姓名' }]" />
        <van-field name="logo" label="头像">
            <template #input>
                <van-uploader v-model="logo" :after-read="upload_to_wx" :deletable="false">
                    <van-button type="primary">更换头像</van-button>
                </van-uploader>
            </template>
        </van-field>
        <van-field name="phone" v-model="userinfo.phone" label="手机号" placeholder="请填入手机号" :rules="[{ required: true, message: '请填写正确手机号', pattern: /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/ }]">
            <template #button v-if="userinfo.phone != $store.state.userinfo.phone || has_verify_code">
                <van-button v-if="current_count_down>0" disabled size="small" type="primary" native-type="button" @click="send_sms">
                    <van-row type="flex" justify="center" align="center" :gutter="5">
                        <van-col>
                            <van-count-down format="ss" :time="current_count_down" @finish="current_count_down = 0" />
                        </van-col>
                        <van-col>
                            秒后再次发送
                        </van-col>
                    </van-row>
                </van-button>
                <van-button v-else size="small" type="primary" native-type="button" @click="send_sms">发送验证码</van-button>
            </template>
        </van-field>
        <van-field v-if="has_verify_code" v-model="verify_code" name="验证码" label="验证码" placeholder="请输入验证码" :rules="[{ required: true, message: '请填写验证码' }]" />
        <van-field name="company_picker" v-if="$store.state.userinfo.buyer" v-model="userinfo.company" label="公司" placeholder="请填入所在公司" :rules="[{ required: true, message: '请填写所属公司' }]" />
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">提交</van-button>
        </div>
    </van-form>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Form,
    Field
} from 'vant';
import {
    Popup
} from 'vant';
import {
    Picker
} from 'vant';
import {
    Uploader
} from 'vant';
import {
    Button
} from 'vant';
import {
    RadioGroup,
    Radio
} from 'vant';
import {
    CountDown
} from 'vant';
import {
    Col,
    Row
} from 'vant';

import {
    compressAccurately
} from 'image-conversion';
Vue.use(Col);
Vue.use(Row);
Vue.use(CountDown);
Vue.use(Radio);
Vue.use(RadioGroup);
Vue.use(Button);
Vue.use(Uploader);
Vue.use(Picker);
Vue.use(Popup);
Vue.use(Field);
Vue.use(Form);
export default {
    name: 'SelfInfo',
    data: function () {
        return {
            logo: [{
                url: ''
            }],
            files: {
                name: '',
                type: '',
            },
            userinfo: {
                name: '',
                company: '',
                logo: '',
                buyer: false,
                phone: '',
            },
            admin: '',
            verify_code: '',
            has_verify_code: false,
            current_count_down: 0,
        }
    },
    beforeMount: function () {
        this.userinfo = JSON.parse(JSON.stringify(this.$store.state.userinfo));
        this.logo[0].url = this.$remote_url + this.userinfo.logo;
    },
    watch: {
        "$store.state.userinfo": function (_new_value) {
            this.userinfo = JSON.parse(JSON.stringify(_new_value));
            this.logo[0].url = this.$remote_url + this.userinfo.logo;
        }
    },
    methods: {
        send_sms: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", "send_sms_verify", [vue_this.$cookies.get('pa_ssid'), vue_this.userinfo.phone]).then(function (resp) {
                if (resp) {
                    vue_this.has_verify_code = true;
                    vue_this.current_count_down = 60000;
                }
            });
        },
        on_submit: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", 'update_user_info', [vue_this.userinfo, vue_this.$cookies.get('pa_ssid'), vue_this.verify_code]).then(function (resp) {
                if (resp == true) {
                    var ssid = vue_this.$cookies.get('pa_ssid');
                    vue_this.$call_remote_process('user_management', 'get_user_info', [ssid]).then(function (resp) {
                        if (resp.user_id != 0) {
                            vue_this.$store.commit('set_userinfo', resp);
                            vue_this.$store.commit('set_login', true);
                        } else {
                            vue_this.$store.commit('set_userinfo', {
                                buyer: true,
                            });
                            vue_this.$store.commit('set_login', false);
                        }
                        if (vue_this.$route.query.from == 'auto') {
                            window.location.replace("/pa_web/");
                        } else {
                            vue_this.$router.go(-1);
                        }
                    });
                } else {
                    console.log(resp);
                }
            });
        },

        // 组件方法 获取 流
        upload_to_wx(file) {
            this.logo.splice(0, 1);
            var vue_this = this;
            compressAccurately(file.file, 100).then(function (res) {
                var reader = new FileReader();
                reader.readAsDataURL(res);
                reader.onloadend = function () {
                    vue_this.postImg(this.result);
                };
            });
        },
        postImg(base64) {
            var file_content = base64.split(';base64,')[1];
            this.$call_remote_process("user_management", 'update_logo', [file_content, this.$cookies.get('pa_ssid')]).
            then(function (resp) {
                console.log(resp);
            });
        }
    },
}
</script>

<style>

</style>
