<template>
<div class="pa_step_zhuguan_show">
    <van-radio-group v-model="step_detail.manager_approve">
        <van-radio :name="true">批准</van-radio>
        <van-radio :name="false">驳回</van-radio>
    </van-radio-group>
</div>
</template>

<script>
import Vue from 'vue';
import {
    RadioGroup,
    Radio
} from 'vant';

Vue.use(Radio);
Vue.use(RadioGroup);
export default {
    name: 'zhuguan',
    data: function () {
        return {
            step_detail: {
                manager_approve: {},
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
