<template>
<div class="plan_detail show">
    <stuff-info-submit :orig_comment="comment" :orig_name="name" :orig_price="price" :is_create="false"  :orig_plan_time="plan_time" :plan_id="parseInt($route.params.plan_id)" :orig_vichele_info="vichele_info" :min_time="new Date(created_time)"></stuff-info-submit>
</div>
</template>

<script>
import StuffInfoSubmit from '../components/StuffInfoSubmit.vue'
export default {
    name: 'PlanUpdate',
    components: {
        "stuff-info-submit": StuffInfoSubmit,
    },
    data: function () {
        return {
            name: '',
            company: '',
            price: 0.0,
            plan_time: '',
            vichele_info: [],
            created_time: 10,
            comment:'',
        };
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$call_remote_process("stuff_plan_management",'get_plan', [parseInt(vue_this.$route.params.plan_id)]).then(function (resp) {
            vue_this.plan_time = resp.plan_time;
            vue_this.created_time = resp.created_time * 1000;
            resp.vichele_info.forEach((element, index) => {
                vue_this.$set(vue_this.vichele_info, index, element);
            });
            vue_this.name = resp.name;
            vue_this.company = resp.sale_company;
            vue_this.price = resp.price;
        });
        vue_this.$call_remote_process("stuff_plan_management","get_status_rule", [parseInt(vue_this.$route.params.plan_id)]).then(function (resp) {
            vue_this.comment = resp[0].comment;
        });
    },
}
</script>

<style>

</style>
