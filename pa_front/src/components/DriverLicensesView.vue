<template>
    <div>
        <van-cell v-for="(item, i) in driverLicenseList" :key="item.i64" :label="'有效期'" center>
            <template #title>
                <span>{{ item.expire_date }}
                    <van-tag v-if="item.expire_date < formatDateTime()" type="danger">已过期</van-tag>
                </span>
            </template>
            <template #icon>
                <van-image style="margin-right:10px" @click="previewLicense(i)"
                           width="50"
                           height="50"
                           :src="getFullImgPath(item.attachment_path)"/>
            </template>
            <template #right-icon>
                <van-button v-if="showExpireDateEdit" plain hairline icon="edit" size="small" type="default"
                            @click="callDateTimePicker(item)">有效期
                </van-button>
                <van-button v-if="showDelete" plain hairline icon="delete-o" size="small" type="default"
                            @click="operationHandler('del', item)"></van-button>
            </template>
        </van-cell>
        <van-popup v-model="showEditDatePicker" position="bottom">
            <van-datetime-picker
                type="date"
                :min-date="new Date()"
                @confirm="onConfirm"
                @cancel="showEditDatePicker = false"/>
        </van-popup>
    </div>
</template>

<script>

import {ImagePreview} from "vant";

export default {
    name: 'DriverLicensesView',
    props: {
        driverLicenseList: Array,
        showExpireDateEdit: {
            type: Boolean,
            default: true
        },
        showDelete: {
            type: Boolean,
            default: true
        }
    },
    data: function () {
        return {
            editingItem: null,
            showEditDatePicker: false
        };
    },
    methods: {
        formatDateTime: function (date = new Date()) {
            let y = date.getFullYear();
            let m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            let d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            return y + '-' + m + '-' + d;
        },
        getFullImgPath: function (path) {
            return this.$remote_url + path;
        },
        previewLicense(startIndex) {
            ImagePreview({
                images: this.driverLicenseList.map(item => this.getFullImgPath(item.attachment_path)),
                closeable: true,
                startPosition: startIndex
            });
        },
        callDateTimePicker(license) {
            this.editingItem = license;
            this.showEditDatePicker = true;
        },
        onConfirm(date) {
            this.showEditDatePicker = false;
            this.editingItem.expire_date = this.formatDateTime(date);
            this.operationHandler('update');
        },
        operationHandler(op, license = this.editingItem) {
            this.$emit(op, license)
        }
    },
}
</script>

<style scoped>
</style>
