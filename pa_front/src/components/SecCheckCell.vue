<template>
<div class="sec_check_cell">
    <van-cell is-link @click="show_scd_edit" :icon="ap" :value="scd_online.expired_date" :label="scd_online.input_content">
        <template #title>
            <span>{{lr.name}}</span>
            <van-tag v-if="scd_online.id == 0" type="danger">未上传</van-tag>
            <van-tag v-else-if="scd_online.has_confirmed" type="success">已安检</van-tag>
            <van-tag v-else type="warning">已上传</van-tag>
        </template>
        <template #right-icon>
            <van-button size="mini" v-if="scd_online.id == 0" type="info">上传</van-button>
            <van-button size="mini" v-else type="warning">查看</van-button>
        </template>
    </van-cell>

    <van-dialog show-cancel-button cancel-button-text="取消" v-model="upload_diag" title="上传信息" get-container="body" closeOnClickOverlay @confirm="add_scd">
        <van-notice-bar left-icon="volume-o" :text="lr.prompt" />
        <van-form @submit="add_scd">
            <van-field v-if="lr.input_method.indexOf(1) != -1" v-model="new_scd.input_content" label="填写信息" :placeholder="'请填写' + lr.name" :rules="[{ required: true, message: '请填写' }]" />
            <van-field v-if="lr.input_method.indexOf(0) != -1" label="上传照片" :rules="[{message:'请上传图片', required:true }]">
                <template #input>
                    <van-uploader :after-read="upload_attachment" v-model="file_list" @delete="new_scd.attachment_path = ''" :max-count="1" />
                </template>
            </van-field>
            <van-field readonly clickable name="datetimePicker" :value="new_scd.expired_date" label="有效日期" placeholder="点击选择日期" @click="showPicker = true" :rules="[{ required: true, message: '请填写有效期' }]" />
            <van-popup v-model="showPicker" position="bottom">
                <van-datetime-picker :minDate="new Date()" type="date" @confirm="onConfirm" @cancel="showPicker = false" />
            </van-popup>
        </van-form>
    </van-dialog>
    <van-dialog show-cancel-button cancel-button-text="关闭" v-model="show_lcd_detail" title="详细信息" get-container="body" closeOnClickOverlay confirm-button-text="删除" @confirm="delete_lcd">
        <van-cell-group :title="lr.name">
            <van-cell v-if="scd_online.input_content" title="内容" :value="scd_online.input_content"></van-cell>
            <van-cell v-if="scd_online.attachment_path" title="照片">
                <van-image width="100" height="100" fit="contain" :src="$remote_url + scd_online.attachment_path" @click="preview_lcd_pic([$remote_url+scd_online.attachment_path]);" />
            </van-cell>
            <van-cell title="有效期" :value="scd_online.expired_date"></van-cell>
        </van-cell-group>
    </van-dialog>
</div>
</template>

<script>
import {
    compressAccurately
} from 'image-conversion';
import moment from 'moment'
import {
    ImagePreview
} from 'vant';
export default {
    name: "SecCheckCell",
    data: function () {
        return {
            file_list: [],
            related_info: '',
            showPicker: false,
            scd_online: {
                input_content: '',
                attachment_path: '',
                expired_date: '',
                id: 0,
                has_confirmed: false,
            },
            upload_diag: false,
            new_scd: {
                input_content: '',
                expired_date: "",
                attachment_path: '',
            },
            show_lcd_detail: false,
        };
    },
    computed: {
        ap: function () {
            var ret = "";
            if (this.scd_online.attachment_path) {
                ret = this.$remote_url + this.scd_online.attachment_path;
            }

            return ret;
        }
    },
    props: {
        lr: Object,
        mv: String,
        bv: String,
        driver: String,
    },
    methods: {
        delete_lcd: function () {
            var vue_this = this;
            vue_this.$dialog.confirm({
                    title: '删除确认',
                    message: '确定要删除该证件吗',
                })
                .then(() => {
                    vue_this.$call_remote_process("stuff_plan_management", "del_sec_check_data", [vue_this.$cookies.get("driver_silent_id"), vue_this.scd_online]).then(function () {
                        vue_this.show_lcd_detail = false;
                        vue_this.init_cell();
                    });
                });
        },
        preview_lcd_pic: function (_pic) {
            ImagePreview(_pic)
        },
        show_scd_edit: function () {
            if (this.scd_online.id > 0) {
                this.show_lcd_detail = true;
            } else {
                this.upload_diag = true;
            }
        },
        add_scd: function () {
            var vue_this = this;
            var add_req = {
                input_content: vue_this.new_scd.input_content,
                attachment_path: vue_this.new_scd.attachment_path,
                expired_date: vue_this.new_scd.expired_date,
                related_info: vue_this.related_info,
                related_type_id: vue_this.lr.id,
            };
            vue_this.$call_remote_process("stuff_plan_management", "add_sec_check_data", [vue_this.$cookies.get('driver_silent_id'), add_req]).then(function (resp) {
                if (resp) {
                    vue_this.upload_diag = false;
                    vue_this.init_cell();
                }
            });
        },
        convert_2_base64: function (_file) {
            var vue_this = this;
            var reader = new FileReader();
            reader.readAsDataURL(_file);
            reader.onloadend = function () {
                var result = this.result;
                var file_content = result.split(';base64,')[1];
                vue_this.new_scd.attachment_path = file_content;
                vue_this.file_list[0].status = 'done';
            };
        },
        upload_attachment: function (_file) {
            var vue_this = this;
            vue_this.file_list[0].status = 'uploading';
            compressAccurately(_file.file, 400).then(function (res) {
                vue_this.convert_2_base64(res);
            });
        },
        onConfirm: function (_date) {
            this.new_scd.expired_date = moment(_date).format('YYYY-MM-DD');
            this.showPicker = false;
        },
        init_cell: function () {
            var vue_this = this;
            var related_info = vue_this.driver;
            switch (vue_this.lr.use_for) {
                case 0:
                    break;
                case 1:
                    related_info = vue_this.mv;
                    break;
                case 2:
                    related_info = vue_this.bv;
                    break;
                default:
                    break;
            }
            vue_this.related_info = related_info;
            vue_this.$call_remote_process("stuff_plan_management", "get_all_sec_check_data", [vue_this.lr.id, related_info]).then(function (resp) {
                vue_this.scd_online.attachment_path = resp.attachment_path;
                vue_this.scd_online.input_content = resp.input_content;
                vue_this.scd_online.expired_date = resp.expired_date;
                vue_this.scd_online.id = resp.id;
                vue_this.scd_online.has_confirmed = resp.has_confirmed;
            });
        },
    },
    beforeMount: function () {
        this.init_cell();
    },

}
</script>

<style>

</style>
