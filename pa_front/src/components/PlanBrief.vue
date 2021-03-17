<template>
<div class="plan_brief_show">
    <van-card :desc="company" :title="name" @click="nav_to_detail">
        <template #num>
            {{plan_count}}吨
        </template>
        <template #tags>
            <div>
                进厂时间：{{plan_time}}
            </div>
        </template>
        <template #footer>
            创建时间:{{created_time}}
        </template>
    </van-card>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Card
} from 'vant';

Vue.use(Card);
export default {
    name: 'PlanBrief',
    props: {
        plan_id: Number,
    },
    data: function () {
        return {
            name: '',
            company: '',
            plan_count: 0.1,
            plan_time: '',
            vichele_info: [],
            created_time: ''
        };
    },
    methods: {
        nav_to_detail: function () {
            this.$router.push({name:'PlanDetail', params:{plan_id:this.plan_id}});
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            var h = date.getHours();
            h = h < 10 ? ('0' + h) : h;
            var minute = date.getMinutes();
            minute = minute < 10 ? ('0' + minute) : minute;
            var second = date.getSeconds();
            second = second < 10 ? ('0' + second) : second;
            return y + '-' + m + '-' + d + ' ' + h + ':' + minute + ':' + second;
        },
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$get_client("stuff_plan_management").get_plan(vue_this.plan_id).then(function (resp) {
            vue_this.plan_count = resp.count;
            vue_this.plan_time = resp.plan_time;
            resp.vichele_info.forEach((element, index) => {
                vue_this.$set(vue_this.vichele_info, index, element);
            });
            vue_this.created_time = vue_this.formatDateTime(new Date(resp.created_time * 1000));

            vue_this.$get_client("stuff_info").get_stuff_detail(resp.type_id).then(function (detail_resp) {
                vue_this.name = detail_resp.name;
                vue_this.company = detail_resp.company;
            }).catch(function (err) {
                console.log(err);
            });
        }).catch(function (err) {
            console.log(err);
        });
    },
}
</script>

<style>

</style>
