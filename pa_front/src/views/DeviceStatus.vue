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
            else if (vue_this.$store.state.zczh_back_end)
            {
                vue_this.$axios.post(vue_this.$store.state.zczh_back_end + "/api/get_scale_sm_info", {
                }, {
                    headers:{
                        "Content-Type": "application/json",
                        "token": vue_this.$store.state.zczh_back_token,
                    }
                }).then(function (resp) {
                    resp.data.result.forEach((element, index) => {
                        console.log(element);
                        var tmp_v = {};
                        tmp_v.name = element.set_info.name;
                        tmp_v.enter_gate_is_close = element.front_gate_is_close;
                        tmp_v.exit_gate_is_close = element.back_gate_is_close;
                        tmp_v.type_id = 2;
                        tmp_v.scale_status = element.cur_state + " " + element.cur_plate;
                        tmp_v.cur_weight = element.cur_weight.toFixed(2);
                        vue_this.$set(vue_this.device_status, index, tmp_v);
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
        "$store.state.zczh_back_end": function () {
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
