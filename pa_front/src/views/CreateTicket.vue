<template>
<div class="create_ticket_show">
    <div class="app_name_show">
        {{app_info.app_name}}
    </div>
    <div class="app_description_show">
        {{app_info.app_description}}
    </div>
    <step-detail :editable="true" :first_step="true" :step_description="current_step.description" :step_component="current_step.component" :step_id="current_step.id" :step_name="current_step.name"></step-detail>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Form
} from 'vant';
import {
    Button
} from 'vant';

Vue.use(Form);
Vue.use(Button);
import StepDetail from '../components/StepDetail.vue'
export default {
    name: 'CreateTicket',
    data: function () {
        return {
            first_step_component: {},
            app_info: {
                app_name: '',
                app_description: '',
            },
            current_step: {
                id:0,
                name:'',
                description:'',
                component:'',
            },
        };
    },
    components: {
        "step-detail": StepDetail,
    },
    methods: {
        get_steps: function () {
            var vue_this = this;
            vue_this.$axios.get('/steps/' + this.$route.params.app_id).then(function (resp) {
                if (resp.data.result[0]) {
                    var current_step = resp.data.result[0];
                    vue_this.current_step = {
                        id:current_step.step_id,
                        name:current_step.step_name,
                        description:current_step.step_description,
                        component:current_step.step_component,
                    };
                }
            }).catch(function (err) {
                console.log(err);
            });
        },
        get_app_info: function () {
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
