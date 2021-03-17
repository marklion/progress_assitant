<template>
<div class="company_home_show">
    <van-row type="flex" justify="center" align="center">
        <h1>今日报价</h1>
        <van-field v-for="(single_type, index) in all_type" :key="index" v-model="single_type.price" :label="single_type.name" placeholder="请输入新价格" />
    </van-row>
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

Vue.use(Field);
Vue.use(Col);
Vue.use(Row);
export default {
    name: 'CompanyHome',
    data: function () {
        return {
            all_type: [],
        };
    },
    methods: {
        get_type_detail: function (_id) {
            var vue_this = this;
            vue_this.all_type = [];
            this.$get_client("stuff_info").get_stuff_detail(_id).then(function (resp) {
                vue_this.$set(vue_this.all_type, vue_this.all_type.length, resp);
            }).catch(function (err) {
                console.log(err);
            });
        },
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$get_client("company_management").get_all_type(vue_this.$cookies.get('pa_ssid')).then(function (resp) {
            resp.forEach(function (element) {
                vue_this.get_type_detail(element);
            });
        }).catch(function (err) {
            console.log(err);
        });
    },
}
</script>

<style>

</style>
