<template>
<div class="sec_check_mobile_show">
    <van-cell-group inset>
        <van-cell v-for="(single_item, index) in sec_items" :key="index" :title="single_item.name">
            <template #right-icon>
                <van-button size="small" type="danger" @click="remove_item(single_item)">删除</van-button>
            </template>
        </van-cell>
        <template #title>
            <van-row>
                <van-col :span="20">安检项目</van-col>
                <van-col :span="4">
                    <van-button type="primary" @click="add_item_diag = true" size="small">新增</van-button>
                </van-col>
            </van-row>
        </template>
    </van-cell-group>
    <van-dialog v-model="add_item_diag" title="新增安检项目" closeOnClickOverlay :showConfirmButton="false">
        <van-form @submit="add_item">
            <van-field v-model="new_item_name" name="证件名称" label="证件名称" placeholder="请输入证件名称" :rules="[{ required: true, message: '请填写证件名称' }]" />
            <van-button plain block>确认</van-button>
        </van-form>
    </van-dialog>
</div>
</template>

<script>
export default {
    name: "SecCheckMobile",
    data: function () {
        return {
            sec_items: [],
            add_item_diag: false,
            new_item_name: "",
        };
    },
    methods: {
        add_item: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "add_license_require", [vue_this.$cookies.get('pa_ssid'), vue_this.new_item_name]).then(function (resp) {
                if (resp) {
                    vue_this.init_items();
                    vue_this.new_item_name = "";
                    vue_this.add_item_diag = false;
                }
            });
        },
        remove_item: function (_item) {
            var vue_this = this;
            vue_this.$dialog.confirm({
                title: '确认删除',
                message: '是否要删除' + _item.name + '吗？',
            }).then(() => {
                vue_this.$call_remote_process("company_management", "del_license_require", [vue_this.$cookies.get('pa_ssid'), _item.id]).then(function () {
                    vue_this.init_items();
                });
            });
        },
        init_items: function () {
            var vue_this = this;
            vue_this.$call_remote_process("company_management", "get_license_require", [vue_this.$store.state.userinfo.company]).then(function (resp) {
                vue_this.sec_items = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.sec_items, index, element);
                });
            });
        },
    },
    beforeMount: function () {
        this.init_items();
    },
}
</script>

<style>

</style>
