<template>
<div class="admin_show">
    <van-cell v-for="(single_apply, index) in all_apply " :key="index" :lable="'手机号:' + single_apply.phone" title="加入公司申请">
        <van-image round width="60px" height="60px" :src="single_apply.logo" />
        {{single_apply.name}}
        <template #right-icon v-if="single_apply.status == 0">
            <van-button type="primary" @click="submit_approve(single_apply, true)">批准</van-button>
            <van-button type="danger" @click="submit_approve(single_apply, false)">驳回</van-button>
        </template>
    </van-cell>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup
} from 'vant';
import {
    Image as VanImage
} from 'vant';
import {
    Button
} from 'vant';

Vue.use(Button);
Vue.use(VanImage);
Vue.use(Cell);
Vue.use(CellGroup);
export default {
    name: 'Admin',
    data: function () {
        return {
            all_apply: [],
        };
    },
    methods: {
        submit_approve: function (_apply, approve) {
            var vue_this = this;
            vue_this.$get_client("company_management").appove_apply(_apply.apply_id, vue_this.$cookies.get('pa_ssid'), approve).then(function (resp) {
                if (resp)
                {
                    vue_this.init_apply_info();
                }
            }).catch(function (err) {
                console.log(err);
            });
        },
        init_apply_info: function () {
            var vue_this = this;
            vue_this.all_apply = [];
            vue_this.$get_client("company_management").get_all_apply(vue_this.$cookies.get('pa_ssid')).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_apply, index, element);
                });
            }).catch(function (err) {
                console.log(err);
            });

        },
    },
    beforeMount: function () {
        this.init_apply_info();
    },
}
</script>

<style>

</style>
