<template>
<div class="bind_company_show">
    <van-form @submit="on_submit">
        <van-field v-model="username" name="姓名" label="姓名" placeholder="推荐填入真实姓名" :rules="[{ required: true, message: '请填写姓名' }]" />
        <van-field name="logo" label="头像">
            <template #input>
                <van-uploader v-model="logo" :after-read="upload_to_wx" :deletable="false">
                    <van-button type="primary">更换头像</van-button>
                </van-uploader>
            </template>
        </van-field>
        <van-field readonly clickable name="company_picker" :value="company_picker" label="公司" placeholder="点击选择公司" @click="show_company_picker = true" :rules="[{ required: true, message: '请选择公司' }]" />
        <van-popup v-model="show_company_picker" position="bottom">
            <van-picker show-toolbar :columns="company_from_server" @confirm="on_company_confirm" @cancel="show_company_picker = false" />
        </van-popup>
        <van-field readonly clickable name="role_picker" :value="role_picker" label="角色" placeholder="点击选择角色" @click="show_role_picker = true" :rules="[{ required: true, message: '请选择角色' }]" />
        <van-popup v-model="show_role_picker" position="bottom">
            <van-picker show-toolbar :columns="role_from_server" @confirm="on_role_confirm" @cancel="show_role_picker = false" />
        </van-popup>
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

Vue.use(Button);
Vue.use(Uploader);
Vue.use(Picker);
Vue.use(Popup);
Vue.use(Field);
Vue.use(Form);
export default {
    name: 'BindCompany',
    data: function () {
        return {
            company_picker: '',
            role_picker: '',
            name_input: '',
            logo_img: '',
            show_company_picker: false,
            show_role_picker: false,
            company_from_server: [],
            role_from_server: [],
            username: '',
            logo: [{
                url: ''
            }],
            files: {
                name: '',
                type: '',
            },
        }
    },
    methods: {
        on_company_confirm: function (value) {
            this.company_picker = value;
            this.get_role_against_company(value);
            this.show_company_picker = false;
        },
        on_role_confirm: function (value) {
            this.role_picker = value;
            this.show_role_picker = false;
        },
        get_all_company: function () {
            var vue_this = this;
            vue_this.$axios.get('/all_companies').then(function (resp) {
                resp.data.result.forEach((element, index) => {
                    vue_this.$set(vue_this.company_from_server, index, element);
                });
            }).catch(function (err) {
                console.log(err);
            });

        },
        get_role_against_company: function (_company) {
            var vue_this = this;
            vue_this.role_from_server = [];
            vue_this.$axios.get('/all_roles/' + _company).then(function (resp) {
                resp.data.result.forEach((element, index) => {
                    vue_this.$set(vue_this.role_from_server, index, element);
                });
            }).catch(function (err) {
                console.log(err);
            });
        },
        on_submit: function () {
            var vue_this = this;
            vue_this.$axios.post('/userinfo', {
                pa_ssid: vue_this.$cookies.get('pa_ssid'),
                name: vue_this.username,
                company: vue_this.company_picker,
                role: vue_this.role_picker,
            }).then(function (resp) {
                if (resp.data.result == true) {
                    vue_this.$store.commit('set_userinfo', {
                        name: vue_this.username,
                        logo: vue_this.$store.state.userinfo.logo,
                        company: vue_this.company_picker,
                        role: vue_this.role_picker,
                    });
                    vue_this.$router.replace({
                        name: 'Home'
                    });
                } else {
                    console.log(resp);
                }
            }).catch(function (err) {
                console.log(err);
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
            this.$axios.post('/upload_img', {
                file_content: file_content,
                pa_ssid: this.$cookies.get('pa_ssid')
            }).then(function (resp) {
                console.log(resp);
            }).catch(function (err) {
                console.log(err);
            });
            // 提交图片
            // Some code
        }
    },
    watch: {
        "$store.state.userinfo": function (value) {
            this.username = value.name;
            this.logo[0].url = value.logo;
            this.role_picker = value.role;
            if ('' == this.company_picker) {
                this.company_picker = value.company;
            }
        }
    },
    beforeMount: function () {
        this.username = this.$store.state.userinfo.name;
        this.logo[0].url = this.$store.state.userinfo.logo;
        if ('' == this.company_picker) {
            this.company_picker = this.$store.state.userinfo.company;
        }
        this.role_picker = this.$store.state.userinfo.role;
        this.get_all_company();
        var vue_this = this;
        vue_this.$axios.get('/company/' + this.$route.query.company).then(function (resp) {
            if (resp.data.result != '') {
                vue_this.company_picker = resp.data.result;
                vue_this.get_role_against_company(vue_this.company_picker);
            }
        }).catch(function (err) {
            console.log(err);
        });
        vue_this.config_with_wx();
    },
}
</script>

<style>

</style>
