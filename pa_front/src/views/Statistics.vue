<template>
<div class="statistics_show">
    <van-cell-group title="快捷导出">
        <van-cell title="计划进厂时间" center>
            <van-radio-group v-model="plan_date" direction="horizontal">
                <van-radio name="1">今日</van-radio>
                <van-radio name="2">明日</van-radio>
            </van-radio-group>
        </van-cell>
        <van-cell title="提报时间" center>
            <van-radio-group v-model="create_date" direction="horizontal">
                <van-radio name="1">不限</van-radio>
                <van-radio name="2">昨日</van-radio>
                <van-radio name="3">今日</van-radio>
            </van-radio-group>
        </van-cell>
        <div style="margin: 16px;">
            <van-button type="info" block round size="small" @click="export_plan_by_date">导出</van-button>
        </div>
    </van-cell-group>
    <van-cell-group title="按计划创建时间导出">
        <van-cell title="选择日期区间" :value="date" @click="show_date_plan = true" center>
        </van-cell>
        <van-calendar v-model="show_date_plan" get-container="body" position="right" allow-same-day type="range" @confirm="onConfirm" :min-date="minDate" :max-date="maxDate" />
    </van-cell-group>
    <van-cell-group title="按装车时间导出">
        <van-cell title="选择日期区间" :value="date" @click="show_date_deliver = true" center>
        </van-cell>
        <van-calendar v-model="show_date_deliver" get-container="body" position="right" type="range" allow-same-day @confirm="export_plan_deliver" :min-date="minDate" :max-date="maxDate" />
    </van-cell-group>

    <export-file :remote_file="download_url" v-model="show_export_file"></export-file>
    <van-cell-group title="按计划日期导出">
        <van-field readonly clickable name="calendar" :value="plan_search_date" label="计划日期" placeholder="点击选择日期" @click="show_plan_search_date = true" />
        <van-calendar v-model="show_plan_search_date" @confirm="confirm_search_date" :min-date="new Date(new Date().setDate(new Date().getDate() - 61))" />
    </van-cell-group>
    <van-cell-group title="导出执行率">
        <van-cell title="选择日期区间" :value="date" @click="show_date = true" center>
        </van-cell>
        <van-calendar v-model="show_date" get-container="body" position="right" type="range" allow-same-day @confirm="export_exe_rate" :min-date="minDate" :max-date="maxDate" />
    </van-cell-group>
    <van-cell-group title="导出磅单">
        <van-cell title="选择日期区间" :value="date" @click="show_ticket_export= true" center>
        </van-cell>
        <van-calendar v-model="show_ticket_export" allow-same-day get-container="body" position="right" type="range" @confirm="export_ticket" :min-date="minDate" :max-date="maxDate" />
    </van-cell-group>

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
import {
    Divider
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    RadioGroup,
    Radio
} from 'vant';

Vue.use(Radio);
Vue.use(RadioGroup);
Vue.use(Col);
Vue.use(Row);
Vue.use(Divider);
Vue.use(Button);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Calendar);
import ExportFile from '../components/ExportFile.vue'
import VueClipboard from 'vue-clipboard2'
Vue.use(VueClipboard)
export default {
    name: 'Statistics',
    data: function () {
        return {
            plan_search_date: '',
            show_plan_search_date: false,
            begin_date: 0,
            end_date: 0,
            date: '',
            show_date: false,
            show_ticket_export: false,
            show_date_plan: false,
            show_date_deliver: false,
            minDate: new Date(),
            maxDate: new Date(),
            download_url: '',
            show_export_file: false,
            plan_date: '1',
            create_date: '1',
        };
    },
    components: {
        "export-file": ExportFile,
    },
    methods: {
        export_ticket: function (_date) {
            console.log(_date);
            var begin_date = this.formatDateTime(_date[0]);
            var end_date = this.formatDateTime(_date[1]);
            this.show_ticket_export = false;

            this.$copyText(this.$remote_url + '/pc/ticket_export?date_begin=' + begin_date + '&date_end=' + end_date + '&ssid=' + this.$cookies.get('pa_ssid')).then(() => {
                this.$dialog.alert({
                    message: '链接已复制，请粘贴到PC浏览器打开'
                });
            });

        },
        export_plan_deliver: function (_date) {
            const [start, end] = _date;
            this.show_date_deliver = false;
            this.date = `${start.getFullYear()}/${start.getMonth() + 1}/${start.getDate()}-${end.getFullYear()}/${end.getMonth() + 1}/${end.getDate()}`
            end.setDate(end.getDate() + 1);
            var begin_date = this.formatDateTime(start);
            var end_date = this.formatDateTime(end);
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "export_plan_by_deliver_date_range", [vue_this.$cookies.get("pa_ssid"), begin_date, end_date]).then(function (resp) {
                if (resp) {
                    vue_this.download_url = vue_this.$remote_url + resp;
                    vue_this.show_export_file = true;
                } else {
                    vue_this.$toast("无信息");
                }
            });
        },
        confirm_search_date: function (_date) {
            var vue_this = this;
            vue_this.show_plan_search_date = false;
            vue_this.$call_remote_process("stuff_plan_management", "export_plan_by_deliver_date", [vue_this.$cookies.get('pa_ssid'), vue_this.formatDateTime(_date)]).then(function (resp) {
                if (resp) {
                    vue_this.download_url = vue_this.$remote_url + resp;
                    vue_this.show_export_file = true;
                } else {
                    vue_this.$toast("无交易信息");
                }
            });
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            return y + '-' + m + '-' + d;
        },
        export_plan_by_date: function () {
            var vue_this = this;
            var current_time = new Date();
            var create_time = new Date();
            if (this.plan_date == '2') {
                current_time.setDate(current_time.getDate() + 1);
            }
            if (this.create_date == '2') {
                create_time.setDate(create_time.getDate() - 1);
            }
            var create_date_string = "";
            if (this.create_date != '1') {
                create_date_string = this.formatDateTime(create_time);
            }
            var plan_date_string = this.formatDateTime(current_time);
            console.log(plan_date_string);
            vue_this.$call_remote_process("stuff_plan_management", 'export_plan_by_plan_date', [vue_this.$cookies.get('pa_ssid'), plan_date_string, create_date_string]).then(function (resp) {
                vue_this.download_url = vue_this.$remote_url + resp;
                vue_this.show_export_file = true;
            });
        },
        generate: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'export_plan_by_create_date', [vue_this.$cookies.get('pa_ssid'), vue_this.begin_date, vue_this.end_date]).then(function (resp) {
                if (resp) {
                    vue_this.download_url = vue_this.$remote_url + resp;
                    vue_this.show_export_file = true;
                } else {
                    vue_this.$toast("无交易信息");
                }
            });

        },
        export_exe_rate: function (_date) {
            const [start, end] = _date;
            this.show_date = false;
            this.date = `${start.getFullYear()}/${start.getMonth() + 1}/${start.getDate()}-${end.getFullYear()}/${end.getMonth() + 1}/${end.getDate()}`
            var begin_date = this.formatDateTime(start);
            var end_date = this.formatDateTime(end);
            console.log(begin_date);
            console.log(end_date);
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "export_exe_rate", [vue_this.$cookies.get("pa_ssid"), begin_date, end_date]).then(function (resp) {
                if (resp) {
                    vue_this.download_url = vue_this.$remote_url + resp;
                    vue_this.show_export_file = true;
                } else {
                    vue_this.$toast("无信息");
                }
            });

        },
        onConfirm: function (_date) {
            const [start, end] = _date;
            this.show_date_plan = false;
            this.begin_date = start.valueOf() / 1000;
            this.end_date = end.valueOf() / 1000 + 60 * 60 * 24;
            this.date = `${start.getFullYear()}/${start.getMonth() + 1}/${start.getDate()}-${end.getFullYear()}/${end.getMonth() + 1}/${end.getDate()}`
            this.generate();
        },
    },
    beforeMount: function () {
        this.minDate.setFullYear(this.minDate.getFullYear() - 1);
    }
}
</script>

<style>

</style>
