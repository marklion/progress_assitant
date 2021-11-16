<template>
<div class="plan_copy_show">
    <van-notice-bar v-if="company_notice" left-icon="volume-o" :text="company_notice" />
    <van-dialog v-model="show_notice_diag" v-if="company_notice" title="公告" :message="company_notice" @confirm="show_copy_info">
    </van-dialog>
    <van-cell-group title="当前价格">
        <van-cell :title="name" :value="price" :label="sale_company" />
    </van-cell-group>
    <stuff-info-submit :proxy_company="proxy_company" :type_id="type_id" :is_create="true" :min_time="new Date()" :orig_name="name" :orig_vichele_info="vichele_info" :orig_price="price"></stuff-info-submit>
</div>
</template>

<script>
import Vue from 'vue';
import StuffInfoSubmit from '../components/StuffInfoSubmit.vue'
import {
    Cell,
    CellGroup
} from 'vant';
import {
    NoticeBar
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Notify
} from 'vant';
// 全局注册
Vue.use(Dialog);
Vue.use(NoticeBar);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'PlanCopy',
    components: {
        "stuff-info-submit": StuffInfoSubmit,
    },
    data: function () {
        return {
            name: '',
            price: 0,
            plan_time: '',
            vichele_info: [],
            proxy_company: '',
            type_id: 0,
            sale_company: '',
            company_notice: '',
            show_notice_diag: true,
        };
    },
    methods: {
        show_copy_info: function () {
            Notify({
                type: 'warning',
                message: "复制成功，请修改进厂时间，车辆信息等相关信息"
            });
        }
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$call_remote_process("stuff_plan_management", 'plan_created_by_user', [vue_this.$cookies.get('pa_ssid'), parseInt(vue_this.$route.params.plan_id)]).then(function (resp) {
            if (!resp) {
                vue_this.$toast("无法复制");
                vue_this.$router.back();
            }
        });

        vue_this.$call_remote_process("stuff_plan_management", 'get_plan', [parseInt(vue_this.$route.params.plan_id)]).then(function (resp) {
            vue_this.plan_time = resp.plan_time;

            resp.vichele_info.forEach(element => {
                if (!element.finish || !vue_this.$route.query.delay) {
                    vue_this.vichele_info.push(element);
                }
            });
            vue_this.name = resp.name;
            vue_this.proxy_company = resp.proxy_company;
            vue_this.type_id = resp.type_id;
            vue_this.sale_company = resp.sale_company;
            vue_this.$call_remote_process("stuff_info", "get_stuff_detail", [vue_this.type_id, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.price = resp.price;
                vue_this.$call_remote_process("company_management", 'get_notice', [resp.company]).then(function (resp) {
                    vue_this.company_notice = resp;
                });
            });
        });
        if (vue_this.$route.query.delay) {
            vue_this.$route.meta.private_title = "计划延期";
        }
    },
}
</script>

<style>

</style>
