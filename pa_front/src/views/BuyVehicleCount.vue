<template>
<div class="buy_vehicle_count_show">
    <van-search v-model="search_key" placeholder="输入车号搜索" @input="search_more" />
    <van-list v-model="loading" :finished="finished" finished-text="没有更多了" @load="load_all_vehicle" ref="all_record">
        <div v-for="(single_vehicle, index) in vehicle_need_show" :key="index">
            <van-cell center :title="single_vehicle.main_vichele_number" :value="single_vehicle.stuff_name">
                <template #right-icon>
                    <van-button type="primary" size="small" @click="open_count_diag(single_vehicle)">计量</van-button>
                </template>
                <template #label>
                    <div>{{single_vehicle.driver_name + ' ' + single_vehicle.driver_phone}}</div>
                    <div>{{single_vehicle.company_name}}</div>
                </template>
            </van-cell>
        </div>
    </van-list>
    <van-dialog :show-confirm-button="false" close-on-click-overlay v-model="count_diag" title="采购计量" @open="fetch_scale_attach">
        <van-form @submit="upload_count">
            <van-field v-model="focus_vehicle.stuff_name" name="品名" label="品名" readonly />
            <van-field v-model="focus_vehicle.main_vichele_number" name="车号" label="车号" readonly />
            <van-field v-model="focus_vehicle.company_name" name="公司" label="公司" readonly />
            <van-field v-model="focus_vehicle.manual_count" name="卸车量" label="卸车量" type="number" :rules="[{ required: true, message: '请填写卸车量' }]" />
            <van-field name="uploader" label="附件上传">
                <template #input>
                    <van-uploader v-model="uploader" :after-read="upload_scale_attach" :before-delete="delete_scale_attach" />
                </template>
            </van-field>
            <div style="margin: 16px;">
                <van-button round block type="info" native-type="submit">确认</van-button>
            </div>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
import PinyinMatch from 'pinyin-match';
import {
    compressAccurately
} from 'image-conversion';

export default {
    name: "BuyVehicleCount",
    data() {
        return {
            uploader: [],
            search_key: '',
            all_vehicle: [],
            finished: false,
            loading: false,
            count_diag: false,
            focus_vehicle: {
                main_vichele_number: '',
                company_name: '',
                stuff_name: '',
                driver_name: '',
                driver_phone: '',
                manual_count: 0,
            },
        }
    },
    computed: {
        vehicle_need_show: function () {
            var ret = [];
            if (this.search_key) {
                this.all_vehicle.forEach(element => {
                    if (PinyinMatch.match(element.main_vichele_number, this.search_key)) {
                        ret.push(element);
                    }
                });
            } else {
                ret = this.all_vehicle;
            }
            return ret;
        },
    },
    methods: {
        delete_scale_attach: function (_file) {
            var vue_this = this;
            var p_url = _file.url.split(vue_this.$remote_url)[1];
            vue_this.$call_remote_process("vichele_management", "del_scale_attach", [vue_this.$cookies.get("pa_ssid"), vue_this.focus_vehicle.id, p_url]).then(function (resp) {
                if (resp) {
                    vue_this.fetch_scale_attach();
                }
            });
        },
        fetch_scale_attach: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", 'get_scale_attach', [vue_this.$cookies.get('pa_ssid'), vue_this.focus_vehicle.id]).then(function (resp) {
                vue_this.uploader = [];
                resp.forEach(element => {
                    vue_this.uploader.push({
                        url: vue_this.$remote_url + element
                    })
                })
            });
        },
        convert_2_base64_send: function (_file) {
            var vue_this = this;
            var reader = new FileReader();
            reader.readAsDataURL(_file);
            reader.onloadend = function () {
                var result = this.result;
                var file_content = result.split(';base64,')[1];
                vue_this.$call_remote_process("vichele_management", "add_scale_attach", [vue_this.$cookies.get('pa_ssid'), vue_this.focus_vehicle.id, file_content]).then(function (resp) {
                    if (resp) {
                        vue_this.fetch_scale_attach();
                    }
                });
            };
        },
        upload_scale_attach: function (file) {
            var vue_this = this;

            compressAccurately(file.file, 400).then(function (res) {
                vue_this.convert_2_base64_send(res);
            }).catch(function (err) {
                vue_this.$toast(err)
            });
        },
        open_count_diag: function (_vehicle) {
            this.focus_vehicle = {
                ..._vehicle
            };
            this.count_diag = true;
        },
        search_more: function () {
            this.$refs.all_record.check();
        },
        load_all_vehicle: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", 'get_company_vichele_info', [vue_this.$cookies.get('pa_ssid'), vue_this.all_vehicle.length, 1]).then(function (resp) {
                resp.forEach(element => {
                    vue_this.all_vehicle.push(element);
                });
                if (resp.length < 15) {
                    vue_this.finished = true;
                }
                vue_this.loading = false;
            });
        },
        upload_count: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", 'fillin_manual_count', [vue_this.$cookies.get('pa_ssid'), vue_this.focus_vehicle.id, vue_this.focus_vehicle.manual_count]).then(function (resp) {
                if (resp) {
                    vue_this.$toast.success('计量成功');
                    vue_this.$router.go(0);
                }
            });
        },
    },
}
</script>

<style>

</style>
