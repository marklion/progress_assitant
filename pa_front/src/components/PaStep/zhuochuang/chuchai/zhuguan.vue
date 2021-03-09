<template>
<div class="pa_step_zhuguan_show">
    <van-field v-model="step_detail.comment" :readonly="!editable" name="title" label="意见" placeholder="请填入详细意见" :rules="[{ required: true, message: '请填写意见' }]" />
</div>
</template>

<script>
import Vue from 'vue';
import {
    Field,
} from 'vant';

Vue.use(Field);
export default {
    name: 'zhuguan',
    data: function () {
        return {
            step_detail: {
                comment:''
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
