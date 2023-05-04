<template>
<div class="device_status_show">
    <van-divider>设备状态</van-divider>
    <scale-gate-panel v-for="(single_status, index) in device_status" :key="index" :cur_weight="single_status.cur_weight" :scale_status="single_status.scale_status" :name="single_status.name" :exit_is_close="single_status.exit_gate_is_close" :enter_is_close="single_status.enter_gate_is_close" :is_scale="single_status.type_id == 2" @refresh="init_device_status">

    </scale-gate-panel>

    <van-divider>装车位状态</van-divider>
    <van-grid column-num="6">
        <van-grid-item v-for="(single_dc, index) in all_dc_device" :key="index" :text="single_dc.name">
            <template #icon>
                <van-icon v-if="single_dc.has_vehicle" name="underway-o" color="red" />
                <van-icon v-else name="aim" color="green" />
            </template>
        </van-grid-item>
    </van-grid>
</div>
</template>

<script>
import ScaleGatePanel from '../components/ScaleGatePanel.vue'
export default {
    name: 'DeviceStatus',
    data: function () {
        return {
            device_status: [],

            all_dc_device: [],
        };
    },
    components: {
        "scale-gate-panel": ScaleGatePanel,
    },
    methods: {
        init_device_status: function () {
            var vue_this = this;
            if (vue_this.$store.state.zc_rpc_url) {
                vue_this.$call_remote_process_no_toast("open_api", 'get_device_status', [vue_this.$store.state.userinfo.phone], vue_this.$store.state.zc_rpc_url).then(function (resp) {
                    resp.forEach((element, index) => {
                        vue_this.$set(vue_this.device_status, index, element);
                    });
                });
            }
        },
        init_dc_device: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_dc_status", [vue_this.$cookies.get("pa_ssid")]).then(function (resp) {
                vue_this.all_dc_device = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_dc_device, index, element);
                });
            });
        },
    },
    watch: {
        "$store.state.zc_rpc_url": function () {
            this.init_device_status();
        },
    },
    beforeMount: function () {
        this.init_device_status();
        this.init_dc_device();
    }
}
</script>

<style>

</style>
