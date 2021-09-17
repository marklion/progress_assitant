<template>
<div class="bound_info_show">
    <van-form @submit="update_address_contact">
        <van-field v-model="address" rows="1" @input="show_address_contact_update_button = true" autosize label="公司地址" type="textarea" :placeholder="'请输入公司地址,例如：\n广东省广州市白云区XX路XX号'">
        </van-field>
        <van-field v-model="contact" rows="1" @input="show_address_contact_update_button = true" autosize label="联系方式" type="textarea" :placeholder="'请输入联系方式,例如：\n销售138XXXXXXXX\n泵房189XXXXXXXX'">
        </van-field>
        <div style="margin: 16px;">
            <van-button round block :disabled="!show_address_contact_update_button" type="info" native-type="submit" size="small">保存</van-button>
        </div>
    </van-form>
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>
                    <van-row type="flex" align="center">
                        <van-col>
                            资质证明
                        </van-col>
                        <van-col>
                            <van-popover v-model="show_attachment_prompt" trigger="click" placement="right-end">
                                <template #reference>
                                    <van-icon name="info-o" size="20px"></van-icon>
                                </template>
                                <div style="width: 240px;">LNG买家请上传公司营业执照和危化品经营许可证或燃气经营许可证</div>
                            </van-popover>
                        </van-col>
                    </van-row>
                </van-col>
                <van-col>
                    <van-uploader :after-read="upload_attachment" accept="image/*,.pdf">
                        <van-button icon="plus" size="small" type="primary">上传文件</van-button>
                    </van-uploader>
                </van-col>
            </van-row>
        </template>
        <van-cell v-for="(single_attach, index) in attachment" :key="index" :title="'证明' + (index + 1)">
            <template #right-icon>
                <van-button size="small" type="info" @click="pre_view_attach(single_attach.pic_path)">预览</van-button>
                <van-button size="small" type="danger" @click="remove_attach(single_attach)">删除</van-button>
            </template>
        </van-cell>
    </van-cell-group>
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>主车</van-col>
                <van-col>
                    <van-button size="small" type="primary" @click="show_main_vichele_add_diag = true">新增</van-button>
                </van-col>
            </van-row>
        </template>
        <van-cell v-for="(single_main_vichele, index) in main_vichele" :key="index" :value="single_main_vichele.number">
            <template #right-icon>
                <van-button size="small" plain type="danger" @click="remove_vichele(single_main_vichele.id, true)" hairline icon="delete-o"></van-button>
            </template>
        </van-cell>
    </van-cell-group>
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>挂车</van-col>
                <van-col>
                    <van-button size="small" type="primary" @click="show_behind_vichele_add_diag = true">新增</van-button>
                </van-col>
            </van-row>
        </template>
        <van-cell v-for="(single_behind_vichele, index) in behind_vichele" :key="index" :value="single_behind_vichele.number">
            <template #right-icon>
                <van-button size="small" plain type="danger" @click="remove_vichele(single_behind_vichele.id, false)" hairline icon="delete-o"></van-button>
            </template>
        </van-cell>
    </van-cell-group>
    <van-cell-group>
        <template #title>
            <van-row type="flex" justify="space-between" align="center">
                <van-col>司机信息</van-col>
                <van-col>
                    <van-button size="small" type="primary" @click="show_driver_add_diag= true">新增</van-button>
                </van-col>
            </van-row>
        </template>
        <van-cell v-for="(single_driver_info, index) in driver" :key="index" :title="single_driver_info.name" :value="single_driver_info.phone">
            <template #right-icon>
                <van-button style="margin-left:15px;" size="small" plain type="danger" @click="remove_driver(single_driver_info.id)" hairline icon="delete-o"></van-button>
            </template>
        </van-cell>
    </van-cell-group>
    <van-dialog v-model="show_driver_add_diag" title="绑定司机" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="add_driver">
            <van-field v-model="new_driver_name" name="司机" label="姓名" placeholder="请输入司机姓名" :rules="[{ required:true, message:'请输入司机姓名'}]" />
            <van-field v-model="new_driver_phone" name="司机电话" label="手机号" placeholder="请输入司机电话" :rules="[{ required:true, message:'请输入司机电话', pattern: phone_pattern}]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>

    <van-dialog v-model="show_main_vichele_add_diag" title="绑定主车" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="add_vichele(true)">
            <van-field v-model="new_vichele" name="车牌" label="车牌号" placeholder="请输入车牌号" :formatter="convert_bigger" :rules="[{ required:true, message:'请输入正确车牌', pattern: vichele_number_patten}]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>
    <van-dialog v-model="show_behind_vichele_add_diag" title="绑定挂车" :showConfirmButton="false" closeOnClickOverlay>
        <van-form @submit="add_vichele(false)">
            <van-field v-model="new_vichele" name="车牌" label="车牌号" placeholder="请输入车牌号" :formatter="convert_bigger" :rules="[{ required:true, message:'请输入正确车牌', pattern: vichele_number_patten}]" />
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">提交</van-button>
            </div>
        </van-form>
    </van-dialog>

</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Col,
    Row
} from 'vant';
import {
    Button
} from 'vant';
import {
    Dialog
} from 'vant';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Uploader
} from 'vant';
import {
    ImagePreview
} from 'vant';
import {
    compressAccurately
} from 'image-conversion';
import {
    Popover
} from 'vant';
import { Icon } from 'vant';

Vue.use(Icon);
Vue.use(Popover);
Vue.use(Uploader);
Vue.use(Field);
Vue.use(Form);
Vue.use(Dialog);
Vue.use(Button);
Vue.use(Col);
Vue.use(Row);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'BoundInfo',
    data: function () {
        return {
            show_address_contact_update_button: false,
            main_vichele: [],
            behind_vichele: [],
            driver: [],
            show_main_vichele_add_diag: false,
            show_behind_vichele_add_diag: false,
            show_driver_add_diag: false,
            new_vichele: '',
            new_driver_name: '',
            new_driver_phone: '',
            vichele_number_patten: /^(京[A-HJ-NPQY]|沪[A-HJ-N]|津[A-HJ-NPQR]|渝[A-DFGHN]|冀[A-HJRST]|晋[A-FHJ-M]|蒙[A-HJKLM]|辽[A-HJ-NP]|吉[A-HJK]|黑[A-HJ-NPR]|苏[A-HJ-N]|浙[A-HJKL]|皖[A-HJ-NP-S]|闽[A-HJK]|赣[A-HJKLMS]|鲁[A-HJ-NP-SUVWY]|豫[A-HJ-NP-SU]|鄂[A-HJ-NP-S]|湘[A-HJ-NSU]|粤[A-HJ-NP-Y]|桂[A-HJ-NPR]|琼[A-F]|川[A-HJ-MQ-Z]|贵[A-HJ]|云[AC-HJ-NP-SV]|藏[A-HJ]|陕[A-HJKVU]|甘[A-HJ-NP]|青[A-H]|宁[A-E]|新[A-HJ-NP-S])([0-9A-HJ-NP-Z]{4}[0-9A-HJ-NP-Z挂试]|[0-9]{4}学|[A-D0-9][0-9]{3}警|[DF][0-9A-HJ-NP-Z][0-9]{4}|[0-9]{5}[DF])$|^WJ[京沪津渝冀晋蒙辽吉黑苏浙皖闽赣鲁豫鄂湘粤桂琼川贵云藏陕甘青宁新]?[0-9]{4}[0-9JBXTHSD]$|^(V[A-GKMORTV]|K[A-HJ-NORUZ]|H[A-GLOR]|[BCGJLNS][A-DKMNORVY]|G[JS])[0-9]{5}$|^[0-9]{6}使$|^([沪粤川渝辽云桂鄂湘陕藏黑]A|闽D|鲁B|蒙[AEH])[0-9]{4}领$|^粤Z[0-9A-HJ-NP-Z][0-9]{3}[港澳]$/i,
            phone_pattern: /^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/,
            address: '',
            contact: '',
            attachment: [],
            show_attachment_prompt: false,
        };
    },
    methods: {
        convert_bigger: function (_value) {
            return _value.toLocaleUpperCase();
        },
        remove_attach: function (_attach) {
            var vue_this = this;
            Dialog.confirm({
                message: "确认要删除证明文件" + (vue_this.attachment.indexOf(_attach) + 1) + "吗?",
            }).then(function () {
                vue_this.$call_remote_process("company_management", "del_attachment", [vue_this.$cookies.get('pa_ssid'), _attach.id]).then(function () {
                    vue_this.init_attachment();
                });
            });
        },
        pre_view_attach: function (_remote_path) {
            ImagePreview({
                images: [this.$remote_url + _remote_path],
                closeable: true
            });
        },
        convert_2_base64_send: function (_file, _is_pdf) {
            var vue_this = this;
            var reader = new FileReader();
            reader.readAsDataURL(_file);
            reader.onloadend = function () {
                var result = this.result;
                var file_content = result.split(';base64,')[1];
                vue_this.$call_remote_process("company_management", "add_attachment", [vue_this.$cookies.get('pa_ssid'), file_content, _is_pdf]).then(function (resp) {
                    if (resp) {
                        vue_this.init_attachment();
                    }
                });
            };
        },
        upload_attachment: function (_file) {
            var vue_this = this;
            var file_is_pdf = true;
            if (/^image/.test(_file.file.type)) {
                file_is_pdf = false;
            }
            if (file_is_pdf) {
                vue_this.convert_2_base64_send(_file.file, true);
            } else {
                compressAccurately(_file.file, 400).then(function (res) {
                    vue_this.convert_2_base64_send(res, false);
                });

            }
        },

        update_address_contact: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "set_address", [vue_this.$cookies.get('pa_ssid'), vue_this.address]).then(function (resp) {
                if (resp) {
                    vue_this.$call_remote_process("company_management", "set_contact", [vue_this.$cookies.get('pa_ssid'), vue_this.contact]).then(function (resp) {
                        if (resp) {
                            vue_this.init_address_contact();
                            vue_this.init_address_contact();
                            vue_this.$toast("已保存");
                        }
                    });
                }
            });
        },
        add_driver: function () {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", "bind_new_driver", [vue_this.$cookies.get('pa_ssid'), {
                name: vue_this.new_driver_name,
                phone: vue_this.new_driver_phone
            }]).then(function (resp) {
                if (resp) {
                    vue_this.init_driver();
                    vue_this.show_driver_add_diag = false;
                }
            });
        },
        remove_driver: function (_id) {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", "remove_driver", [vue_this.$cookies.get('pa_ssid'), _id]).then(function () {
                vue_this.init_driver();
            });
        },
        remove_vichele: function (_id, _main_vichele) {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", "remove_vichele", [vue_this.$cookies.get('pa_ssid'), _id, _main_vichele]).then(function () {
                if (_main_vichele) {
                    vue_this.init_main_vichele();
                } else {
                    vue_this.init_behind_vichele();
                }
            });
        },
        add_vichele: function (_main_vichele) {
            var vue_this = this;
            vue_this.$call_remote_process("user_management", "bind_new_vichele", [vue_this.$cookies.get('pa_ssid'), vue_this.new_vichele, _main_vichele]).then(function (resp) {
                if (resp) {
                    if (_main_vichele) {
                        vue_this.init_main_vichele();
                    } else {
                        vue_this.init_behind_vichele();
                    }
                    vue_this.show_main_vichele_add_diag = false;
                    vue_this.show_behind_vichele_add_diag = false;
                }
            });
        },
        init_main_vichele: function () {
            var vue_this = this;
            vue_this.main_vichele = [];
            vue_this.$call_remote_process("user_management", "get_bound_vichele", [vue_this.$cookies.get('pa_ssid'), true]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.main_vichele, index, element);
                });
            });
        },
        init_behind_vichele: function () {
            var vue_this = this;
            vue_this.behind_vichele = [];
            vue_this.$call_remote_process("user_management", "get_bound_vichele", [vue_this.$cookies.get('pa_ssid'), false]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.behind_vichele, index, element);
                });
            });
        },
        init_driver: function () {
            var vue_this = this;
            vue_this.driver = [];
            vue_this.$call_remote_process("user_management", "get_bound_driver_info", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.driver, index, element);
                });
            });
        },
        init_address_contact: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_address", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.address = resp;
            });
            vue_this.$call_remote_process("company_management", "get_contact", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.contact = resp;
            });
        },
        init_attachment: function () {
            var vue_this = this;
            vue_this.attachment = [];
            vue_this.$call_remote_process("company_management", "get_all_attachment", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.attachment, index, element);
                });
                if (vue_this.attachment.length == 0) {
                    Dialog({
                        message: '请上传资质证明，支持图片和pdf格式',
                    });
                }
            });
        },
    },
    beforeMount: function () {
        this.init_behind_vichele();
        this.init_main_vichele();
        this.init_driver();
        this.init_address_contact();
        this.init_attachment();
    },
}
</script>

<style>

</style>
