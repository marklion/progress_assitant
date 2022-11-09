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
                    <el-popover placement="bottom" title="选择日期范围" trigger="click">
                        <el-button slot="reference" type="text">历史数据导出</el-button>
                        <el-date-picker v-model="date_range" type="daterange" align="right" unlink-panels range-separator="至" start-placeholder="开始日期" end-placeholder="结束日期" :picker-options="pickerOptions">
                        </el-date-picker>
                        <div>
                            <el-button type="primary" size="small" @click="export_sec_check">导出</el-button>
                        </div>
                    </el-popover>
                </span>
            </div>
            <el-input v-model="search_key" placeholder="输入车号过滤"></el-input>
            <el-table highlight-current-row @current-change="handleCurrentChange" :data="all_vehicle_show" style="width: 100%" :row-class-name="tableRowClassName">>
                <el-table-column prop="vichele.main_vichele" label="主车号">
                </el-table-column>
                <el-table-column prop="vichele.behind_vichele" label="挂车号">
                </el-table-column>
                <el-table-column label="司机">
                    <template slot-scope="scope">
                        {{scope.row.vichele.driver_name}}-{{scope.row.vichele.driver_phone}}
                    </template>
                </el-table-column>
            </el-table>
        </el-col>
        <el-col :span="12">
            <el-descriptions title="车辆信息" border :column="3">
                <el-descriptions-item label="司机姓名">{{focus_vehicle.driver_name}}</el-descriptions-item>
                <el-descriptions-item label="手机号">{{focus_vehicle.driver_phone}}</el-descriptions-item>
                <el-descriptions-item label="主车">{{focus_vehicle.mv_number}}</el-descriptions-item>
                <el-descriptions-item label="挂车">{{focus_vehicle.bv_number}}</el-descriptions-item>
                <el-descriptions-item label="皮重">{{focus_vehicle.p_weight.toFixed(2)}}</el-descriptions-item>
                <el-descriptions-item label="毛重">{{focus_vehicle.m_weight.toFixed(2)}}</el-descriptions-item>
                <el-descriptions-item label="净重">{{(focus_vehicle.m_weight - focus_vehicle.p_weight).toFixed(2)}}</el-descriptions-item>
                <el-descriptions-item label="最大充装量">{{max_load.toFixed(2)}}</el-descriptions-item>
                <el-descriptions-item label="是否超量">
                    <div v-if="max_load < (focus_vehicle.m_weight - focus_vehicle.p_weight)" style="color:red;">是</div>
                    <div v-else>否</div>
                </el-descriptions-item>
            </el-descriptions>
            <div style="text-align: center;">
                <el-divider>证件列表</el-divider>
                <el-table :data="sec_check_item" stripe style="width: 100%">
                    <el-table-column prop="name" label="证件名称">
                        <template slot-scope="scope">
                            <span>{{scope.row.name}}</span>
                            <span>
                                <el-tag v-if="scope.row.content_id == undefined || scope.row.content_id <= 0" type="danger">
                                    未上传
                                </el-tag>
                                <el-tag v-else-if="scope.row.has_confirmed" type="success">
                                    已审核
                                </el-tag>
                                <el-tag v-else type="warning">
                                    待审核
                                </el-tag>
                            </span>
                            <div style="color:red;" v-if="scope.row.comment">驳回原因：{{scope.row.comment}}</div>
                        </template>
                    </el-table-column>
                    <el-table-column prop="input_content" label="内容">
                    </el-table-column>
                    <el-table-column label="附件">
                        <template slot-scope="scope">
                            <el-image v-if="scope.row.attachment_path" style="width: 60px; height: 40px;" :src="$remote_url + scope.row.attachment_path" :preview-src-list="[$remote_url + scope.row.attachment_path]">
                            </el-image>
                        </template>
                    </el-table-column>
                    <el-table-column label="有效期">
                        <template slot-scope="scope">
                            <div v-if="scope.row.expired_date == '5000-01-01'">长期有效</div>
                            <div v-else>{{scope.row.expired_date}}</div>
                        </template>
                    </el-table-column>
                    <el-table-column label="操作">
                        <template slot-scope="scope">
                            <div v-if="!scope.row.has_confirmed">
                                <el-button type="primary" size="small" @click="confirm_lcd(scope.row, true)">审核</el-button>
                                <el-button type="text" size="small" @click="comment_lcd(scope.row)">驳回附言</el-button>
                            </div>
                            <el-button v-else type="danger" size="small" @click="confirm_lcd(scope.row, false)">反审</el-button>
                        </template>
                    </el-table-column>
                </el-table>
            </div>
        </el-col>
    </el-row>
</div>
</template>

<script>
import Vue from 'vue';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
// import {
//     VueGrid,
//     VueCell
// } from 'vue-grd';
import XLSX from 'xlsx';
Vue.use(ElementUI);
import PinyinMatch from "pinyin-match";
export default {
    name: "SecCheck",
    // components: {
    //     VueGrid,
    //     VueCell
    // },
    data: function () {
        return {
            search_key: '',
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
            user_info: {
                name: '',
                logo: '',
                is_login: false,
            },
            sec_check_open: false,
            today_vehicle: [],
            focus_vehicle: {
                mv_number: "",
                bv_number: "",
                driver_name: "",
                driver_phone: "",
                p_weight: 0,
                m_weight: 0,
            },
            sec_check_item: [],
            company_name: '',
            saved_cur_row: {},
            date_range: '',
        };
    },
    watch: {
        company_name: function () {
            this.init_sec_check_require();
        },
    },
    computed: {
        max_load: function () {
            var std_load_item = this.sec_check_item.find(item => {
                return item.name == '槽车核载量'
            });
            var std_count_item = this.sec_check_item.find(item => {
                return item.name == '槽车容积'
            });
            var std_load = 0;
            var std_count = 0;
            if (std_load_item && std_load_item.input_content) {
                std_load = parseFloat(std_load_item.input_content);
            }
            if (std_count_item && std_count_item.input_content) {
                std_count = parseFloat(std_count_item.input_content);
            }
            return this.$calc_max_load(std_load, std_count, this.focus_vehicle.p_weight);
        },
        all_vehicle_show: function () {
            var ret = this.today_vehicle;

            if (this.search_key) {
                var tmp_ret = ret;
                ret = [];
                tmp_ret.forEach(element => {
                    if (PinyinMatch.match(element.vichele.behind_vichele, this.search_key) || PinyinMatch.match(element.vichele.main_vichele, this.search_key)) {
                        ret.push(element);
                    }
                });
            }

            ret.sort(function (a) {
                var sort_ret = -1;
                if (a.sec_check_passed) {
                    sort_ret = 1;
                }

                return sort_ret;
            });

            return ret;
        },
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
        export_sec_check: function () {
            var vue_this = this;
            var begin_date = this.formatDateTime(this.date_range[0]);
            var end_date = this.formatDateTime(this.date_range[1]);
            vue_this.$call_remote_process("stuff_plan_management", "export_sec_check_history", [vue_this.$cookies.get("pa_ssid"), begin_date, end_date]).then(function (resp) {
                console.log(resp);
                window.open(vue_this.$remote_url + resp);
            });
        },
        comment_lcd: function (_lcd) {
            var vue_this = this;
            this.$prompt('请输入驳回理由', '提示', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
            }).then(({
                value
            }) => {
                vue_this.$call_remote_process("stuff_plan_management", "confirm_sec_check_data", [vue_this.$cookies.get("pa_ssid"), _lcd.content_id, false, value]).then(function (resp) {
                    if (resp) {
                        vue_this.handleCurrentChange(vue_this.saved_cur_row);
                    }
                });
            });
        },
        confirm_lcd: function (_lcd, is_confirm) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "confirm_sec_check_data", [vue_this.$cookies.get("pa_ssid"), _lcd.content_id, is_confirm]).then(function (resp) {
                if (resp) {
                    vue_this.handleCurrentChange(vue_this.saved_cur_row);
                }
            });
        },
        refresh_page: function () {
            this.$router.go(0);
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
                console.log(vue_this.company_name);
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
            var vue_this = this;
            vue_this.saved_cur_row = {
                ...val
            };
            vue_this.focus_vehicle = {
                mv_number: val.vichele.main_vichele,
                bv_number: val.vichele.behind_vichele,
                driver_name: val.vichele.driver_name,
                driver_phone: val.vichele.driver_phone,
                sec_check_passed: val.vichele.sec_check_passed,
                p_weight: val.vichele.p_weight,
                m_weight: val.vichele.m_weight,
            };
            for (let index = 0; index < vue_this.sec_check_item.length; index++) {
                const element = vue_this.sec_check_item[index];
                var relate_info = "";
                switch (element.use_for) {
                    case 0:
                        relate_info = val.vichele.driver_phone;
                        break;

                    case 1:
                        relate_info = val.vichele.main_vichele;
                        break;
                    case 2:
                        relate_info = val.vichele.behind_vichele;
                        break;
                    default:
                        break;
                }
                var tmp_lic = await vue_this.$call_remote_process("stuff_plan_management", "get_all_sec_check_data", [element.id, relate_info]);
                vue_this.$set(element, "input_content", tmp_lic.input_content);
                vue_this.$set(element, "attachment_path", tmp_lic.attachment_path);
                vue_this.$set(element, "expired_date", tmp_lic.expired_date);
                vue_this.$set(element, "content_id", tmp_lic.id);
                vue_this.$set(element, "has_confirmed", tmp_lic.has_confirmed);
                vue_this.$set(element, "comment", tmp_lic.comment);
            }
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
            });
            this.sec_check_item.forEach(element => {
                fin_json[0][element.name] = element.name;
            });

            for (var i = 0; i < this.today_vehicle.length; i++) {
                var tmp = {
                    mv_number: this.today_vehicle[i].vichele.main_vichele,
                    bv_number: this.today_vehicle[i].vichele.behind_vichele,
                    driver_name: this.today_vehicle[i].vichele.driver_name,
                };
                for (var j = 0; j < this.sec_check_item.length; j++) {
                    var cur_lr = this.sec_check_item[j];
                    var related_info = "";
                    switch (cur_lr.use_for) {
                        case 0:
                            related_info = this.today_vehicle[i].vichele.driver_phone;
                            break;
                        case 1:
                            related_info = this.today_vehicle[i].vichele.main_vichele;
                            break;
                        case 2:
                            related_info = this.today_vehicle[i].vichele.behind_vichele;
                            break;
                        default:
                            break;
                    }
                    var tmp_lcd = await this.$call_remote_process("stuff_plan_management", "get_all_sec_check_data", [cur_lr.id, related_info]);
                    var lcd_content = "无";
                    if (tmp_lcd.id > 0) {
                        lcd_content = tmp_lcd.input_content;
                        if (tmp_lcd.attachment_path) {
                            lcd_content += '<->' + this.$remote_url + tmp_lcd.attachment_path;
                        }
                    }
                    tmp[cur_lr.name] = lcd_content;
                }

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
        },
        init_sec_check_require: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_license_require", [vue_this.company_name]).then(function (resp) {
                vue_this.sec_check_item = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.sec_check_item, index, element);
                });
            });
        },
    },
    beforeMount: function () {
        this.init_user_data();
        this.init_sec_check_require();
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
