<template>
<div class="admin_show">
    <van-cell v-for="(single_apply, index) in all_apply " :key="index" :label="'手机号:' + single_apply.phone" :title="single_apply.name + ' 加入公司申请'">
        <template #right-icon>
            <van-row v-if="single_apply.status == 0" type="flex" justify="center" align="center" :gutter="5">
                <van-col>
                    <van-button type="primary" @click="submit_approve(single_apply, true)">批准</van-button>
                </van-col>
                <van-col>
                    <van-button type="danger" @click="submit_approve(single_apply, false)">驳回</van-button>
                </van-col>
            </van-row>
            <div v-else-if="single_apply.status == 1">
                已批准
            </div>
            <div v-else>
                已驳回
            </div>
        </template>
        <template #icon>
            <van-image round width="40px" height="40px" :src="single_apply.logo" />
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
import {
    Col,
    Row
} from 'vant';

Vue.use(Col);
Vue.use(Row);
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
            vue_this.$get_client("company_management").approve_apply(_apply.apply_id, vue_this.$cookies.get('pa_ssid'), approve).then(function (resp) {
                if (resp) {
                    vue_this.init_apply_info();
                }
            }).catch(function (err) {
                console.log(err);
                vue_this.$toast(err.msg);
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
                vue_this.$toast(err.msg);
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
