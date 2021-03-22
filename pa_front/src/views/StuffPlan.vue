<template>
<div class="stuff_plan_show">
    <div class="stuff_name_show">
        {{stuff_brief.name}}
    </div>
    <div class="stuff_company_show">
        {{stuff_brief.company}}
    </div>
    <stuff-info-submit :type_id="stuff_brief.type_id" :is_create="true" :min_time="min_time" :orig_name="stuff_brief.name" :orig_price="stuff_brief.price"></stuff-info-submit>
</div>
</template>

<script>
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
                type_id:0,
            },
            min_time: {},
        };
    },
    beforeMount: function () {
        var vue_this = this;
        this.$get_client("stuff_info").get_stuff_detail(parseInt(vue_this.$route.params.type_id)).then(function (resp) {
            vue_this.stuff_brief.name = resp.name;
            vue_this.stuff_brief.company = resp.company;
            vue_this.stuff_brief.price = resp.price;
            vue_this.stuff_brief.type_id= resp.type_id;
        }).catch(function (err) {
            console.log(err);
        });
        this.min_time = new Date();
    },
    methods: {

    },
}
</script>

<style>

</style>
