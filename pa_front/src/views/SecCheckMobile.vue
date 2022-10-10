<template>
<div class="sec_check_mobile_show">
    <van-cell-group>
        <van-swipe-cell v-for="(single_item, index) in sec_items" :key="index">
            <van-cell :title="single_item.name" :border="false">
                <div>
                    {{conv_use_for(single_item.use_for)}}
                </div>
                <template #label>
                    <div v-for="(single_method, index) in single_item.input_method" :key="index">
                        {{conv_input_method( single_method)}}
                    </div>
                </template>
            </van-cell>
            <template #right>
                <van-button class="swip-button" square type="danger" text="删除" @click="remove_item(single_item)" />
                <van-button class="swip-button" square type="info" text="修改" @click="prepare_edit_data(single_item)" />
            </template>
        </van-swipe-cell>
        <template #title>
            <van-row>
                <van-col :span="20">安检项目</van-col>
                <van-col :span="4">
                    <van-button type="primary" @click="add_item_diag = true" size="small">新增</van-button>
                </van-col>
            </van-row>
        </template>
    </van-cell-group>
    <van-dialog v-model="add_item_diag" title="编辑安检项目" closeOnClickOverlay :showConfirmButton="false">
        <van-form @submit="add_item">
            <van-field v-model="new_item_name" name="项目名称" label="项目名称" placeholder="请输入项目名称" :rules="[{ required: true, message: '请填写项目名称' }]" />
            <van-field v-model="new_item_prompt" label="项目提示" placeholder="请输入项目提示语句" :rules="[{ required: true, message: '请填写提示语句' }]" />
            <van-field label="项目归属">
                <template #input>
                    <van-radio-group v-model="new_item_use_for" direction="horizontal">
                        <van-radio :name="0">司乘</van-radio>
                        <van-radio :name="1">主车</van-radio>
                        <van-radio :name="2">挂车</van-radio>
                    </van-radio-group>
                </template>
            </van-field>
            <van-field label="输入方式">
                <template #input>
                    <van-checkbox-group v-model="new_item_input_method" direction="horizontal">
                        <van-checkbox :name="0" shape="square">拍照</van-checkbox>
                        <van-checkbox :name="1" shape="square">输入</van-checkbox>
                    </van-checkbox-group>
                </template>
            </van-field>

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
            new_item_prompt: "",
            new_item_use_for: 0,
            new_item_input_method: [],
            is_edit: false,
            focus_lr_id: 0,
            conv_use_for(_use_for) {
                var ret = "";
                switch (_use_for) {
                    case 0:
                        ret = "司乘"
                        break;

                    case 1:
                        ret = "主车"
                        break;
                    case 2:
                        ret = "挂车"
                        break;
                    default:
                        ret = "司乘"
                        break;
                }
                return ret;
            },
            conv_input_method(_input_method) {
                var ret = "输入";
                if (_input_method == 0) {
                    ret = "拍照";
                }
                return ret;
            },
        };
    },
    methods: {
        prepare_edit_data: function (_item) {
            this.is_edit = true;
            this.new_item_name = _item.name;
            this.new_item_prompt = _item.prompt
            this.new_item_use_for = _item.use_for;
            this.new_item_input_method = _item.input_method;
            this.focus_lr_id = _item.id;
            this.add_item_diag = true;
        },
        add_item: function () {
            var vue_this = this;
            var req = {
                name: vue_this.new_item_name,
                prompt: vue_this.new_item_prompt,
                use_for: vue_this.new_item_use_for,
                input_method: vue_this.new_item_input_method,
                id:vue_this.focus_lr_id,
            };
            console.log(req);
            var req_func = "add_license_require";
            if (this.is_edit) {
                req_func = "update_license_require";
            }
            vue_this.$call_remote_process("company_management", req_func, [vue_this.$cookies.get('pa_ssid'), req]).then(function (resp) {
                if (resp) {
                    vue_this.init_items();
                    vue_this.new_item_name = "";
                    vue_this.new_item_prompt = "";
                    vue_this.new_item_use_for = 0;
                    vue_this.new_item_input_method = [];
                    vue_this.add_item_diag = false;
                    vue_this.is_edit = false;
                }
            });
        },
        remove_item: function (_item) {
            var vue_this = this;
            console.log(_item);
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

<style scoped>
.swip-button {
    height: 100%;
}
</style>
