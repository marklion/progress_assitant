<template>
<div class="stuff_plan_show">
    <div class="stuff_name_show">
        {{stuff_brief.name}}
    </div>
    <div class="stuff_company_show">
        {{stuff_brief.company}}
    </div>
    <van-form @submit="submit_plan">
        <van-field name="stepper" label="计划量(吨)">
            <template #input>
                <van-stepper decimal-length="1" v-model="plan_count" />
            </template>
        </van-field>
        <van-field v-model="vichele_info" name="vichele_info" label="运输车辆" :rules="[{ required: true, message: '请添加运输车辆信息' }]" />
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">提交</van-button>
        </div>
    </van-form>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Stepper
} from 'vant';
import { Button } from 'vant';

Vue.use(Button);
Vue.use(Stepper);
Vue.use(Field);
Vue.use(Form);
export default {
    name: 'StuffPlan',
    data: function () {
        return {
            stuff_brief: {
                name: '',
                company: '',
            },
            plan_count:10.0,
            vichele_info:'',
        };
    },
    beforeMount: function () {
        var vue_this = this;
        this.$get_client("stuff_info").get_stuff_detail(parseInt(vue_this.$route.params.type_id)).then(function (resp) {
            vue_this.stuff_brief.name = resp.name;
            vue_this.stuff_brief.company = resp.company;
        }).catch(function (err) {
            console.log(err);
        });
    },
    methods: {
        submit_plan: function () {
            var vue_this = this;
            this.$get_client("stuff_plan_management").create_plan({
                count:this.plan_count,
                type_id: parseInt(this.$route.params.type_id),
                }, this.$cookies.get("pa_ssid")).then(function (resp) {
                    vue_this.$router.push({name:'PlanDetail', params:{plan_id:resp}});
                }).catch(function (err) {
                    console.log(err);
                });
        } 
    },
}
</script>

<style>

</style>
