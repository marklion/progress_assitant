<template>
<div>
    <el-popover placement="bottom" title="选择日期范围" trigger="click">
        <el-button class="op-btn op-export" slot="reference" plain size="medium" type="primary">
            {{title}}
        </el-button>
        <el-date-picker v-model="date_range" type="daterange" align="right" unlink-panels range-separator="至" start-placeholder="开始日期" end-placeholder="结束日期" :picker-options="pickerOptions">
        </el-date-picker>
        <div>
            <el-button type="primary" size="small" @click="onClick">导出</el-button>
        </div>
    </el-popover>
    <a :id="id" :href="exportUrl" download hidden="hidden" ref="downloadLink">保存文件</a>
</div>
</template>

<script>
import Vue from 'vue';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
Vue.use(ElementUI);

export default {
    name: 'exportButton',

    props: {
        title: {
            type: String,
            default: 'export'
        },
        rowData: {
            type: Object,
            default: () => {
                return {}
            }
        },
        index: {
            type: Number,
            default: 0
        },
        token: {
            type: String
        }
    },

    data: function () {
        return {
            loading: false,
            date_range: [],
            disabled: false,
            exportUrl: '',
            pickerOptions: {
                shortcuts: [{
                    text: '最近一周',
                    onClick(picker) {
                        const end = new Date();
                        const start = new Date();
                        start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
                        picker.$emit('pick', [start, end]);
                    }
                }, {
                    text: '最近一个月',
                    onClick(picker) {
                        const end = new Date();
                        const start = new Date();
                        start.setTime(start.getTime() - 3600 * 1000 * 24 * 30);
                        picker.$emit('pick', [start, end]);
                    }
                }, {
                    text: '最近三个月',
                    onClick(picker) {
                        const end = new Date();
                        const start = new Date();
                        start.setTime(start.getTime() - 3600 * 1000 * 24 * 90);
                        picker.$emit('pick', [start, end]);
                    }
                }]
            },
        }
    },
    computed: {
        id() {
            return 'save-file' + this.index
        }
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
        async onClick() {
            this.loading = true;
            try {
                console.log(this.token, this.rowData.customerName)
                var begin_date = this.formatDateTime(this.date_range[0]);
                var end_date = this.formatDateTime(this.date_range[1]);
                let downloadPath = await this.$call_remote_process_no_toast('open_api_management', 'export_balance_audit_log', [this.token, this.rowData.customerName, begin_date, end_date])

                this.exportUrl = this.$remote_url + downloadPath

                this.$nextTick(() => {
                    this.$refs.downloadLink.click();
                    this.loading = false;
                })
            } catch (err) {
                console.log(err)
                this.$message({
                    type: 'error',
                    message: '获取下载地址失败'
                })
                this.loading = false;
            }

        }
    }

}
</script>

<style scoped>
div {
    display: inline-block;
    margin-left: 5px;
}

@media screen and (max-width: 420px) {
    .op-btn {
        font-size: 12px;
        padding: 6px;
    }

    div {
        display: block;
        margin-left: 0;
    }
}
</style>
