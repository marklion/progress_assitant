<template>
<div class="admin_show">
    <van-tabs v-model="active">
        <van-tab title="待审批">
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
                    <van-image round width="40px" height="40px" :src="$remote_url + single_apply.logo" />
                </template>
            </van-cell>
        </van-tab>
        <van-tab title="所有员工">
            <van-cell v-for="(single_user, index) in all_user" :key="index" :label="'手机号' + single_user.phone" :title="single_user.name">
                <template #right-icon>
                    <van-button type="danger" plain size="small" @click="remove_user(single_user)">移除用户</van-button>
                </template>
            </van-cell>
        </van-tab>
    </van-tabs>

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
import {
    Tab,
    Tabs
} from 'vant';
import {
    Dialog
} from 'vant';
Vue.use(Tab);
Vue.use(Tabs);
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
            active: 0,
            all_user: [],
        };
    },
    methods: {
        remove_user: function (_user) {
            var vue_this = this;
            Dialog.confirm({
                message: '确定要移除' + _user.name + '吗',
            }).then(() => {
                vue_this.$call_remote_process("company_management", "remove_user_from_company", [vue_this.$cookies.get('pa_ssid'), _user.user_id]).then(function (resp) {
                    if (resp)
                    {
                        vue_this.init_all_user();
                    }
                });
            });
        },
        submit_approve: function (_apply, approve) {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", 'approve_apply', [_apply.apply_id, vue_this.$cookies.get('pa_ssid'), approve]).then(function (resp) {
                if (resp) {
                    vue_this.init_apply_info();
                }
            });
        },
        init_apply_info: function () {
            var vue_this = this;
            vue_this.all_apply = [];
            vue_this.$call_remote_process("company_management", 'get_all_apply', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_apply, index, element);
                });
            });
        },
        init_all_user: function () {
            var vue_this = this;
            vue_this.all_user = [];
            vue_this.$call_remote_process("company_management", "get_all_compay_user", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_user, index, element);
                });
            });
        },
    },
    beforeMount: function () {
        this.init_apply_info();
        this.init_all_user();
    },
}
</script>

<style>

</style>
