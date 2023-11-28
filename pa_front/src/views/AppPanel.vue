<template>
<div class="app_panel_show">
    <van-grid clickable :column-num="3">
        <van-grid-item icon="newspaper-o" text="设备管理" :to="{name:'DeviceStatus'}" v-if="$store.state.zc_rpc_url || $store.state.zczh_back_end" />
        <van-grid-item icon="bullhorn-o" text="出入管理" :to="{name:'VehicleQue'}" v-if="$store.state.zc_rpc_url || $store.state.zczh_back_end" />
        <van-grid-item v-if="$store.state.need_buy_feature" icon="bill-o" text="采购派车" :to="{name:'CompanyExtraVichele'}" />
        <van-grid-item v-if="$store.state.need_buy_feature" icon="desktop-o" text="采购计量" :to="{name:'BuyVehicleCount'}" />
        <van-grid-item v-if="$store.state.need_buy_feature" icon="records" text="采购统计" :to="{name:'BuyBrief'}" />
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
