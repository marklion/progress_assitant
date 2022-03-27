<template>
<div class="driver_license_dialog">
    <van-popup v-model="show_dialog" position="bottom" @open="loadDriverLicense" @close="driverLicenseList=[]">
        <van-empty description="无证件" v-if="driverLicenseList.length <= 0" />
        <driverLicensesView v-else :show-delete="false" :driver-license-list="driverLicenseList" @update="doLicenseUpdate" />
    </van-popup>
</div>
</template>

<script>
import {
    getAllLicenseInfoByDriverPhone,
    updateLicenseExpireDate
} from '@/api/driver'
import driverLicensesView from '@/components/DriverLicensesView'
export default {
    name: 'DriverLicenseDialog',
    props: {
        phone: String,
        show: Boolean,
    },
    model: {
        prop: 'show',
        event: 'change',
    },
    data: function () {
        return {
            driverLicenseList: [],
        };
    },
    computed: {
        show_dialog: {
            set(_x) {
                this.$emit('change', _x);
            },
            get() {
                return this.show;
            }
        },
    },
    components: {
        driverLicensesView
    },
    methods: {
        async loadDriverLicense() {
            this.driverLicenseList = await getAllLicenseInfoByDriverPhone(this.$cookies.get("pa_ssid"), this.phone);
            console.log(this.driverLicenseList);
        },
        async doLicenseUpdate(license) {
            await updateLicenseExpireDate('', this.$cookies.get("pa_ssid"), license);
            await this.loadDriverLicense();
        },
    },
}
</script>

<style>

</style>
