<template>
    <van-collapse v-model="activeNames" @change="!activeNames.includes('head') ? loadData() : ''">
        <van-collapse-item :icon="icon" :title="title" name="head">
            <van-cell class="driver-title-cell" title="证件图片" value="">
                <template #right-icon>
                    <van-button v-if="!showLicenseForm" icon="plus" size="mini" type="primary" @click="showLicenseForm = true">新增
                    </van-button>
                    <van-button v-if="showLicenseForm" icon="cross" size="mini" type="default" @click="showLicenseForm = false">取消
                    </van-button>
                </template>
            </van-cell>

            <van-form v-if="showLicenseForm" @submit="onSubmitLicense">
                <van-field name="licenseFile" label="证件照片" :rules="[{ required: true, message: '请选择要上传的图片' }]">
                    <template #input>
                        <van-uploader v-model="fileList" :max-count="1" />
                    </template>
                    <template #right-icon>
                        <van-icon name="info-o">点击选择图片</van-icon>
                    </template>
                </van-field>

                <van-field readonly clickable name="expireDate" :value="value" label="有效期" placeholder="请选择有效期截止日期" :rules="[{ required: true, message: '请选择过期时间' }]" @click="showLicenseDatePicker = true" />

                <van-popup v-model="showLicenseDatePicker" position="bottom">
                    <van-datetime-picker type="date" :min-date="current_date" @confirm="onConfirmLicenseDate" @cancel="showLicenseDatePicker = false" />
                </van-popup>
                <div style="margin: 16px;">
                    <van-button round block type="info" native-type="submit">提交</van-button>
                </div>
            </van-form>

            <driverLicensesView :driver-license-list="licenseList"
                                :showExpireDateEdit="showExpireDateEdit"
                                :showDelete="showDelete"
                                @update="doLicenseUpdate" @del="doLicenseDelete" />
        </van-collapse-item>
    </van-collapse>
</template>

<script>
import moment from 'moment'
import driverLicensesView from "@/components/DriverLicensesView";

export default {
    name: "LicenseCollapse",
    components: {
        driverLicensesView
    },
    props: {
        licenseList: Array,
        belong: {
            type : String,
            default : ''
        },
        icon: {
            type: String,
            default: ''
        },
        title: {
            type: String,
            default: '其他'
        },
        showExpireDateEdit: {
            type: Boolean,
            default: true
        },
        showDelete: {
            type: Boolean,
            default: true
        }
    },
    data(){
        return {
            activeNames : [],
            fileList: [],
            showLicenseForm : false,
            showLicenseDatePicker : false,
            current_date: new Date(),
            value: moment().format('YYYY-MM-DD'),
        }
    },
    methods : {
        loadData(){
            this.$emit('open')
        },
        onSubmitLicense(formData) {
            formData.belong = this.belong
            this.$emit('submit', formData, () => {
                this.showLicenseForm = false;
                this.fileList = [];
            })
        },
        onConfirmLicenseDate(date) {
            this.showLicenseDatePicker = false;
            this.value = moment(date).format('YYYY-MM-DD');
        },
        doLicenseUpdate(license){
            if(this.belong){
                license.belong = this.belong
            }
            this.$emit('update', license)
        },
        doLicenseDelete(license){
            if(this.belong){
                license.belong = this.belong
            }
            this.$emit('delete', license)
        }
    }
}
</script>

<style scoped>

</style>
