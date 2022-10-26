<template>
<div class="driver_register_show">
    <div v-if="is_login">
        <van-cell-group>
            <van-cell class="driver-title-cell" title="司机基本信息" value="">
                <template #right-icon>
                    <van-button icon="replay" size="mini" type="warning" @click="reset_user">重置</van-button>
                </template>
            </van-cell>
            <van-cell icon="phone-o" title="电话号" :value="driver_phone"></van-cell>
            <van-cell icon="idcard" title="身份证号" :value="driver_id"></van-cell>

            <licenseCollapse v-if="need_driver_license" title="证件" icon="passed" :licenseList="driverLicenseList" @open="loadDriverLicense" @submit="onSubmitLicense" @update="doLicenseUpdate" @delete="doLicenseDelete"></licenseCollapse>

        </van-cell-group>

        <van-divider>今日承运信息</van-divider>
        <van-empty v-if="trans_info.length <= 0" description="当前无承运任务，请联系所属单位派车后点击刷新">
        </van-empty>
        <div v-else>
            <div class="single_record_show" v-for="(single_trans, index) in trans_info" :key="index">
                <div v-if="!single_trans.can_enter" style="color:red;">不可进</div>
                <div v-else style="color:green;">可进</div>
                <van-cell title="进厂时间" :value="single_trans.date"></van-cell>
                <van-cell :title="single_trans.main_vichele + '-' + single_trans.behind_vichele" :value="single_trans.stuff_name" :label="single_trans.order_company?single_trans.order_company:'(未指定拉货公司)'" />

                <div v-if="single_trans.need_license">
                    <licenseCollapse title="主车证件" icon="logistics" :belong="single_trans.main_vichele" :show-expire-date-edit="false" :licenseList="vehicleLicense[single_trans.main_vichele]" @open="loadVehicleLicense(single_trans.main_vichele)" @submit="onSubmitVehicleLicense" @delete="doDeleteVehicleLicense"></licenseCollapse>
                    <licenseCollapse title="挂车证件" icon="cart-o" :belong="single_trans.behind_vichele" :show-expire-date-edit="false" :licenseList="vehicleLicense[single_trans.behind_vichele]" @open="loadVehicleLicense(single_trans.behind_vichele)" @submit="onSubmitVehicleLicense" @delete="doDeleteVehicleLicense"></licenseCollapse>
                </div>

                <van-cell v-if="!single_trans.is_buy" :title="single_trans.destination_company" center>
                    <template #right-icon>
                        <div style="margin-left:8px;">
                            <div v-if="single_trans.need_sec_check">
                                <div v-if="!single_trans.sec_check_passed">
                                    <span>安检未通过</span>
                                    <van-button type="info" plain size="mini" @click="enter_sec_check_prepare(single_trans.destination_company, single_trans.main_vichele, single_trans.behind_vichele, driver_phone)">
                                        上传安检信息
                                    </van-button>
                                </div>
                                <div v-else>
                                    <van-button type="warning" plain size="mini" @click="enter_sec_check_prepare(single_trans.destination_company, single_trans.main_vichele, single_trans.behind_vichele, driver_phone)">
                                        查看证件信息
                                    </van-button>
                                    <van-button v-if="should_checkin(single_trans.date) && !single_trans.is_registered && single_trans.destination_company" type="info" size="small" @click="register_vichele(single_trans.destination_company, single_trans.id)">排号
                                    </van-button>
                                </div>
                            </div>
                            <div v-else>
                                <div v-if="single_trans.need_checkin">
                                    <van-button v-if="should_checkin(single_trans.date) && !single_trans.is_registered && single_trans.destination_company" type="info" size="small" @click="register_vichele(single_trans.destination_company, single_trans.id)">排号
                                    </van-button>
                                </div>
                            </div>
                        </div>
                    </template>
                    <div v-if="single_trans.is_registered">
                        <div v-if="single_trans.register_number != '0'">
                            <div>
                                进厂序号：{{ single_trans.register_number }}
                            </div>
                            <div>
                                还需等待：{{ single_trans.register_order }}个
                            </div>
                        </div>
                        <div v-else>
                            已叫号，请进场
                        </div>
                    </div>
                </van-cell>
                <div v-if="single_trans.is_buy">
                    <van-button v-if="!single_trans.order_company" type="info" size="small" @click="act_select_company = true;focus_vichele_index = index">指定拉货公司
                    </van-button>
                    <div v-else>
                        <van-button v-if="single_trans.company_for_select.length > 0 && single_trans.is_buy" type="warning" size="small" @click="act_select_company = true;focus_vichele_index = index">修改拉货公司
                        </van-button>
                        <van-cell title="磅单照片" center :label="single_trans.upload_permit?'允许上传':'当前位置不允许上传'">
                            <template #right-icon>
                                <van-button v-if="single_trans.attach_url" size="small" type="info" @click="pre_view_attach(single_trans.attach_url)">预览
                                </van-button>
                                <van-uploader :after-read="upload_attachment" @click-upload="proc_focus(single_trans.id)" accept="image/*">
                                    <van-button :disabled="!single_trans.upload_permit" icon="plus" size="small" type="primary">上传
                                    </van-button>
                                </van-uploader>
                            </template>
                        </van-cell>
                        <div v-if="single_trans.attach_url">
                            <van-field label="出矿（厂）净重" type="number" v-model="input_enter_weight[index]" placeholder="请输入出厂净重"></van-field>
                            <van-field label="确认出矿（厂）净重" type="number" v-model="input_enter_weight_confirm[index]" placeholder="请再次输入出厂净重"></van-field>
                            <div style="margin:16px;">
                                <van-button type="primary" block size="small" @click="fill_enter_weight(single_trans.id, index)">提交
                                </van-button>
                            </div>
                        </div>

                    </div>
                </div>
                <van-cell v-if="single_trans.destination_address" title="详细地址：" :value="single_trans.destination_address"></van-cell>
                <van-cell v-if="single_trans.is_registered" title="进厂位置：" :value="single_trans.enter_location" :label="'签到时间:' + single_trans.register_timestamp"></van-cell>
            </div>
        </div>
        <van-button round type="info" block @click="refresh_cur_page">刷新</van-button>
        <van-action-sheet v-model="act_select_company" :actions="company_for_select" @select="fill_company" />
    </div>
    <div v-if="need_bind_info">
        <van-form @submit="register_driver">
            <van-field v-model="bind_info.phone" name="手机号" type="tel" label="手机号" placeholder="手机号" :rules="[{ required: true, message: '请填写手机号' }]" />
            <van-field v-model="bind_info.verify_code" maxlength="6" type="digit" name="验证码" label="验证码" placeholder="验证码" :rules="[{ required: true, message: '请填写验证码' }]">
                <template #button>
                    <van-button v-if="current_count_down == 0" size="small" type="primary" native-type="button" @click="send_sms">发送验证码
                    </van-button>
                    <van-count-down v-else format="ss秒后再次发送" :time="current_count_down" @finish="current_count_down = 0" />
                </template>
            </van-field>
            <van-field v-model="bind_info.id" name="身份证号" label="身份证号" placeholder="身份证号" :rules="[{ required: true, message: '请填写身份证号' }]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </div>

    <van-dialog get-container="body" show-cancel-button cancel-button-text="关闭" v-model="sec_check_diag_show" title="安检" closeOnClickOverlay :showConfirmButton="false">
        <sec-check-diag :company="sec_check_company" :mv="sec_check_mv" :bv="sec_check_bv" :driver="sec_check_driver"></sec-check-diag>
    </van-dialog>
</div>
</template>

<script>
import {
    compressAccurately
} from 'image-conversion';
import {
    addDriverLicense,
    getLicenseBySilentId,
    updateLicenseExpireDate,
    delLicense,
    addVehicleLicense,
    delVehicleLicense,
    getVehicleLicenseByPlateNo
} from '@/api/driver';
import {
    ImagePreview
} from 'vant';
import licenseCollapse from '@/components/LicenseCollapse'
import SecCheckDiag from '@/components/SecCheckDiag'

export default {
    name: 'DriverRegister',
    components: {
        licenseCollapse,
        "sec-check-diag": SecCheckDiag,
    },
    data: function () {
        return {
            sec_check_company: '',
            sec_check_mv: '',
            sec_check_bv: '',
            sec_check_driver: '',
            sec_check_diag_show: false,
            driverLicenseList: [],
            vehicleLicense: {},
            is_login: false,
            need_bind_info: false,
            bind_info: {
                phone: '',
                id: '',
                verify_code: '',
            },
            trans_info: [],
            driver_id: '',
            driver_phone: '',
            driver_license: [],
            current_count_down: 0,
            act_select_company: false,
            focus_vichele_index: 0,
            focus_vichele: 0,
            input_enter_weight: [],
            input_enter_weight_confirm: [],
            current_date: new Date(),
            should_checkin: function (_date) {
                var ret = false;
                var cur_date = this.formatDateTime(this.current_date);

                if (_date.split(' ')[0] == cur_date) {
                    ret = true;
                }
                return ret;
            },
        };
    },
    computed: {
        need_driver_license: function () {
            var ret = false;
            this.trans_info.forEach(element => {
                if (element.need_license) {
                    ret = true;
                }
            });
            return ret;
        },
        company_for_select: function () {
            let ret = [];
            if (this.trans_info.length > 0) {
                this.trans_info[this.focus_vichele_index].company_for_select.forEach(element => {
                    ret.push({
                        name: element
                    });
                });
            }
            return ret;
        },
        silent_id: function () {
            return this.$cookies.get('driver_silent_id');
        }
    },
    methods: {
        enter_sec_check_prepare: function (_company, _mv, _bv, _driver) {
            this.sec_check_company = _company;
            this.sec_check_mv = _mv;
            this.sec_check_bv = _bv;
            this.sec_check_driver = _driver;
            this.sec_check_diag_show = true;
        },
        formatDateTime: function (date = new Date()) {
            let y = date.getFullYear();
            let m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            let d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            return y + '-' + m + '-' + d;
        },
        async loadDriverLicense() {
            let silent_id = this.silent_id;
            this.driverLicenseList = await getLicenseBySilentId(silent_id);
        },
        async onSubmitLicense(formData, callback) {
            try {
                let silent_id = this.silent_id;
                let file = formData.licenseFile[0];
                await addDriverLicense(file.file, silent_id, formData.expireDate);
                await this.loadDriverLicense();
                callback();
            } catch (err) {
                console.log(err);
            }
        },
        async doLicenseUpdate(license) {
            await updateLicenseExpireDate(this.silent_id, '', license);
            await this.loadDriverLicense();
        },
        async doLicenseDelete(license) {
            try {
                await this.$dialog.confirm({
                    title: '确认删除证件记录吗？'
                })
                await delLicense(this.silent_id, license.id);
                await this.loadDriverLicense();
            } catch (err) {
                console.log(err);
            }
        },
        async loadVehicleLicense(plate_no) {
            let list = await getVehicleLicenseByPlateNo(plate_no);
            this.$set(this.vehicleLicense, plate_no, list)
        },
        async onSubmitVehicleLicense(formData, callback) {
            try {
                let silent_id = this.silent_id;
                let file = formData.licenseFile[0];
                await addVehicleLicense(file.file, silent_id, formData.expireDate, formData.belong);
                await this.loadVehicleLicense(formData.belong);
                callback();
            } catch (err) {
                console.log(err);
            }
        },
        async doDeleteVehicleLicense(license) {
            await delVehicleLicense(this.silent_id, license.id)
            await this.loadVehicleLicense(license.belong)
        },
        refresh_cur_page: function () {
            this.$router.go(0);
        },
        proc_focus: function (_id) {
            console.log(_id);
            this.focus_vichele = _id;
        },
        fill_enter_weight: function (_id, index) {
            var vue_this = this;
            if (vue_this.input_enter_weight[index].length <= 0 || vue_this.input_enter_weight_confirm[index].length <= 0) {
                vue_this.$notify("出厂重量填写错误");
                return;
            }
            if (vue_this.input_enter_weight[index] != vue_this.input_enter_weight_confirm[index]) {
                vue_this.$notify("出厂重量填写错误");
                return;
            }
            vue_this.$call_remote_process("vichele_management", "fill_enter_weight", [vue_this.$cookies.get('driver_silent_id'), _id, parseFloat(vue_this.input_enter_weight[index])]).then(function (resp) {
                if (resp) {
                    vue_this.$dialog.confirm({
                        title: '提交成功'
                    }).finally(function () {
                        vue_this.$router.go(0);
                    });
                }
            });
        },
        convert_2_base64_send: function (_file) {
            var vue_this = this;
            var reader = new FileReader();
            reader.readAsDataURL(_file);
            reader.onloadend = function () {
                var result = this.result;
                var file_content = result.split(';base64,')[1];
                vue_this.$call_remote_process("vichele_management", "fill_weight_attach", [vue_this.$cookies.get('driver_silent_id'), vue_this.focus_vichele, file_content]).then(function (resp) {
                    if (resp) {
                        vue_this.$router.go(0);
                    }
                });
            };
        },
        upload_attachment: function (_file) {
            var vue_this = this;
            compressAccurately(_file.file, 400).then(function (res) {
                vue_this.convert_2_base64_send(res, false);
            });
        },
        pre_view_attach: function (_remote_path) {
            ImagePreview({
                images: [this.$remote_url + _remote_path],
                closeable: true
            });
        },
        tmd_upper: function (_value) {
            return _value.toUpperCase();
        },
        save_tmd: function (_trans) {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "fill_tmd", [vue_this.$cookies.get('driver_silent_id'), _trans.id, _trans.tmd_no]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },
        fill_company: function (item) {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "fill_company_name", [vue_this.$cookies.get('driver_silent_id'), vue_this.trans_info[vue_this.focus_vichele_index].id, item.name]).then(function (resp) {
                if (resp) {
                    vue_this.act_select_company = false;
                    vue_this.$router.go(0);
                }
            });
        },
        reset_user: function () {
            var vue_this = this;
            this.$dialog.confirm({
                title: '重置确认',
                message: '只有电话或身份证等信息输入错误时才需要重置，确认重置吗？',
            }).then(() => {
                vue_this.$call_remote_process("stuff_plan_management", "driver_silent_unregister", [vue_this.silent_id]).then(function () {
                    vue_this.$router.go(0);
                });
            });
        },
        send_sms: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "driver_silent_send_sms", [vue_this.bind_info.phone]).then(function (resp) {
                if (resp) {
                    vue_this.$toast("验证码已发送");
                    vue_this.current_count_down = 60000;
                }
            });
        },
        login: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "driver_silent_login", [vue_this.$route.query.code]).then(function (resp) {
                if (resp.length <= 0) {
                    window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web%2fdriver_register&response_type=code&scope=snsapi_base&state=bind#wechat_redirect"
                } else {
                    vue_this.$cookies.set('driver_silent_id', resp, '5d');
                    window.location.replace("/pa_web/driver_register");
                }
            });
        },
        register_driver: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'driver_silent_register', [vue_this.$route.query.code, vue_this.bind_info.id, vue_this.bind_info.phone, vue_this.bind_info.verify_code]).then(function (resp) {
                vue_this.$cookies.set('driver_silent_id', resp, '5d');
                window.location.replace("/pa_web/driver_register");
            });
        },
        get_trans_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'get_today_driver_info', [vue_this.$cookies.get('driver_silent_id')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.trans_info, index, element);
                    if (element.can_enter) {
                        vue_this.$set(vue_this.input_enter_weight, index, element.count);
                    } else {
                        vue_this.$set(vue_this.input_enter_weight, index, "");
                    }
                    vue_this.$set(vue_this.input_enter_weight_confirm, index, "");
                });
            });
        },
        getDistance(lat1, lng1, lat2, lng2) {
            var dis = 0;
            var radLat1 = toRadians(lat1);
            var radLat2 = toRadians(lat2);
            var deltaLat = radLat1 - radLat2;
            var deltaLng = toRadians(lng1) - toRadians(lng2);
            dis = 2 * Math.asin(Math.sqrt(Math.pow(Math.sin(deltaLat / 2), 2) + Math.cos(radLat1) * Math.cos(radLat2) * Math.pow(Math.sin(deltaLng / 2), 2)));
            return dis * 6378137;

            function toRadians(d) {
                return d * Math.PI / 180;
            }
        },
        getLocation(options = {}) {
            const resultPromise = new Promise((resolve, reject) => {
                if (navigator.geolocation && navigator.geolocation.getCurrentPosition) {
                    navigator.geolocation.getCurrentPosition((res) => {
                        resolve(res)
                    }, (err) => {
                        reject(err)
                    }, options)
                } else {
                    reject({
                        code: 1,
                        message: "Browser doesn't support"
                    })
                }
            })
            return resultPromise
        },

        unregister_vichele: function (_id) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'unregister_vichele', [vue_this.$cookies.get('driver_silent_id'), _id]).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            });
        },

        register_vichele: async function (_dest, _id) {
            var vue_this = this;
            var position_err = true;

            try {
                var current_position = await vue_this.getLocation();
                position_err = false;
                var config_position = await vue_this.$call_remote_process("company_management", "get_company_position_config", [_dest]);
                var real_distance = vue_this.getDistance(current_position.coords.latitude, current_position.coords.longitude, config_position.lat, config_position.lag);
                if (config_position.distance > real_distance) {

                    var resp = await vue_this.$call_remote_process("stuff_plan_management", 'register_vichele', [vue_this.$cookies.get('driver_silent_id'), _id]);
                    if (resp) {
                        vue_this.$toast("签到成功");
                        vue_this.$router.go(0);
                    }
                } else {
                    vue_this.$toast("当前距离厂区" + (real_distance / 1000).toFixed(1) + "公里, 请在" + config_position.distance / 1000 + "公里范围内签到");
                }
            } catch (error) {
                if (position_err) {
                    vue_this.$toast("获取定位失败。");

                }
            }

        },
        get_driver_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "get_driver_info", [vue_this.$cookies.get('driver_silent_id')]).then(function (resp) {
                vue_this.driver_id = resp.id;
                vue_this.driver_phone = resp.phone;
            });
        }
    },
    beforeMount: function () {
        var vue_this = this;
        if (vue_this.$route.query.state == 'bind') {
            vue_this.need_bind_info = true;
        } else if (vue_this.$route.query.code) {
            vue_this.login();
        } else {
            vue_this.$call_remote_process("stuff_plan_management", "verify_driver_silent_login", [vue_this.$cookies.get('driver_silent_id')]).then(function (resp) {
                if (resp) {
                    vue_this.is_login = true;
                    vue_this.get_trans_info();
                    vue_this.get_driver_info();
                } else {
                    window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web%2fdriver_register&response_type=code&scope=snsapi_base&state=STATE#wechat_redirect"
                }
            });
        }
    }
}
</script>

<style scoped>
.driver-title-cell {
    color: #969799
}

.single_record_show {
    margin-bottom: 5px;
    margin-right: 10px;
    margin-left: 10px;
    background-color: rgb(238, 238, 238);
    border: 1px solid black;
}
</style>
