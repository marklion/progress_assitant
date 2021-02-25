<template>
<div class="bind_company_show">
    <van-form @submit="on_submit">
        <van-field v-model="username" name="姓名" label="姓名" placeholder="推荐填入真实姓名" :rules="[{ required: true, message: '请填写姓名' }]" />
        <van-field name="logo" label="头像">
            <template #input>
                <van-uploader v-model="logo">
                    <van-button type="primary">更换头像</van-button>
                </van-uploader>
            </template>
        </van-field>
        <van-field readonly clickable name="company_picker" :value="company_picker" label="公司" placeholder="点击选择公司" @click="show_company_picker = true" :rules="[{ required: true, message: '请选择公司' }]" />
        <van-popup v-model="show_company_picker" position="bottom">
            <van-picker show-toolbar :columns="company_from_server" @confirm="on_company_confirm" @cancel="show_company_picker = false" />
        </van-popup>
        <van-field readonly clickable name="role_picker" :value="role_picker" label="角色" placeholder="点击选择角色" @click="show_role_picker = true" :rules="[{ required: true, message: '请选择角色' }]" />
        <van-popup v-model="show_role_picker" position="bottom">
            <van-picker show-toolbar :columns="role_from_server" @confirm="on_role_confirm" @cancel="show_role_picker = false" />
        </van-popup>
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

Vue.use(Button);
Vue.use(Uploader);
Vue.use(Picker);
Vue.use(Popup);
Vue.use(Field);
Vue.use(Form);
export default {
    name: 'BindCompany',
    data: function () {
        return {
            company_picker: '',
            role_picker: '',
            name_input: '',
            logo_img: '',
            show_company_picker: false,
            show_role_picker: false,
            company_from_server: [],
            role_from_server: [],
            username: '',
            logo: [{
                url: ''
            }],
        }
    },
    methods: {
        on_company_confirm: function (value) {
            this.company_picker = value;
            this.get_role_against_company(value);
            this.show_company_picker = false;
        },
        on_role_confirm: function (value) {
            this.role_picker = value;
            this.show_role_picker = false;
        },
        get_all_company: function () {
            var vue_this = this;
            vue_this.$axios.get('/all_companies').then(function (resp) {
                resp.data.result.forEach((element, index) => {
                    vue_this.$set(vue_this.company_from_server, index, element);
                });
            }).catch(function (err) {
                console.log(err);
            });

        },
        get_role_against_company: function (_company) {
            var vue_this = this;
            vue_this.role_from_server = [];
            vue_this.$axios.get('/all_roles/' + _company).then(function (resp) {
                resp.data.result.forEach((element, index) => {
                    vue_this.$set(vue_this.role_from_server, index, element);
                });
            }).catch(function (err) {
                console.log(err);
            });
        },
        on_submit: function () {
            var vue_this = this;
            vue_this.$axios.post('/userinfo', {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                name: vue_this.username,
                logo: vue_this.logo[0].url.split(vue_this.$remote_url)[1],
                company: vue_this.company_picker,
                role: vue_this.role_picker,
            }).then(function (resp) {
                if (resp.data.result == true) {
                    vue_this.$store.commit('set_userinfo', {
                        name: vue_this.username,
                        logo: vue_this.logo[0].url.split(vue_this.$remote_url)[1],
                        company: vue_this.company_picker,
                        role: vue_this.role_picker,
                    });
                    vue_this.$router.replace({
                        name: 'Home'
                    });
                } else {
                    console.log(resp);
                }
            }).catch(function (err) {
                console.log(err);
            });
        },
    },
    watch: {
        "$store.state.userinfo": function (value) {
            this.username = value.name;
            this.logo[0].url = value.logo;
            this.role_picker = value.role;
            if ('' == this.company_picker) {
                this.company_picker = value.company;
            }
        }
    },
    beforeMount: function () {
        this.username = this.$store.state.userinfo.name;
        this.logo[0].url = this.$store.state.userinfo.logo;
        if ('' == this.company_picker) {
            this.company_picker = this.$store.state.userinfo.company;
        }
        this.role_picker = this.$store.state.userinfo.role;
        this.get_all_company();
        var vue_this = this;
        vue_this.$axios.get('/company/' + this.$route.query.company).then(function (resp) {
            if (resp.data.result != '') {
                vue_this.company_picker = resp.data.result;
                vue_this.get_role_against_company(vue_this.company_picker);
            }
        }).catch(function (err) {
            console.log(err);
        });
    },
}
</script>

<style>

</style>
