<template>
<div class="order_show">
    <plan-brief v-for="(single_plan, index) in all_plans" :key="index" :plan_id="single_plan" :company_view="false"></plan-brief>
</div>
</template>

<script>
import PlanBrief from '../components/PlanBrief.vue'
export default {
    name: 'Order',
    data: function () {
        return {
            all_plans: [],
        };
    },
    components: {
        "plan-brief": PlanBrief,
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$get_client("stuff_plan_management").get_created_plan(vue_this.$cookies.get('pa_ssid')).then(function (resp) {
            resp.forEach((element, index) => {
                vue_this.$set(vue_this.all_plans, index, element);
            });
        }).catch(function (err) {
            console.log(err);
            vue_this.$toast(err.msg);
        });
    },
}
</script>

<style>

</style>
