<template>
<div class="sec_check_show">
    <div class="web_header_show">
        <el-row type="flex" justify="space-between" align="middle">
            <el-col :span="20">
                <div class="web_header_content_show">
                    掌易安检
                </div>
            </el-col>
            <el-col :span="4">
                <el-button v-if="!user_info.is_login" @click="get_ssid" type="primary">登录</el-button>
                <div v-else>
                    <div class="user_info_show">
                        <div>当前用户：{{user_info.name}}</div>
                    </div>
                    <el-button @click="logoff_user" type="dangeer">退出登录</el-button>
                </div>
            </el-col>
        </el-row>
    </div>
    <div v-if="!sec_check_open">
        未定制安检模块
    </div>
    <el-row v-else>
        <el-col :span="12">
            <div>
                <span>
                    今日车辆
                </span>
                <span>
                    <el-button type="primary" @click="refresh_page">刷新</el-button>
                    <el-button type="warning" @click="export_record">导出</el-button>
                </span>
            </div>
            <el-table highlight-current-row @current-change="handleCurrentChange" :data="today_vehicle" style="width: 100%" :row-class-name="tableRowClassName">>
                <el-table-column prop="vichele.main_vichele" label="主车号">
                </el-table-column>
                <el-table-column prop="vichele.behind_vichele" label="主车号">
                </el-table-column>
                <el-table-column label="司机">
                    <template slot-scope="scope">
                        {{scope.row.vichele.driver_name}}-{{scope.row.vichele.driver_phone}}
                    </template>
                </el-table-column>
            </el-table>
        </el-col>
        <el-col :span="12">
            <el-descriptions title="车辆信息" border :column="2">
                <el-descriptions-item label="司机姓名">{{focus_vehicle.driver_name}}</el-descriptions-item>
                <el-descriptions-item label="手机号">{{focus_vehicle.driver_phone}}</el-descriptions-item>
                <el-descriptions-item label="主车">{{focus_vehicle.mv_number}}</el-descriptions-item>
                <el-descriptions-item label="挂车">{{focus_vehicle.bv_number}}</el-descriptions-item>
            </el-descriptions>
            <div style="text-align: center;">
                <el-divider>证件列表</el-divider>
                <vue-grid align="stretch" justify="start">
                    <vue-cell v-for="(single_license, index) in focus_vehicle.lic_set" :key="index" width="6of12">
                        <el-image :preview-src-list="getPrivewImages(index)" style="width: 100px; height: 100px" :src="$remote_url + single_license.attachment_path" fit="fill"></el-image>
                        <div>到期时间:{{single_license.expire_date}}</div>
                    </vue-cell>
                </vue-grid>
                <div v-if="focus_vehicle.lic_set.length > 0">
                    <el-divider></el-divider>
                    <el-button type="success" v-if="!focus_vehicle.sec_check_passed" @click="pass_sec">审核通过</el-button>
                    <el-button type="danger" v-else @click="reject_sec">撤销审核</el-button>
                </div>
            </div>
        </el-col>
    </el-row>
</div>
</template>

<script>
import Vue from 'vue';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
import {
    VueGrid,
    VueCell
} from 'vue-grd';
import XLSX from 'xlsx';
Vue.use(ElementUI);
// import PinyinMatch from "pinyin-match";
export default {
    name: "SecCheck",
    components: {
        VueGrid,
        VueCell
    },
    data: function () {
        return {
            user_info: {
                name: '',
                logo: '',
                is_login: false,
                company_name: ''
            },
            sec_check_open: false,
            today_vehicle: [],
            focus_vehicle: {
                mv_number: "",
                bv_number: "",
                driver_name: "",
                driver_phone: "",
                lic_set: []
            }
        };
    },
    methods: {
        refresh_page: function () {
            this.$router.go(0);
        },
        pass_sec: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "sec_check_pass", [vue_this.$cookies.get('pa_ssid'), vue_this.focus_vehicle.id]).then(function (resp) {
                if (resp) {
                    vue_this.refresh_page();
                }
            });
        },
        reject_sec: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "sec_check_reject", [vue_this.$cookies.get('pa_ssid'), vue_this.focus_vehicle.id]).then(function (resp) {
                if (resp) {
                    vue_this.refresh_page();
                }
            });
        },
        getPrivewImages: function (_index) {
            var li_array = [];
            this.focus_vehicle.lic_set.forEach(element => {
                li_array.push(this.$remote_url + element.attachment_path);
            });
            let tempImgList = [...li_array];
            if (_index == 0) return tempImgList;
            let start = tempImgList.splice(_index);
            let remain = tempImgList.splice(0, _index);
            return start.concat(remain);
        },
        init_check_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "get_today_statistics", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.today_vehicle = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.today_vehicle, index, element);
                });
            });
        },
        init_user_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process('user_management', 'get_user_info', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.user_info.name = resp.name;
                vue_this.user_info.logo = resp.logo;
                vue_this.company_name = resp.company;
                vue_this.$call_remote_process("company_management", "get_customize", [vue_this.company_name]).then(function (resp) {
                    if (resp) {
                        vue_this.sec_check_open = resp.need_sec_check;
                    }
                });
                if (resp.user_id != 0) {
                    vue_this.user_info.is_login = true;
                    vue_this.init_check_data();
                }
            });
        },
        logoff_user: function () {
            this.$cookies.set("pa_ssid", "");
            this.$router.go(0);
        },
        get_ssid: function () {
            var vue_this = this;
            this.$prompt('授权码', '提示', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
            }).then(({
                value
            }) => {
                vue_this.$call_remote_process("user_management", "exchange_ssid", [value]).then(function (resp) {
                    vue_this.$cookies.set('pa_ssid', resp, '5d');
                    vue_this.init_user_data();
                });
            });
        },
        async handleCurrentChange(val) {
            console.log(val);
            var vue_this = this;
            vue_this.focus_vehicle = {
                mv_number: val.vichele.main_vichele,
                bv_number: val.vichele.behind_vichele,
                driver_name: val.vichele.driver_name,
                driver_phone: val.vichele.driver_phone,
                sec_check_passed: val.vichele.sec_check_passed,
                lic_set: [],
                id: val.vichele.vichele_id,
            };
            var mv_lic = await vue_this.$call_remote_process("stuff_plan_management", "get_license_by_vehicle_number", [vue_this.focus_vehicle.mv_number]);
            var bv_lic = await vue_this.$call_remote_process("stuff_plan_management", "get_license_by_vehicle_number", [vue_this.focus_vehicle.bv_number]);
            var driver_lic = await vue_this.$call_remote_process("stuff_plan_management", "get_all_license_info_by_driver_phone", [vue_this.$cookies.get('pa_ssid'), vue_this.focus_vehicle.driver_phone]);
            vue_this.focus_vehicle.lic_set = [];
            vue_this.focus_vehicle.lic_set.push(...mv_lic, ...bv_lic, ...driver_lic);
        },
        tableRowClassName({
            row,
        }) {
            if (row.vichele.sec_check_passed) {
                return 'success-row';
            } else {
                return 'warning-row';
            }
        },
        export_record: async function () {
            var fin_json = [];
            fin_json.push({
                mv_number: '主车',
                bv_number: '挂车',
                driver_name: '司机',
                lic_set: "证件"
            });

            for (var i = 0; i < this.today_vehicle.length; i++) {
                var tmp = {
                    mv_number: this.today_vehicle[i].vichele.main_vichele,
                    bv_number: this.today_vehicle[i].vichele.behind_vichele,
                    driver_name: this.today_vehicle[i].vichele.driver_name,
                    lic_set: ''
                };
                var mv_lic = await this.$call_remote_process("stuff_plan_management", "get_license_by_vehicle_number", [tmp.mv_number]);
                var bv_lic = await this.$call_remote_process("stuff_plan_management", "get_license_by_vehicle_number", [tmp.bv_number]);
                var driver_lic = await this.$call_remote_process("stuff_plan_management", "get_all_license_info_by_driver_phone", [this.$cookies.get('pa_ssid'), this.today_vehicle[i].vichele.driver_phone]);
                var all_lic = [];
                all_lic.push(...mv_lic, ...bv_lic, ...driver_lic);
                all_lic.forEach(item => {
                    tmp.lic_set = tmp.lic_set + this.$remote_url + item.attachment_path + ";\r\n";
                });
                fin_json.push(tmp);
            }

            var sheet = XLSX.utils.json_to_sheet(fin_json, {
                skipHeader: true
            });
            var wb = {
                SheetNames: ["s1"],
                Sheets: {
                    s1: sheet
                }
            };
            XLSX.writeFile(wb, "审核记录.xlsx");
        }
    },
    beforeMount: function () {
        this.init_user_data();
    },
}
</script>

<style scoped>
.web_header_show {
    background-color: #000c5e;
    height: 12vh;
}

.web_header_content_show {
    padding-left: 40px;
    color: bisque;
    font-weight: bold;
    font-size: 40px;
    line-height: 80px;
}

.user_info_show {
    color: white;
}
</style>
<style>
.el-table .warning-row {
    background: oldlace;
}

.el-table .success-row {
    background: #f0f9eb;
}
</style>
