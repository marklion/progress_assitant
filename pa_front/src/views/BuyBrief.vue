<template>
<div>
    <van-row type="flex" align="center">
        <van-col :span="20">
            <div id="my_chart" style="width: 83vw;height:30vh;"></div>
        </van-col>
        <van-col :span="4">
            <van-icon size="50px" name="bars" @click="export_diag = true" />
            <div>导出</div>
        </van-col>
    </van-row>
    <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="export_diag" title="数据导出">
        <van-field readonly clickable name="calendar" :value="begin_date?( begin_date + '~' + end_date):''" label="日期范围" placeholder="点击选择日期" @click="showCalendar = true" />
        <van-calendar get-container="body" v-model="showCalendar" type="range" :min-date="new Date(new Date().setDate(new Date().getDate() - 61))" allow-same-day @confirm="onConfirm" />
        <div style="margin: 16px;">
            <van-button round block type="info" @click="export_buy_data">确认</van-button>
        </div>
    </van-dialog>
    <export-file :remote_file="exported_file" v-model="show_finish_diag"></export-file>
</div>
</template>

<script>
import * as echarts from 'echarts';
import ExportFile from '../components/ExportFile.vue'
export default {
    name: 'BuyBrief',
    components: {
        ExportFile
    },
    data: function () {
        return {
            loaded: 0,
            unloaded: 0,
            begin_date: '',
            end_date: '',
            export_diag: false,
            showCalendar: false,
            exported_file: '',
            show_finish_diag: false,
        };
    },
    methods: {
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            return y + '-' + m + '-' + d;
        },
        export_buy_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "export_buy_data", [vue_this.$cookies.get('pa_ssid'), vue_this.begin_date, vue_this.end_date]).then(function (resp) {
                vue_this.exported_file = vue_this.$remote_url + '/logo_res/' + resp;
                vue_this.export_diag = false;
                vue_this.show_finish_diag = true;
            });
        },
        onConfirm: function (date) {
            this.begin_date = this.formatDateTime(date[0]);
            this.end_date = this.formatDateTime(date[1]);
            this.showCalendar = false;
        },
        initChart: function () {
            var myChart = echarts.init(document.getElementById('my_chart'));
            myChart.setOption({
                title: {
                    text: '今日车辆',
                    left: 'center',
                },
                legend: {
                    show: false,
                },
                series: [{
                    type: 'pie',
                    label: {
                        position: 'inside',
                        formatter: '{b}:{c}'
                    },
                    data: [{
                            value: this.loaded,
                            name: '已卸车'
                        },
                        {
                            value: this.unloaded,
                            name: '未卸车'
                        },
                    ],
                }]

            });
        },
    },
    mounted() {
        var vue_this = this;
        vue_this.$call_remote_process("vichele_management", 'get_statistics', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            vue_this.unloaded = resp.today_total - resp.today_finish;
            vue_this.loaded = resp.today_finish;
            vue_this.initChart();
        });
    },
}
</script>

<style>

</style>
