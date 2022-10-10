<template>
<div class="sec_check_cell">
    <van-cell is-link @click="upload_diag = true;" :icon="ap" :value="scd_online.expired_date" :label="scd_online.input_content">
        <template #title>
            <span>{{lr.name}}</span>
            <van-tag v-if="scd_online.id == 0" type="danger">未上传</van-tag>
            <van-tag v-else-if="scd_online.has_confirmed" type="success">已安检</van-tag>
            <van-tag v-else type="warning">已上传</van-tag>
        </template>
    </van-cell>

    <van-dialog v-model="upload_diag" title="上传信息" get-container="body" closeOnClickOverlay :showConfirmButton="false">
        <van-notice-bar left-icon="volume-o" :text="lr.prompt" />
        <van-form @submit="add_scd">
            <van-field v-if="lr.input_method.indexOf(1) != -1" v-model="new_scd.input_content" label="填写信息" :placeholder="'请填写' + lr.name" :rules="[{ required: true, message: '请填写' }]" />
            <van-field v-if="lr.input_method.indexOf(0) != -1" label="上传照片">
                <template #input>
                    <van-uploader :after-read="upload_attachment" v-model="file_list" @delete="new_scd.attachment_path = ''" :max-count="1" />
                </template>
            </van-field>
            <van-field readonly clickable name="datetimePicker" :value="new_scd.expired_date" label="有效日期" placeholder="点击选择日期" @click="showPicker = true" />
            <van-popup v-model="showPicker" position="bottom">
                <van-datetime-picker :minDate="new Date()" type="date" @confirm="onConfirm" @cancel="showPicker = false" />
            </van-popup>
            <van-button plain block>确认</van-button>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import {
    compressAccurately
} from 'image-conversion';
import moment from 'moment'
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
                id: 0
            },
            upload_diag: false,
            new_scd: {
                input_content: '',
                expired_date: "",
                attachment_path: '',
            },
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
                if (resp.id > 0) {
                    vue_this.scd_online.attachment_path = resp.attachment_path;
                    vue_this.scd_online.input_content = resp.input_content;
                    vue_this.scd_online.expired_date = resp.expired_date;
                    vue_this.scd_online.id = resp.id;
                }
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
