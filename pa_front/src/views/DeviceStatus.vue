<template>
<div class="device_status_show">
    <scale-gate-panel v-for="(single_status, index) in device_status" :key="index" :cur_weight="single_status.cur_weight" :scale_status="single_status.scale_status" :name="single_status.name" :exit_is_close="single_status.exit_gate_is_close" :enter_is_close="single_status.enter_gate_is_close" :is_scale="single_status.type_id == 2" @refresh="init_device_status">

    </scale-gate-panel>
</div>
</template>

<script>
import ScaleGatePanel from '../components/ScaleGatePanel.vue'
export default {
    name: 'DeviceStatus',
    data: function () {
        return {
            device_status: [],
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
    },
    watch: {
        "$store.state.zc_rpc_url": function () {
            this.init_device_status();
        },
    },
    beforeMount: function () {
        this.init_device_status();
    }
}
</script>

<style>

</style>
