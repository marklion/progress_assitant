<template>
<div class="container">
    <div class=nav-bar>
        <div class="logo-container">
            <img :src="logoUrl" alt="平台logo" class="sidebar-logo">
            <h1 class="sidebar-title">掌易助理管理平台</h1>
        </div>
        <div class="menu-container">
            <ul>
                <li class="active"><i class="el-icon-s-custom"> 客户管理</i></li>
            </ul>
        </div>
        <span v-if="isLogin" class="logout" @click="doLogout">
            <i class="el-icon-user">注销</i>
        </span>
    </div>
    <div class="main-content">
        <el-card class="login-panel" v-if="!isLogin">
            <div slot="header" class="clearfix">
                <span>登录</span>
            </div>
            <el-alert title="平台未登录，请输入凭证登录" close type="info" center show-icon>
            </el-alert>
            <el-form ref="form" :model="form" label-width="80px" style="margin-top: 20px">
                <el-form-item label="用户凭证">
                    <el-input v-model="form.token"></el-input>
                </el-form-item>
            </el-form>
            <el-button type="primary" @click="onLogin" style="width: 100%">登 录</el-button>
        </el-card>
        <el-card class="info-panel" v-if="isLogin">
            <el-table :data="tableData" stripe style="width: 100%">
                <el-table-column type="index" width="50">
                </el-table-column>
                <el-table-column prop="customerName" label="客户名称">
                </el-table-column>
                <el-table-column prop="balance" label="余额">
                </el-table-column>
                <el-table-column fixed="right" label="操作">
                    <template slot-scope="scope">
                        <el-button plain @click="doEditBalance(scope.row)" type="primary" size="medium" icon="el-icon-edit">设置余额</el-button>
                        <exportButton :index="scope.$index" :rowData="scope.row" title="导出审计" :token="token"></exportButton>
                    </template>
                </el-table-column>
            </el-table>
        </el-card>
    </div>

    <el-dialog title="设置余额" :visible.sync="showBalanceDialog" width="30%" :before-close="handleClose">
        <el-form ref="balanceForm" :rules="rules" :model="balanceForm" label-width="80px">
            <el-form-item label="客户名称" prop="customerName">
                <el-input v-model="balanceForm.customerName" disabled></el-input>
            </el-form-item>
            <el-form-item label="余额" prop="balance">
                <el-input type="number" v-model="balanceForm.balance"></el-input>
            </el-form-item>
            <el-form-item label="备注" prop="reason">
                <el-input type="textarea" v-model="balanceForm.reason"></el-input>
            </el-form-item>
        </el-form>
        <span slot="footer" class="dialog-footer">
            <el-button @click="handleClose">取 消</el-button>
            <el-button type="primary" @click="submitBalanceEdit">确 定</el-button>
        </span>
    </el-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
Vue.use(ElementUI);
import logoUrl from '@/assets/img.png'
import exportButton from './exportButton'

export default {
    components: {
        exportButton
    },
    data() {
        return {
            logoUrl: logoUrl,
            isLogin: true,
            tokenKey: 'pc_admin_token',
            form: {
                token: ''
            },
            tableData: [{
                "customerId": "",
                "customerName": "山西汽运集团天然气利用有限公司",
                "balance": 100000
            }, {
                "customerId": "",
                "customerName": "山西汽运集团天然气利用有限公司",
                "balance": 100000
            }],
            showBalanceDialog: false,
            balanceForm: {
                customerName: '',
                balance: '',
                reason: ''
            },
            rules: {
                balance: [{
                    required: true,
                    message: '请输入余额',
                    trigger: 'blur'
                }],
                reason: [{
                    required: true,
                    message: '请输入备注',
                    trigger: 'blur'
                }]
            }
        }
    },
    computed: {
        token() {
            return this.$cookies.get(this.tokenKey);
        }
    },
    async mounted() {
        if (this.isLogin) {
            this.loadData(this.token)
        }
    },
    methods: {
        async loadData(token) {
            this.tableData = await this.call_remote_process_no_toast('open_api_management', 'get_all_customer_balance', [token])
        },
        async onLogin() {
            try {
                await this.loadData(this.form.token)
                this.$cookies.set(this.tokenKey, this.form.token, -1)
                this.isLogin = true
                this.form = {
                    token: ''
                }
            } catch (err) {
                this.$message({
                    message: '凭证错误，请核对',
                    type: 'error'
                });
            }
        },
        doLogout() {
            this.$cookies.remove(this.tokenKey)
            this.$message({
                message: '已注销',
                type: 'success'
            });
            this.isLogin = false
        },
        doEditBalance(row) {
            this.showBalanceDialog = true
            Object.assign(this.balanceForm, row)
        },
        handleClose() {
            this.balanceForm = {
                customerName: '',
                balance: '',
                reason: ''
            }
            this.showBalanceDialog = false
        },
        async submitBalanceEdit() {
            let valid = await this.$refs.balanceForm.validate()
            if (valid) {
                await this.$call_remote_process_no_toast('open_api_management', 'proc_push_balance', [this.balanceForm, this.token])
                await this.loadData()
            }
        },
        async doExport(row, index) {
            console.log(this.$refs['exportBtn' + index].disabled = true)
            console.log(row)
        }
    }
}
</script>

<style scoped>
html,
body {
    height: 100vh;
    margin: 0;
    padding: 0;
}

.container {
    height: 100vh;
    background: #f7f9fa;
}

.nav-bar {
    height: 50px;
    background-color: #304156;
    box-shadow: 0 2px 6px #7e7979;
}

.nav-bar .logo-container {
    display: inline-block;
    width: 210px;
}

.nav-bar .menu-container {
    display: inline-block;
}

.nav-bar .menu-container li {
    line-height: 50px;
    padding: 0 12px;
}

.nav-bar .menu-container .active {
    color: #fff;
    background: #2b2f3a;
}

.nav-bar .logout {
    float: right;
    color: #FFFFFF;
    line-height: 50px;
    padding: 0 20px;
    cursor: pointer;
}

.nav-bar .logout:hover {
    background: #2b2f3a;
}

.sidebar-logo {
    width: 32px;
    height: 32px;
    padding: 8px;
    background: #FFFFFF;
    vertical-align: middle;
    margin-right: 12px;
    position: relative;
    top: -2px
}

.sidebar-title {
    display: inline-block;
    margin: 0;
    color: #fff;
    font-weight: 600;
    line-height: 50px;
    font-size: 14px;
    font-family: Avenir, Helvetica Neue, Arial, Helvetica, sans-serif;
    vertical-align: middle;
}

.main-content {
    width: 1280px;
    margin: 0 auto;
}

.main-content .login-panel {
    width: 500px;
    margin: 50px auto 0 auto;
}

.main-content .info-panel {
    margin: 20px 0;
}
</style>
