<template>
<div class="statistics_show">
    <van-cell title="选择日期区间" :value="date" @click="show_date = true" />
    <van-calendar  v-model="show_date" get-container="body" position="right" type="range" @confirm="onConfirm" :min-date="minDate" :max-date="maxDate" />
    <div v-if="download_url">
        <van-divider>请以下链接复制到浏览器下载</van-divider>
        <van-cell  :value="download_url"></van-cell>
    </div>
    <div style="margin: 16px;">
        <van-button type="info" round block @click="generate">生成统计表</van-button>
    </div>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Calendar
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Button
} from 'vant';
import { Divider } from 'vant';

Vue.use(Divider);
Vue.use(Button);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Calendar);
export default {
    name: 'Statistics',
    data: function () {
        return {
            begin_date: 0,
            end_date: 0,
            date: '',
            show_date: false,
            minDate: new Date(),
            maxDate: new Date(),
            download_url:'',
        };
    },
    methods: {
        generate: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management",'generate_statistics', [vue_this.$cookies.get('pa_ssid'), vue_this.begin_date, vue_this.end_date]).then(function (resp) {
                if (resp) {
                    vue_this.download_url = vue_this.$remote_url + resp;
                } else {
                    vue_this.$toast("无交易信息");
                }
            });

        },
        onConfirm: function (_date) {
            const [start, end] = _date;
            this.show_date = false;
            this.begin_date = start.valueOf() / 1000;
            this.end_date = end.valueOf() / 1000;
            this.date = `${start.getFullYear()}/${start.getMonth() + 1}/${start.getDate()}-${end.getFullYear()}/${end.getMonth() + 1}/${end.getDate()}`
        },
    },
    beforeMount: function () {
        this.minDate.setFullYear(this.minDate.getFullYear() - 1);
    }
}
</script>

<style>

</style>
