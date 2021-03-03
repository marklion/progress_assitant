<template>
<div class="pa_step_1_show">
    <van-form @submit="onSubmit">
        <van-field v-model="step_detail.title" name="title" label="标题" placeholder="请简要描述问题" :rules="[{ required: true, message: '请填写标题' }]" />
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">创建</van-button>
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
    Field,
    Button
} from 'vant';

Vue.use(Field);
Vue.use(Form);
Vue.use(Button);
export default {
    name: 'PaStep1',
    data: function () {
        return {
            step_detail: {
                title: '',
            },
        };
    },
    methods: {
        onSubmit: function () {
            var vue_this = this;
            vue_this.$axios.post('/ticket', {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                comments: JSON.stringify(vue_this.step_detail),
                step_id:1,
            }).then(function (resp) {
                if (resp.data.result.length > 0) {
                    console.log(resp);
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
