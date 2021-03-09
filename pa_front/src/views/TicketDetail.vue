<template>
<div class="ticket_detail_show">
    <div class="ticket_number_show">
        {{ticket_detail_info.ticket_number}}
    </div>
    <div class="app_name_show">
        {{ticket_detail_info.app_name}}
    </div>
    <div class="ticket_timestamp_show">

        {{ticket_detail_info.ticket_timestamp}}
    </div>
    <van-steps direction="vertical" :active="step_index">
        <van-step v-for="(single_step, index) in all_step" :key="index">{{single_step.name}}
            <step-detail 
                :editable="step_editable(single_step.id)" 
                :first_step="index == 0" 
                :step_comment="single_step.comment" 
                :step_component="single_step.component" 
                :step_description="single_step.description" 
                :step_id="single_step.id" 
                :step_name="single_step.name"
                :step_operator="single_step.operator_user" :step_result="single_step.result" :step_timestamp="single_step.timestamp"
                :ticket_number="ticket_detail_info.ticket_number">
            </step-detail>
        </van-step>
    </van-steps>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Step,
    Steps
} from 'vant';
import {
    Button
} from 'vant';

Vue.use(Button);
Vue.use(Step);
Vue.use(Steps);

import StepDetail from '../components/StepDetail.vue'

export default {
    name: 'TicketDetail',
    data: function () {
        return {
            step_index: 0,
            ticket_detail_info: {
                ticket_number: '',
                app_name: '',
                app_description: '',
                ticket_timestamp: '',
                next_step: 0,
            },
            all_step: [],
            current_step_component: [],
            step_comments: function (_index) {
                return this.all_step[_index].comment;
            },
            editable: false,
            step_editable: function (_step_id) {
                var ret = false;
                if (this.editable && _step_id == this.ticket_detail_info.next_step)
                {
                    ret = true;
                }
                return ret;
            },
        };
    },
    components: {
        "step-detail":StepDetail,
    },
    methods: {
        
    },
    beforeMount: function () {
        var vue_this = this;
        vue_this.$axios.get('/ticket_detail/' + vue_this.$route.params.ticket_number).then(function (resp) {
            vue_this.ticket_detail_info.ticket_number = resp.data.result.ticket_number;
            vue_this.ticket_detail_info.app_name = resp.data.result.app_name;
            vue_this.ticket_detail_info.app_description = resp.data.result.app_description;
            vue_this.ticket_detail_info.ticket_timestamp = resp.data.result.ticket_timestamp;
            vue_this.ticket_detail_info.next_step = resp.data.result.next_step;
            resp.data.result.all_steps.forEach((element, index) => {
                vue_this.$set(vue_this.all_step, index, element);
                vue_this.$set(vue_this.current_step_component, index, () => import('../components/' + element.component));
                if (element.id == vue_this.ticket_detail_info.next_step) {
                    vue_this.step_index = index;
                }
            });

        }).catch(function (err) {
            console.log(err);
        });

        vue_this.$axios.get('/ticket_editable/' + vue_this.$route.params.ticket_number + '/' + vue_this.$cookies.get('pa_ssid')).then(function (resp) {
            if (true == resp.data.result) {
                vue_this.editable = true;
            }
        }).catch(function (err) {
            console.log(err);
        });
    },
}
</script>

<style>

</style>
