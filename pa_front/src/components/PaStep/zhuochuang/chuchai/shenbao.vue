<template>
<div class="pa_step_shenbao_show">
    <van-field v-model="step_detail.title" :readonly="!editable" name="title" label="标题" placeholder="请简要描述问题" :rules="[{ required: true, message: '请填写标题' }]" />
</div>
</template>

<script>
import Vue from 'vue';
import {
    Form
} from 'vant';
import {
    Field,
    Button
} from 'vant';

Vue.use(Field);
Vue.use(Form);
Vue.use(Button);
export default {
    name: 'shenbao',
    data: function () {
        return {
            step_detail: {
                title: '',
            },
        };
    },
    props: {
        editable: Boolean,
        step_content: String,
    },
    methods: {
        make_json: function () {
            return JSON.stringify(this.step_detail);
        },
    },
    beforeMount: function () {
        if (this.step_content) {
            var inject_obj = JSON.parse(this.step_content);
            this.step_detail = inject_obj;
        }
    }
}
</script>

<style>

</style>
