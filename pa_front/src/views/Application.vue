<template>
<div class="application_show">
    <van-grid>
        <van-grid-item v-for="app in apps" :key="app.app_id" icon="photo-o" :text="app.app_name" :to="{name:'CreateTicket',params: {app_id:app.app_id}, query: {app_name: app.app_name, app_description: app.app_description}}" />
    </van-grid>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Grid,
    GridItem
} from 'vant';

Vue.use(Grid);
Vue.use(GridItem);
export default {
    name: 'Application',
    data: function () {
        return {
            apps: [],
        };
    },
    beforeMount: function () {
        var vue_this = this;
        // eslint-disable-next-line
        var transport = new Thrift.Transport("https://www.d8sis.cn/pa_web/pa_rpc/");
        // eslint-disable-next-line
        var protocol = new Thrift.TJSONProtocol(transport);
        // eslint-disable-next-line
        var client = new config_managementClient(protocol);
        client.get_support_apps("ssid").then(function (resp) {
            console.log(resp);
        }).catch(function (err) {
            console.log(err);
        });
        vue_this.$axios.get('/apps/' + vue_this.$cookies.get('pa_ssid')).then(function (resp) {
            resp.data.result.forEach((element, index) => {
                vue_this.$set(vue_this.apps, index, element);
            });
        }).catch(function (err) {
            console.log(err);
        });
    }
}
</script>

<style>

</style>
