<template>
<div class="step_detail show">
    <div v-if="editable">{{step_description}}</div>
    <div v-if="step_timestamp">最近提交时间：{{step_timestamp}}</div>
    <div v-if="step_operator">最近提交人：{{step_operator}}</div>
    <div v-if="!first_step && step_result != 0">
        <div v-if="step_result == 1">审批结果:同意</div>
        <div v-else>审批结果:驳回</div>
    </div>
    <component :is="step_component_obj" ref="step_content" :step_content="step_comment" :editable="editable"></component>
    <div v-if="editable">
        <van-button v-if="first_step && !step_timestamp" round type="info" editable @click="create_ticket">创建</van-button>
        <div v-else>
            <van-button v-if="!first_step" round type="danger" editable @click="submit(-1,$refs.step_content)">退回</van-button>
            <van-button round type="info" editable @click="submit(1, $refs.step_content)">提交</van-button>
        </div>
    </div>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Button
} from 'vant';

Vue.use(Button);
export default {
    name: 'StepDetail',
    data: function () {
        return {};
    },
    computed: {
        step_component_obj: function () {
            var vue_this = this;
            console.log(this.step_component);
            var myComponent = resolve => require.ensure([], () => resolve(require('./' + vue_this.step_component)));
            return myComponent;
        },
    },
    props: {
        editable: Boolean,
        step_name: String,
        step_description: String,
        step_timestamp: String,
        step_operator: String,
        step_comment: String,
        step_component: String,
        first_step: Boolean,
        step_id: Number,
        step_result: Number,
        ticket_number:String,
    },
    methods: {
        submit: function (_dir, _component) {
            var vue_this = this;
            vue_this.$axios.post('/ticket/' + vue_this.ticket_number, {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                comments: _component.make_json(),
                step_id: vue_this.step_id,
                direction: _dir,
            }).then(function (resp) {
                if (resp.data.result == true) {
                    vue_this.$router.back(-1);
                }
            }).catch(function (err) {
                console.log(err);
            });
        },
        create_ticket: function () {
            var vue_this = this;
            vue_this.$axios.post('/ticket', {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                comments: vue_this.$refs.step_content.make_json(),
                step_id: vue_this.step_id,
            }).then(function (resp) {
                if (resp.data.result.length > 0) {
                    vue_this.$router.push({
                        name: 'TicketDetail',
                        params: {
                            ticket_number: resp.data.result
                        }
                    });
                }
            }).catch(function (err) {
                console.log(err);
            });
        },
    },
}
</script>

<style>

</style>
