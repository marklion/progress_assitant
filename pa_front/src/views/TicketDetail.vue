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
            <component :is="current_step_component[index]" :ref="single_step.id" :step_content="step_comments(index)" :editable="editable"></component>
            <div style="margin: 16px;" v-if="editable && step_index == index">
                <van-button round type="info" editable @click="submit(1, $refs[single_step.id][0])">提交下一步</van-button>
                <van-button round type="danger" editable @click="submit(-1,$refs[single_step.id][0])">返回上一步</van-button>
            </div>
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
        };
    },
    methods: {
        submit: function (_dir, _component) {
            var vue_this = this;
            vue_this.$axios.post('/ticket/' + vue_this.ticket_detail_info.ticket_number, {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                comments: _component.make_json(),
                step_id: vue_this.ticket_detail_info.next_step,
                direction:_dir,
            }).then(function (resp) {
                if (resp.data.result == true) {
                    vue_this.$router.replace({
                        name: 'Home',
                    });
                }
            }).catch(function (err) {
                console.log(err);
            });
        }
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
