<template>
<div class="stuff_plan_show">
    <van-notice-bar v-if="company_notice" left-icon="volume-o" :text="company_notice" />
    <van-cell :value="stuff_brief.name" title="计划货品" :label="stuff_brief.company"></van-cell>
    <stuff-info-submit :proxy_company="$route.query.proxy_company" :type_id="stuff_brief.type_id" :is_create="true" :min_time="min_time" :orig_name="stuff_brief.name" :orig_price="stuff_brief.price"></stuff-info-submit>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    NoticeBar
} from 'vant';

Vue.use(NoticeBar);
Vue.use(Cell);
Vue.use(CellGroup);
import StuffInfoSubmit from '../components/StuffInfoSubmit.vue'
export default {
    name: 'StuffPlan',
    components: {
        "stuff-info-submit": StuffInfoSubmit,
    },
    data: function () {
        return {
            stuff_brief: {
                name: '',
                company: '',
                price: 0.0,
                type_id: 0,
            },
            min_time: {},
            company_notice: '',
        };
    },
    beforeMount: function () {
        var vue_this = this;
        this.$call_remote_process("stuff_info", 'get_stuff_detail', [parseInt(vue_this.$route.params.type_id)]).then(function (resp) {
            vue_this.stuff_brief.name = resp.name;
            vue_this.stuff_brief.company = resp.company;
            vue_this.stuff_brief.price = resp.price;
            vue_this.stuff_brief.type_id = resp.type_id;
            vue_this.$call_remote_process("company_management", 'get_notice', [resp.company]).then(function (resp) {
                vue_this.company_notice = resp;
            });
        });
        this.min_time = new Date();
    },
    methods: {

    },
}
</script>

<style>

</style>
