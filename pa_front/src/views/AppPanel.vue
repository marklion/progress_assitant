<template>
<div class="app_panel_show">
    <van-grid clickable :column-num="3">
        <van-grid-item icon="newspaper-o" text="设备管理" :to="{name:'DeviceStatus'}" v-if="$store.state.zc_rpc_url" />
        <van-grid-item icon="newspaper-o" text="出入管理" :to="{name:'VehicleQue'}" v-if="$store.state.zc_rpc_url" />
    </van-grid>
</div>
</template>

<script>
export default {
    name: 'AppPanel',
    data: function () {
        return {};
    },
}
</script>

<style>

</style>
