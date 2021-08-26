<template>
<div class="driver_register_show">
    <div v-if="is_login">
        <van-row type="flex" align="center">
            <van-col :span="20">
                <van-divider>司机信息</van-divider>
            </van-col>
            <van-col>
                <van-button size="small" type="danger" @click="reset_user">重置</van-button>
            </van-col>
        </van-row>
        <van-cell title="电话号" :value="driver_phone"></van-cell>
        <van-cell title="身份证号" :value="driver_id"></van-cell>
        <van-divider>今日承运信息</van-divider>
        <div class="single_record_show" v-for="(single_trans, index) in trans_info" :key="index">
            <van-cell :title="single_trans.main_vichele + '-' + single_trans.behind_vichele" :value="single_trans.stuff_name" :label="single_trans.order_company" />
            <van-cell :title="single_trans.destination_company" center>
                <template #right-icon>
                    <div style="margin-left:8px;">
                        <van-button v-if="!single_trans.is_registered" type="info" size="small" @click="register_vichele(single_trans.destination_company, single_trans.id)">排号</van-button>
                    </div>
                </template>
                <div v-if="single_trans.is_registered">
                    进厂序号：{{single_trans.register_number}}
                </div>
                <div v-if="single_trans.is_registered">
                    还需等待：{{single_trans.register_order}}个
                </div>
            </van-cell>
            <van-cell title="详细地址：" :value="single_trans.destination_address"></van-cell>
            <van-cell v-if="single_trans.is_registered" title="进厂位置：" :value="single_trans.enter_location" :label="'签到时间:' + single_trans.register_timestamp"></van-cell>
        </div>
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
import Vue from 'vue';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Button
} from 'vant';
import {
    Divider
} from 'vant';
import {
    Card
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    CountDown
} from 'vant';

Vue.use(CountDown);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Col);
Vue.use(Row);
Vue.use(Card);
Vue.use(Divider);
Vue.use(Button);
Vue.use(Form);
Vue.use(Field);
export default {
    name: 'DriverRegister',
    data: function () {
        return {
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
            current_count_down: 0,
        };
    },
    methods: {
        reset_user: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", 'driver_silent_unregister', [vue_this.$cookies.get('driver_silent_id')]).then(function () {
                vue_this.$router.go(0);
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

            try {
                var current_position = await vue_this.getLocation();
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
                vue_this.$toast("获取定位失败。");
            }

        },
        get_driver_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "get_driver_info", [vue_this.$cookies.get('driver_silent_id')]).then(function (resp) {
                vue_this.driver_id = resp.id;
                vue_this.driver_phone = resp.phone;
            });
        },
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
.single_record_show {
    margin-bottom: 5px;
    margin-right: 10px;
    margin-left: 10px;
    background-color: rgb(238, 238, 238);
    border: 1px solid black;
}
</style>
