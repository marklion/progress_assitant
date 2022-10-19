<template>
<div class="sec_check_mobile_show">
    <van-tabs v-model="cur_page">
        <van-tab title="安检执行">
            <van-search v-model="search_key" placeholder="过滤车号" />
            <van-collapse v-model="focus_vehicle_sec_check" accordion @change="handle_change_proc">
                <van-collapse-item v-for="(single_item, index) in vehicle_sec_check_show" :key="index" :title="single_item.main_vehicle_number + '-' + single_item.behind_vehicle_number" :name="index" :value="single_item.driver_name">
                    <template #icon>
                        <van-icon name="passed" size="1.2rem" v-if="single_item.sec_check_passed" color="green" />
                        <van-icon name="question-o" size="1.2rem" v-else color="red" />
                    </template>
                    <van-cell center v-for="(single_lr, lr_index) in  sec_items" :key="lr_index" :label="single_lr.expired_date">
                        <template #title>
                            <span>{{single_lr.name}}</span>
                            <span>
                                <van-tag type="danger" v-if="!single_lr.content_id || single_lr.content_id <= 0">未上传</van-tag>
                                <van-tag type="success" v-else-if="single_lr.has_confirmed">已审核</van-tag>
                                <van-tag type="warning" v-else>未审核</van-tag>
                            </span>
                        </template>
                        <div v-if="single_lr.input_content">
                            {{single_lr.input_content}}
                        </div>
                        <van-image @click="preview_lcd_pic([$remote_url+single_lr.attachment_path]);" v-if="single_lr.attachment_path" width="50" height="50" :src="$remote_url+single_lr.attachment_path" />
                        <template #right-icon>
                            <div v-if="single_lr.content_id && single_lr.content_id > 0">
                                <van-button v-if="!single_lr.has_confirmed" size="small" type="info" @click="confirm_lcd(single_lr, true)">审核</van-button>
                                <van-button v-else size="small" type="danger" @click="confirm_lcd(single_lr, false)">反审</van-button>
                            </div>
                        </template>
                    </van-cell>
                </van-collapse-item>
            </van-collapse>
        </van-tab>
        <van-tab title="安检配置">
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
        </van-tab>
    </van-tabs>

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
import {
    ImagePreview
} from 'vant';

import PinyinMatch from 'pinyin-match';
export default {
    name: "SecCheckMobile",
    data: function () {
        return {
            sec_check_item: [],
            today_vehicle: [],
            focus_vehicle_sec_check: "",
            cur_page: 0,
            sec_items: [],
            add_item_diag: false,
            new_item_name: "",
            new_item_prompt: "",
            new_item_use_for: 0,
            new_item_input_method: [],
            is_edit: false,
            focus_lr_id: 0,
            saved_cur_row: {},
            search_key: "",
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
    watch: {
        "$store.state.userinfo": function () {
            this.init_items();
        },
    },
    computed: {
        vehicle_sec_check_show: function () {
            var ret = [];
            var tmp_ret = [];
            if (this.search_key) {
                this.today_vehicle.forEach(element => {
                    if (PinyinMatch.match(element.vichele.main_vichele, this.search_key) || PinyinMatch.match(element.vichele.behind_vichele, this.search_key)) {
                        tmp_ret.push(element);
                    }
                });
            } else {
                tmp_ret = this.today_vehicle;
            }
            tmp_ret.forEach(element => {
                ret.push({
                    main_vehicle_number: element.vichele.main_vichele,
                    behind_vehicle_number: element.vichele.behind_vichele,
                    driver_name: element.vichele.driver_name,
                    driver_phone: element.vichele.driver_phone,
                    sec_check_passed: element.vichele.sec_check_passed,
                });
            });

            return ret;
        },
    },
    methods: {
        preview_lcd_pic: function (_pic) {
            ImagePreview(_pic)
        },
        confirm_lcd: function (_lcd, is_confirm) {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "confirm_sec_check_data", [vue_this.$cookies.get("pa_ssid"), _lcd.content_id, is_confirm]).then(function (resp) {
                if (resp) {
                    vue_this.handleCurrentChange(vue_this.saved_cur_row);
                }
            });
        },
        async handleCurrentChange(val) {
            var vue_this = this;
            vue_this.saved_cur_row = {
                ...val
            };
            for (let index = 0; index < vue_this.sec_items.length; index++) {
                const element = vue_this.sec_items[index];
                var relate_info = "";
                switch (element.use_for) {
                    case 0:
                        relate_info = val.driver_phone;
                        break;

                    case 1:
                        relate_info = val.main_vehicle_number;
                        break;
                    case 2:
                        relate_info = val.behind_vehicle_number;
                        break;
                    default:
                        break;
                }
                var tmp_lic = await vue_this.$call_remote_process("stuff_plan_management", "get_all_sec_check_data", [element.id, relate_info]);
                vue_this.$set(element, "input_content", tmp_lic.input_content);
                vue_this.$set(element, "attachment_path", tmp_lic.attachment_path);
                vue_this.$set(element, "expired_date", tmp_lic.expired_date);
                vue_this.$set(element, "content_id", tmp_lic.id);
                vue_this.$set(element, "has_confirmed", tmp_lic.has_confirmed);
            }
        },
        handle_change_proc: function (_index) {
            if (typeof (_index) == 'number') {
                this.handleCurrentChange(this.vehicle_sec_check_show[_index]);
            }
        },
        init_check_data: function () {
            var vue_this = this;
            vue_this.$call_remote_process("stuff_plan_management", "get_today_statistics", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                vue_this.today_vehicle = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.today_vehicle, index, element);
                });
            });
        },
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
                id: vue_this.focus_lr_id,
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
        this.init_check_data();
    },
}
</script>

<style scoped>
.swip-button {
    height: 100%;
}
</style>
