<template>
<div class="self_info_show">
    <van-form @submit="on_submit">
        <van-field v-model="userinfo.name" name="姓名" label="姓名" placeholder="推荐填入真实姓名" :rules="[{ required: true, message: '请填写姓名' }]" />
        <van-field name="logo" label="头像">
            <template #input>
                <van-uploader v-model="logo" :after-read="upload_to_wx" :deletable="false">
                    <van-button type="primary">更换头像</van-button>
                </van-uploader>
            </template>
        </van-field>
        <van-field name="phone" v-model="userinfo.phone" label="手机号" placeholder="请填入手机号" :rules="[{ required: true, message: '请填写正确手机号', pattern: /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/ }]" />
        <van-field name="company_picker" v-model="userinfo.company" label="公司" placeholder="请填入所在公司" :rules="[{ required: true, message: '请填写所属公司' }]" />
        <van-field name="radio" label="身份">
            <template #input>
                <van-radio-group v-model="user_role" direction="horizontal">
                    <van-radio name="1">买方</van-radio>
                    <van-radio name="2">卖方</van-radio>
                </van-radio-group>
            </template>
        </van-field>
        <div style="margin: 16px;">
            <van-button round block type="info" native-type="submit">提交</van-button>
        </div>
    </van-form>
</div>
</template>

<script>
import Vue from 'vue';
import Exif from 'exif-js'
import {
    Form,
    Field
} from 'vant';
import {
    Popup
} from 'vant';
import {
    Picker
} from 'vant';
import {
    Uploader
} from 'vant';
import {
    Button
} from 'vant';
import {
    RadioGroup,
    Radio
} from 'vant';

Vue.use(Radio);
Vue.use(RadioGroup);
Vue.use(Button);
Vue.use(Uploader);
Vue.use(Picker);
Vue.use(Popup);
Vue.use(Field);
Vue.use(Form);
export default {
    name: 'SelfInfo',
    data: function () {
        return {
            logo: [{
                url: ''
            }],
            files: {
                name: '',
                type: '',
            },
            userinfo: {
                name: '',
                company: '',
                logo: '',
                buyer: false,
                phone: '',
            },
            admin: '',
        }
    },
    computed: {
        user_role: {
            get() {
                return this.userinfo.buyer ? '1' : '2';
            },
            set(_value) {
                this.userinfo.buyer = _value == '1' ? true : false;
            }
        },
    },
    beforeMount: function () {
        this.userinfo = this.$store.state.userinfo;
        this.logo[0].url = this.$remote_url + this.userinfo.logo;
    },
    watch: {
        "$store.state.userinfo": function (_new_value) {
            this.userinfo = _new_value;
            this.logo[0].url = this.$remote_url + this.userinfo.logo;
        }
    },
    methods: {
        on_submit: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", 'update_user_info', [vue_this.userinfo, vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                if (resp == true) {
                    var ssid = vue_this.$cookies.get('pa_ssid');
                    vue_this.$call_remote_process('user_management', 'get_user_info', [ssid]).then(function (resp) {
                        if (resp.user_id != 0) {
                            vue_this.$store.commit('set_userinfo', resp);
                            vue_this.$store.commit('set_login', true);
                        } else {
                            vue_this.$store.commit('set_userinfo', {
                                buyer: true,
                            });
                            vue_this.$store.commit('set_login', false);
                        }
                        if (vue_this.$route.query.from == 'auto') {
                            window.location.replace("/pa_web/");
                        } else {
                            vue_this.$router.go(-1);
                        }
                    });
                } else {
                    console.log(resp);
                }
            });
        },

        // 组件方法 获取 流
        upload_to_wx(file) {
            this.logo.splice(0, 1);
            this.files.name = file.file.name // 获取文件名
            this.files.type = file.file.type // 获取类型
            console.log(file);
            this.imgPreview(file.file)
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
        //将base64转换为文件
        dataURLtoFile(dataurl) {
            var arr = dataurl.split(','),
                bstr = atob(arr[1]),
                n = bstr.length,
                u8arr = new Uint8Array(n)
            while (n--) {
                u8arr[n] = bstr.charCodeAt(n)
            }
            return new File([u8arr], this.files.name, {
                type: this.files.type
            })
        },
        // 提交图片到后端
        postImg(base64) {
            console.log(this.logo);
            let file = this.dataURLtoFile(base64)
            console.log(file);
            console.log(base64);
            var file_content = base64.split(';base64,')[1];
            console.log(file_content);
            this.$call_remote_process("user_management", 'update_logo', [file_content, this.$cookies.get('pa_ssid')]).
            then(function (resp) {
                console.log(resp);
            });
            // 提交图片
            // Some code
        }
    },
}
</script>

<style>

</style>
