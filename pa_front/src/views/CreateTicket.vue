<template>
<div class="create_ticket_show">
    <div class="app_name_show">
        {{app_info.app_name}}
    </div>
    <div class="app_description_show">
        {{app_info.app_description}}
    </div>
    <van-form @submit="onSubmit">
        <component :is="first_step_component" ref="step_content" :editable="true"></component>
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit" editable>提交</van-button>
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
    Button
} from 'vant';

Vue.use(Form);
Vue.use(Button);
export default {
    name: 'CreateTicket',
    data: function () {
        return {
            first_step_component: {},
            app_info: {
                app_name: '',
                app_description: '',
            },
            current_step:0,
        };
    },
    methods: {
        get_steps: function () {
            var vue_this = this;
            vue_this.$axios.get('/steps/' + this.$route.params.app_id).then(function (resp) {
                if (resp.data.result[0]) {
                    vue_this.current_step = resp.data.result[0].step_id;
                    vue_this.first_step_component = () => import('../components/' + resp.data.result[0].step_component);
                }
            }).catch(function (err) {
                console.log(err);
            });
        },
        get_app_info: function () {
            this.app_info.app_name = this.$route.query.app_name;
            this.app_info.app_description = this.$route.query.app_description;
        },
        onSubmit: function () {
            var vue_this = this;
            vue_this.$axios.post('/ticket', {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                comments: vue_this.$refs.step_content.make_json(),
                step_id: vue_this.current_step,
            }).then(function (resp) {
                if (resp.data.result.length > 0) {
                    vue_this.$router.push({name:'TicketDetail', params:{ticket_number:resp.data.result}});
                }
            }).catch(function (err) {
                console.log(err);
            });
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
