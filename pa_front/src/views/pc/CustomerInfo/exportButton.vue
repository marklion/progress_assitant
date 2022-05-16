<template>
<div>
    <el-button class="op-btn op-export" plain @click="onClick" type="primary" size="medium" :loading="loading" :disabled="disabled" icon="el-icon-download">
        {{title}}
    </el-button>
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
            disabled: false,
            exportUrl: ''
        }
    },
    computed: {
        id() {
            return 'save-file' + this.index
        }
    },

    methods: {
        async onClick() {
            this.loading = true;
            try {
                console.log(this.token, this.rowData.customerName)
                let downloadPath = await this.$call_remote_process_no_toast('open_api_management', 'export_balance_audit_log', [this.token, this.rowData.customerName])

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
@media screen and (max-width: 420px){
    .op-btn{
        font-size: 12px;
        padding: 6px;
    }
    div {
        display: block;
        margin-left: 0;
    }
}

</style>
