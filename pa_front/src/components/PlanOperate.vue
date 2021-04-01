<template>
<div class="plan_operate_show">
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>当前状态</van-col>
                <van-col v-if="$store.state.userinfo.buyer">
                    <van-button v-if="status == 0" size="small" type="warning" :to="{name:'PlanUpdate', params:{plan_id:plan_id}}">修改计划</van-button>
                    <van-uploader v-else-if="status == 2 || status == 1" :max-count="1" :after-read="upload_payinfo">
                        <van-button icon="plus" type="primary" size="small">上传付款凭证</van-button>
                    </van-uploader>
                    <van-button v-else-if="status == 3" size="small" type="info" icon="qr" @click="show_qr = true">展示提货码</van-button>
                </van-col>
                <van-col v-else>
                    <van-button v-if="status == 3" size="small" type="info" icon="scan" @click="confirm_close">确认提货</van-button>
                </van-col>
            </van-row>
        </template>

        <van-cell :value="cur_status" />
    </van-cell-group>
    <div style="margin: 16px;" v-if="has_pri">
        <van-button v-if="status == 0" round block type="primary" @click="submit_confirm">确认计划</van-button>
        <van-button v-if="status == 2" round block type="primary" @click="submit_confirm_pay">确认收款</van-button>
    </div>
    <van-dialog v-model="show_qr" title="提货码">
        <van-row type="flex" justify="center" align="center">
            <vue-qr :text="$remote_url + '/close_plan/' + plan_id" :margin="0" colorDark="#f67b29" colorLight="#fff" :logoScale="0.3" :size="200"></vue-qr>
        </van-row>
    </van-dialog>
</div>
</template>

<script>
import vueQr from 'vue-qr'
import Vue from 'vue';
import Exif from 'exif-js'
import {
    Button
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Uploader
} from 'vant';

Vue.use(Uploader);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Button);
import wx from 'weixin-js-sdk'
export default {
    name: 'PlanOperate',
    data: function () {
        return {
            has_pri: false,
            show_qr: false,
        };
    },
    props: {
        plan_id: Number,
        status: Number,
    },
    components: {
        "vue-qr": vueQr,
    },
    computed: {
        cur_status: function () {
            var ret = "等待商家确认计划";
            switch (this.status) {
                case 1:
                    ret = "等待买方付款";
                    break;
                case 2:
                    ret = "等待商家确认付款";
                    break;
                case 3:
                    ret = "等待买方提货";
                    break;
                case 4:
                    ret = "已提货";
                    break;
                default:
                    break;
            }

            return ret;
        }
    },
    watch: {
        plan_id(_id) {
            this.get_priv(_id);
        },
    },
    methods: {
        randomString: function (len) {
            len = len || 32;
            var $chars = 'ABCDEFGHJKMNPQRSTWXYZabcdefhijkmnprstwxyz2345678'; /****默认去掉了容易混淆的字符oOLl,9gq,Vv,Uu,I1****/
            var maxPos = $chars.length;
            var pwd = '';
            for (var i = 0; i < len; i++) {
                pwd += $chars.charAt(Math.floor(Math.random() * maxPos));
            }
            return pwd;
        },
        config_with_wx: function () {
            var timestamp = (new Date()).getTime();
            var nonceStr = this.randomString(32);
            var vue_this = this;
            vue_this.$get_client("user_management").get_wx_api_signature(timestamp, nonceStr, window.location.href).then(function (resp) {
                wx.config({
                    debug: false,
                    appId: 'wxa390f8b6f68e9c6d',
                    timestamp: timestamp,
                    nonceStr: nonceStr,
                    signature: resp,
                    jsApiList: ['scanQRCode']
                });
                wx.ready(function () {
                    console.log('success to config wx');
                });
                wx.error(function (err) {
                    console.log('fail to config wx');
                    console.log(err);
                });
            }).catch(function (err) {
                console.log(err);
            });
        },
        confirm_close: function () {
            wx.scanQRCode({
                needResult:1,
                success:function(res) {
                    var dest_url = res.resultStr;
                    window.location.href = dest_url;
                }
            });
        },
        upload_payinfo: function (_file) {
            this.imgPreview(_file.file);
        },
        // 处理图片
        imgPreview(file) {
            let self = this
            let Orientation
            //去获取拍照时的信息，解决拍出来的照片旋转问题   npm install exif-js --save   这里需要安装一下包
            Exif.getData(file, function () {
                Orientation = Exif.getTag(this, 'Orientation')
            })
            // 看支持不支持FileReader
            if (!file || !window.FileReader) return
            if (/^image/.test(file.type)) {
                // 创建一个reader
                let reader = new FileReader()
                // 将图片2将转成 base64 格式
                reader.readAsDataURL(file)
                // 读取成功后的回调
                reader.onloadend = function () {
                    let result = this.result
                    let img = new Image()
                    img.src = result
                    //判断图片是否大于500K,是就直接上传，反之压缩图片
                    if (this.result.length <= 500 * 1024) {
                        // 上传图片
                        self.postImg(this.result);
                    } else {
                        img.onload = function () {
                            let data = self.compress(img, Orientation)
                            // 上传图片
                            self.postImg(data);
                        }
                    }
                }
            }
        },
        // 压缩图片
        compress(img, Orientation) {
            let canvas = document.createElement('canvas')
            let ctx = canvas.getContext('2d')
            //瓦片canvas
            let tCanvas = document.createElement('canvas')
            let tctx = tCanvas.getContext('2d')
            // let initSize = img.src.length;
            let width = img.width
            let height = img.height
            //如果图片大于四百万像素，计算压缩比并将大小压至400万以下
            let ratio
            if ((ratio = (width * height) / 4000000) > 1) {
                // console.log("大于400万像素");
                ratio = Math.sqrt(ratio)
                width /= ratio
                height /= ratio
            } else {
                ratio = 1
            }
            canvas.width = width
            canvas.height = height
            //    铺底色
            ctx.fillStyle = '#fff'
            ctx.fillRect(0, 0, canvas.width, canvas.height)
            //如果图片像素大于100万则使用瓦片绘制
            let count
            if ((count = (width * height) / 1000000) > 1) {
                // console.log("超过100W像素");
                count = ~~(Math.sqrt(count) + 1) //计算要分成多少块瓦片
                //      计算每块瓦片的宽和高
                let nw = ~~(width / count)
                let nh = ~~(height / count)
                tCanvas.width = nw
                tCanvas.height = nh
                for (let i = 0; i < count; i++) {
                    for (let j = 0; j < count; j++) {
                        tctx.drawImage(img, i * nw * ratio, j * nh * ratio, nw * ratio, nh * ratio, 0, 0, nw, nh)
                        ctx.drawImage(tCanvas, i * nw, j * nh, nw, nh)
                    }
                }
            } else {
                ctx.drawImage(img, 0, 0, width, height)
            }
            //修复ios上传图片的时候 被旋转的问题
            if (Orientation != '' && Orientation != 1) {
                switch (Orientation) {
                    case 6: //需要顺时针（向左）90度旋转
                        this.rotateImg(img, 'left', canvas)
                        break
                    case 8: //需要逆时针（向右）90度旋转
                        this.rotateImg(img, 'right', canvas)
                        break
                    case 3: //需要180度旋转
                        this.rotateImg(img, 'right', canvas) //转两次
                        this.rotateImg(img, 'right', canvas)
                        break
                }
            }
            //进行最小压缩
            let ndata = canvas.toDataURL('image/jpeg', 0.1)
            tCanvas.width = tCanvas.height = canvas.width = canvas.height = 0;
            return ndata
        },
        // 旋转图片
        rotateImg(img, direction, canvas) {
            //最小与最大旋转方向，图片旋转4次后回到原方向
            const min_step = 0
            const max_step = 3
            if (img == null) return
            //img的高度和宽度不能在img元素隐藏后获取，否则会出错
            let height = img.height
            let width = img.width
            let step = 2
            if (step == null) {
                step = min_step
            }
            if (direction == 'right') {
                step++
                //旋转到原位置，即超过最大值
                step > max_step && (step = min_step)
            } else {
                step--
                step < min_step && (step = max_step)
            }
            //旋转角度以弧度值为参数
            let degree = (step * 90 * Math.PI) / 180
            let ctx = canvas.getContext('2d')
            switch (step) {
                case 0:
                    canvas.width = width
                    canvas.height = height
                    ctx.drawImage(img, 0, 0)
                    break
                case 1:
                    canvas.width = height
                    canvas.height = width
                    ctx.rotate(degree)
                    ctx.drawImage(img, 0, -height)
                    break
                case 2:
                    canvas.width = width
                    canvas.height = height
                    ctx.rotate(degree)
                    ctx.drawImage(img, -width, -height)
                    break
                case 3:
                    canvas.width = height
                    canvas.height = width
                    ctx.rotate(degree)
                    ctx.drawImage(img, -width, 0)
                    break
            }
        },
        // 提交图片到后端
        postImg(base64) {
            var file_content = base64.split(';base64,')[1];
            var vue_this = this;
            this.$get_client("stuff_plan_management").upload_payinfo(vue_this.plan_id, vue_this.$cookies.get('pa_ssid'), file_content).
            then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
            });
            // 提交图片
            // Some code
        },
        get_priv: function (_id) {
            var vue_this = this;
            vue_this.$get_client("stuff_plan_management").has_priv_edit(_id, vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                vue_this.has_pri = resp;
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
            });
        },
        submit_confirm: function () {
            var vue_this = this;
            vue_this.$get_client("stuff_plan_management").confirm_plan(vue_this.plan_id, vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
            });
        },
        submit_confirm_pay: function () {
            var vue_this = this;
            vue_this.$get_client("stuff_plan_management").confirm_pay(vue_this.plan_id, vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                if (resp) {
                    vue_this.$router.go(0);
                }
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
            });
        },
    },
    beforeMount: function () {
        this.get_priv(this.plan_id);
        this.config_with_wx();
    }
}
</script>

<style>

</style>
