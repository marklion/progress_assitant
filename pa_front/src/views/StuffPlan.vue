<template>
<div class="stuff_plan_show">
    <div class="stuff_name_show">
        {{stuff_brief.name}}
    </div>
    <div class="stuff_company_show">
        {{stuff_brief.company}}
    </div>
    <stuff-info-submit :is_create="true" :min_time="min_time"></stuff-info-submit>
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
            },
            min_time: {},
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
        this.min_time = new Date();
    },
    methods: {

    },
}
</script>

<style>

</style>
