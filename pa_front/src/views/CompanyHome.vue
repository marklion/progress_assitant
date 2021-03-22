<template>
<div class="company_home_show">
    <van-row type="flex" justify="center" align="center">
        <h1>今日报价</h1>
        <van-field v-for="(single_type, index) in all_type" :key="index" type="number" v-model="single_type.price" :label="single_type.name" readonly>
            <template #button>
                <van-button size="small" type="primary" @click="show_out_edit_diag(single_type)">编辑</van-button>
                <van-button size="small" type="danger" @click="show_out_remove_diag(single_type)">删除</van-button>
            </template>
        </van-field>
        <van-button type="primary" round icon="plus" @click="show_add_stuff = true">增加商品</van-button>
    </van-row>
    <van-dialog :show-confirm-button="false" :show-cancel-button="false" v-model="show_add_stuff" title="添加商品">
        <van-form @submit="add_stuff">
            <van-field v-model="add_stuff_name" name="商品名" label="商品名" placeholder="请输入商品名" :rules="[{ required: true, message: '请填写商品名' }]" />
            <van-field v-model="add_stuff_price" type="number" name="价格" label="价格" placeholder="请输入价格" :rules="[{ required: true, message: '请填写价格' }]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">确认</van-button>
            </div>
        </van-form>
    </van-dialog>
    <van-dialog :show-confirm-button="false" :show-cancel-button="false" v-model="show_edit_stuff" title="修改商品">
        <van-form @submit="edit_stuff">
            <van-field v-model="stuff_in_edit.name" name="商品名" label="商品名" readonly />
            <van-field v-model="stuff_in_edit.price" name="价格" label="价格" placeholder="请输入价格" :rules="[{ required: true, message: '请填写价格' }]" />
            <van-field v-model="stuff_in_edit.comment" name="价格" label="存量" placeholder="请输入存量信息" :rules="[{ required: true, message: '请填写存量' }]" />
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

Vue.use(Dialog);
Vue.use(Form);
Vue.use(Popup);
Vue.use(Button);
Vue.use(Field);
Vue.use(Col);
Vue.use(Row);
export default {
    name: 'CompanyHome',
    data: function () {
        return {
            all_type: [],
            show_add_stuff: false,
            add_stuff_name: '',
            add_stuff_price: '',
            stuff_in_edit: {
                name: '',
                price: 0,
                comment: '',
            },
            show_edit_stuff: false,
            show_remove_stuff: false,
        };
    },
    methods: {
        edit_stuff: function () {
            var vue_this = this;
            var stuff = Object.create(vue_this.stuff_in_edit);
            stuff.price = parseInt(stuff.price);
            vue_this.$get_client("company_management").edit_type(stuff, vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            }).catch(function (err) {
                console.log(err);
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
                    title: '确认删除',
                    message: '确定要删除' + _stuff.name + '吗？',
                })
                .then(() => {
                    stuff.price = parseInt(stuff.price);
                    vue_this.$get_client("company_management").remove_type(stuff, vue_this.$cookies.get('pa_ssid')).then(function () {
                        vue_this.init_company_data();
                    }).catch(function (err) {
                        console.log(err);
                    });
                });
        },
        add_stuff: function () {
            var vue_this = this;
            vue_this.$get_client("company_management").add_type(vue_this.add_stuff_name, parseInt(vue_this.add_stuff_price), this.$cookies.get("pa_ssid")).then(function (resp) {
                if (resp) {
                    vue_this.init_company_data();
                }
            }).catch(function (err) {
                console.log(err);
            }).finally(function () {
                vue_this.show_add_stuff = false;
            });
        },
        get_type_detail: function (_id) {
            var vue_this = this;
            vue_this.all_type = [];
            this.$get_client("stuff_info").get_stuff_detail(_id).then(function (resp) {
                vue_this.$set(vue_this.all_type, vue_this.all_type.length, resp);
            }).catch(function (err) {
                console.log(err);
            });
        },
        init_company_data: function () {
            var vue_this = this;
            vue_this.$get_client("company_management").get_all_type(vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                resp.forEach(function (element) {
                    vue_this.get_type_detail(element);
                });
            }).catch(function (err) {
                console.log(err);
            });

        }
    },
    beforeMount: function () {
        this.init_company_data();
    },
}
</script>

<style>

</style>
