<template>
<div class="driver_register_show">
    <div v-if="is_login">
        <van-cell-group>
          <van-cell class="driver-title-cell" title="司机基本信息" value="" >
            <template #right-icon>
              <van-button icon="replay" size="mini" type="warning" @click="reset_user">重置</van-button>
            </template>
          </van-cell>
          <van-cell icon="phone-o" title="电话号" :value="driver_phone"></van-cell>
          <van-cell icon="idcard" title="身份证号" :value="driver_id"></van-cell>

          <van-collapse v-model="activeNames">
            <van-collapse-item icon="setting-o" title="其他" name="1">
                <van-cell class="driver-title-cell" title="证件图片" value="" >
                    <template #right-icon>
                        <van-button icon="plus" size="mini" type="primary" @click="add_license_item">新增</van-button>
                       
                    </template>
                  <!-- <van-uploader :after-read="uploadDriverLicense" accept="image/*">
                    <van-button plain icon="plus" size="small" type="primary">上传文件</van-button>
                  </van-uploader> -->
                </van-cell>
                <van-form @submit="onConfirm">
                    <van-field name="uploader" label="证件照片">
    <template #input>
    <van-uploader v-model="fileList" :max-count="1"/>
  </template>
  <template #right-icon><van-icon name="info-o">点击选择图片</van-icon></template>
</van-field>

<van-field
  readonly
  clickable
  name="datetimePicker"
 :value="value"
  label="有效期"
  placeholder="请选择有效期截止日期"
  @click="showPicker = true"
/>
<van-popup v-model="showPicker" position="bottom">
  <van-datetime-picker
    type="date"
    :min-date="current_date"
    @confirm="onConfirm"
    @cancel="showPicker = false"
  />
</van-popup>
<div style="margin: 16px;">
    <van-button round block type="info" native-type="submit">提交</van-button>
  </div>
                </van-form>
                
                <!-- <van-cell v-if="showAddLicenseForm" title="" label="点击选择图片">
                    <template #title>
                        <van-uploader :after-read="afterRead" v-model="fileList" :max-count="1"/>
                    </template>
                    
                    <template #right-icon>
                        <br/>
                       <van-button plain icon="plus" size="mini" type="primary" @click="add_license_item">新增</van-button>
                       <van-button plain block icon="plus" size="mini" type="primary" @click="add_license_item">新增</van-button>
                    </template>
                </van-cell> -->
            </van-collapse-item>
          </van-collapse>

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
                <van-cell v-if="!single_trans.is_buy" :title="single_trans.destination_company" center>
                    <template #right-icon>
                        <div style="margin-left:8px;">
                            <van-button v-if="should_checkin(single_trans.date) && !single_trans.is_registered && single_trans.destination_company" type="info" size="small" @click="register_vichele(single_trans.destination_company, single_trans.id)">排号</van-button>
                        </div>
                    </template>
                    <div v-if="single_trans.is_registered">
                        进厂序号：{{single_trans.register_number}}
                    </div>
                    <div v-if="single_trans.is_registered">
                        还需等待：{{single_trans.register_order}}个
                    </div>
                </van-cell>
                <div v-if="single_trans.is_buy">
                    <van-button v-if="!single_trans.order_company" type="info" size="small" @click="act_select_company = true;focus_vichele_index = index">指定拉货公司</van-button>
                    <div v-else>
                        <van-button v-if="single_trans.company_for_select.length > 0 && single_trans.is_buy" type="warning" size="small" @click="act_select_company = true;focus_vichele_index = index">修改拉货公司</van-button>
                        <van-cell title="磅单照片" center :label="single_trans.upload_permit?'允许上传':'当前位置不允许上传'">
                            <template #right-icon>
                                <van-button v-if="single_trans.attach_url" size="small" type="info" @click="pre_view_attach(single_trans.attach_url)">预览</van-button>
                                <van-uploader :after-read="upload_attachment" @click-upload="proc_focus(single_trans.id)" accept="image/*">
                                    <van-button :disabled="!single_trans.upload_permit" icon="plus" size="small" type="primary">上传</van-button>
                                </van-uploader>
                            </template>
                        </van-cell>
                        <div v-if="single_trans.attach_url">
                            <van-field label="出矿（厂）净重" type="number" v-model="input_enter_weight[index]" placeholder="请输入出厂净重"></van-field>
                            <van-field label="确认出矿（厂）净重" type="number" v-model="input_enter_weight_confirm[index]" placeholder="请再次输入出厂净重"></van-field>
                            <div style="margin:16px;">
                                <van-button type="primary" block size="small" @click="fill_enter_weight(single_trans.id, index)">提交</van-button>
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
                    <van-button v-if="current_count_down == 0" size="small" type="primary" native-type="button" @click="send_sms">发送验证码</van-button>
                    <van-count-down v-else format="ss秒后再次发送" :time="current_count_down" @finish="current_count_down = 0" />
                </template>
            </van-field>
            <van-field v-model="bind_info.id" name="身份证号" label="身份证号" placeholder="身份证号" :rules="[{ required: true, message: '请填写身份证号' }]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </div>
    
</div>
</template>

<script>
import {
    compressAccurately
} from 'image-conversion';
import {ImagePreview} from 'vant';
// import _ from 'lodash'

export default {
    name: 'DriverRegister',
    data: function () {
        return {
            activeNames: [],
            fileList: [],
            value:this.formatDateTime(),
            showPicker: false,
            showAddLicenseForm : false,
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
        company_for_select: function () {
            var ret = [];
            if (this.trans_info.length > 0) {
                this.trans_info[this.focus_vichele_index].company_for_select.forEach(element => {
                    ret.push({
                        name: element
                    });
                });
            }

            return ret;
        },
    },
    methods: {
         formatDateTime: function (date = new Date()) {
                var y = date.getFullYear();
                var m = date.getMonth() + 1;
                m = m < 10 ? ('0' + m) : m;
                var d = date.getDate();
                d = d < 10 ? ('0' + d) : d;
                return y + '-' + m + '-' + d;
            },
        add_license_item : function(){
            this.showAddLicenseForm = true;
            console.log(this.showAddLicenseForm);
            return 
        },
        onConfirm (date){
            this.showPicker = false;
            this.value = this.formatDateTime(date);
        },
        afterRead : function(file){
            console.log(file);
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


        async addDriverLicense(file_content){
            let expire_date = '2022-03-04';//TODO 在上传照片的时候默认给定一个过期时间，比如当前时间后5天
            let driverLiscenseInfo = await this.$call_remote_process("stuff_plan_management", "add_driver_license", [this.$cookies.get('driver_silent_id'), file_content, expire_date]);
            this.driver_license.push(driverLiscenseInfo);
        },
      async compressAndSend (_file, sendFn){
        let res = await compressAccurately(_file.file, 400);
        this.convert_2_base64_send(res, false);
        let reader = new FileReader();
        reader.onloadend = function(){
          let file_content = this.result.split(';base64,')[1]
          sendFn(file_content)
        }
      },
        // curried :  _.curryRight(methods.compressAndSend),
        // uploadDriverLicense : this.compressAndSend(this.addDriverLicense),
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
          this.$dialog.confirm({
                title: '重置确认',
                message: '只有电话或身份证等信息输入错误时才需要重置，确认重置吗？',
            }).then(() => {
                this.$dialog.alert({message:"请联系送货或收货公司负责人操作重置"});
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
.driver-title-cell{
    color : #969799
}
.single_record_show {
    margin-bottom: 5px;
    margin-right: 10px;
    margin-left: 10px;
    background-color: rgb(238, 238, 238);
    border: 1px solid black;
}
</style>
