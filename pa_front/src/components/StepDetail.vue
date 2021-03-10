<template>
<div class="step_detail show">
    <div v-if="editable">{{step_description}}</div>
    <div v-if="step_timestamp">最近提交时间：{{step_timestamp}}</div>
    <div v-if="step_operator">最近提交人：{{step_operator}}</div>
    <div v-if="!first_step && step_result != 0">
        <div v-if="step_result == 1">审批结果:同意</div>
        <div v-else>审批结果:驳回</div>
    </div>
    <van-form>
        <component :is="step_component_obj" ref="step_content" :step_content="step_comment" :editable="editable"></component>
        <div v-if="editable">
            <van-field v-if="!last_step" readonly clickable label="审批人" :value="assignee_name" placeholder="请选择审批人" @click="assignee_picker = true" :rules="[{ required: true, message: '请选择审批人' }]" />
            <van-popup v-model="assignee_picker" round position="bottom">
                <van-picker show-toolbar :columns="assignees_from_server" @cancel="assignee_picker = false" @confirm="confirm_pick_assignee" />
            </van-popup>
            <van-button v-if="first_step && !step_timestamp" round type="info" editable @click="create_ticket">创建</van-button>
            <div v-else>
                <van-button v-if="!first_step" round type="danger" editable @click="submit(-1,$refs.step_content)">退回</van-button>
                <van-button round type="info" editable @click="submit(1, $refs.step_content)">提交</van-button>
            </div>
        </div>
    </van-form>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Button
} from 'vant';
import {
    Field
} from 'vant';
import {
    Popup
} from 'vant';
import {
    Picker
} from 'vant';
import {
    Form
} from 'vant';

Vue.use(Form);
Vue.use(Picker);
Vue.use(Popup);
Vue.use(Field);
Vue.use(Button);
export default {
    name: 'StepDetail',
    data: function () {
        return {
            assignee_name: '',
            assignee_id: 0,
            assignee_picker: false,
            assignees_from_server: [],
        };
    },
    beforeMount: function () {
        this.get_assignee_from_server();
    },
    computed: {
        step_component_obj: function () {
            var vue_this = this;
            console.log(this.step_component);
            var myComponent = resolve => require.ensure([], () => resolve(require('./' + vue_this.step_component)));
            return myComponent;
        },
    },
    watch: {
        step_id() {
            this.get_assignee_from_server();
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
        ticket_number: String,
        last_step: Boolean,
    },
    methods: {
        get_assignee_from_server: function () {
            var vue_this = this;
            vue_this.assignees_from_server = [];
            vue_this.$axios.get('/users_by_step/' + vue_this.step_id).then(function (resp) {
                resp.data.result.forEach((element, index) => {
                    vue_this.$set(vue_this.assignees_from_server, index, {
                        id: element.id,
                        text: element.name
                    });
                });
            }).catch(function (err) {
                console.log(err);
            });
        },
        confirm_pick_assignee: function (value) {
            this.assignee_name = value.text;
            this.assignee_id = value.id;
            this.assignee_picker = false;
        },
        submit: function (_dir, _component) {
            var vue_this = this;
            var next_assginee_id = vue_this.assignee_id;
            if (_dir < 0) {
                next_assginee_id = 0;
            }
            vue_this.$axios.post('/ticket/' + vue_this.ticket_number, {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                comments: _component.make_json(),
                step_id: vue_this.step_id,
                direction: _dir,
                assignee_id: next_assginee_id,
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
                assignee_id: vue_this.assignee_id,
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
