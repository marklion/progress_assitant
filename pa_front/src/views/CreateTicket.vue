<template>
<div class="create_ticket_show">
    <div class="app_name_show">
        {{app_info.app_name}}
    </div>
    <div class="app_description_show">
        {{app_info.app_description}}
    </div>
    <component :is="first_step_component"></component>
</div>
</template>

<script>
export default {
    name: 'CreateTicket',
    data: function () {
        return {
            first_step_component: {},
            app_info: {
                app_name:'',
                app_description:'',
            },
        };
    },
    methods: {
        get_steps: function() {
        var vue_this = this;
        vue_this.$axios.get('/steps/' + this.$route.params.app_id).then(function (resp) {
            if (resp.data.result[0]) {
                vue_this.first_step_component = () => import('../components/PaStep' + resp.data.result[0].step_id + '.vue');
            }
        }).catch(function (err) {
            console.log(err);
        });
        },
        get_app_info: function() {
            this.app_info.app_name = this.$route.query.app_name;
            this.app_info.app_description = this.$route.query.app_description;
        },
    },
    beforeMount: function () {
        this.get_steps();
        this.get_app_info();
    },
}
</script>

<style>

</style>
