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
                    <van-cell :value=" max_load(single_item).toFixed(2)">
                        <template #title>
                            <span>最大充装量</span>
                            <van-tag v-if="max_load(single_item) < (single_item.m_weight - single_item.p_weight)" type="danger">超量</van-tag>
                        </template>
                        <template #label>
                            <div>皮重：{{single_item.p_weight.toFixed(2)}}</div>
                            <div>毛重：{{single_item.m_weight.toFixed(2)}}</div>
                            <div v-if="single_item.m_weight != 0 && single_item.p_weight != 0">净重：{{(single_item.m_weight - single_item.p_weight).toFixed(2)}}</div>
                        </template>
                    </van-cell>
                    <van-cell center v-for="(single_lr, lr_index) in  sec_items" :key="lr_index">
                        <template #label>
                            <div v-if="single_lr.expired_date == '5000-01-01'">长期有效</div>
                            <div v-else>{{single_lr.expired_date}}</div>
                        </template>
                        <template #title>
                            <span>{{single_lr.name}}</span>
                            <span>
                                <van-tag type="danger" v-if="!single_lr.content_id || single_lr.content_id <= 0">未上传</van-tag>
                                <van-tag type="success" v-else-if="single_lr.has_confirmed">已审核</van-tag>
                                <van-tag type="warning" v-else>未审核</van-tag>
                            </span>
                            <div style="color:red;" v-if="single_lr.comment">驳回附言{{single_lr.comment}}</div>
                        </template>
                        <div v-if="single_lr.input_content">
                            {{single_lr.input_content}}
                        </div>
                        <van-image @click="preview_lcd_pic([$remote_url+single_lr.attachment_path]);" v-if="single_lr.attachment_path" width="50" height="50" :src="$remote_url+single_lr.attachment_path" />
                        <template #right-icon>
                            <div v-if="single_lr.content_id && single_lr.content_id > 0">
                                <div v-if="!single_lr.has_confirmed">
                                    <van-button size="small" type="info" @click="confirm_lcd(single_lr, true)">审核</van-button>
                                    <van-button size="small" type="warning" @click="comment_lcd(single_lr)">驳回附言</van-button>
                                </div>
                                <van-button v-else size="small" type="danger" @click="confirm_lcd(single_lr, false)">反审</van-button>
                            </div>
                            <van-button size="small" type="primary" @click="edit_scd(single_lr, single_item)">编辑</van-button>
                        </template>
                    </van-cell>
                </van-collapse-item>
            </van-collapse>
        </van-tab>
        <van-tab title="安检配置">
            <van-cell-group>
                <van-swipe-cell v-for="(single_item, index) in sec_items" :key="index">
                    <van-cell :title="single_item.name" :border="false">
                        <template #title>
                            <span>{{single_item.name}}</span>
                            <van-tag type="primary" v-if="single_item.ltv">长期</van-tag>
                        </template>
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
            <van-field name="switch" label="长期有效">
                <template #input>
                    <van-switch v-model="ltv" size="20" />
                </template>
            </van-field>
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

    <van-dialog @close="handleCurrentChange(saved_cur_row)" get-container="body" show-cancel-button cancel-button-text="关闭" v-model="sec_check_diag_show" title="安检" closeOnClickOverlay :showConfirmButton="false">
        <sec-check-cell :lr="edit_lr" :mv="sec_check_mv" :bv="sec_check_bv" :driver="sec_check_driver"></sec-check-cell>
    </van-dialog>
</div>
</template>

<script>
import Vue from 'vue';
import {
    ImagePreview
} from 'vant';
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
Vue.use(ElementUI);

import SecCheckCell from '@/components/SecCheckCell'
import PinyinMatch from 'pinyin-match';
export default {
    name: "SecCheckMobile",
    components: {
        "sec-check-cell": SecCheckCell,
    },
    data: function () {
        return {
            sec_check_mv: '',
            sec_check_bv: '',
            sec_check_driver: '',
            sec_check_diag_show: false,
            edit_lr: {},
            sec_check_item: [],
            today_vehicle: [],
            focus_vehicle_sec_check: "",
            cur_page: 0,
            sec_items: [],
            add_item_diag: false,
            new_item_name: "",
            new_item_prompt: "",
            ltv: false,
            new_item_use_for: 0,
            new_item_input_method: [],
            is_edit: false,
            focus_lr_id: 0,
            saved_cur_row: {},
            search_key: "",
            max_load(_vehicle) {
                var std_load = 0;
                var std_count = 0;
                var std_load_item = this.sec_items.find(item => {
                    return item.name == '槽车核载量'
                });
                var std_count_item = this.sec_items.find(item => {
                    return item.name == '槽车容积'
                });
                if (std_load_item && std_load_item.input_content) {
                    std_load = parseFloat(std_load_item.input_content);
                }
                if (std_count_item && std_count_item.input_content) {
                    std_count = parseFloat(std_count_item.input_content);
                }

                return this.$calc_max_load(std_load, std_count, _vehicle.p_weight);
            },
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
                    p_weight: element.vichele.p_weight,
                    m_weight: element.vichele.m_weight,
                });
            });
            ret.sort(function (a) {
                var sort_ret = -1;
                if (a.sec_check_passed) {
                    sort_ret = 1;
                }

                return sort_ret;
            });

            return ret;
        },
    },
    methods: {
        edit_scd: function (_lcd_info, _vehicle_info) {
            this.edit_lr = _lcd_info;
            this.sec_check_mv = _vehicle_info.main_vehicle_number;
            this.sec_check_bv = _vehicle_info.behind_vehicle_number;
            this.sec_check_driver = _vehicle_info.driver_phone;
            this.sec_check_diag_show = true;
        },
        preview_lcd_pic: function (_pic) {
            ImagePreview(_pic)
        },
        comment_lcd: function (_lcd) {
            var vue_this = this;
            this.$prompt('请输入驳回理由', '提示', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
                customClass: "reason_input",
                center: true,
            }).then(({
                value
            }) => {
                vue_this.$call_remote_process("stuff_plan_management", "confirm_sec_check_data", [vue_this.$cookies.get("pa_ssid"), _lcd.content_id, false, value]).then(function (resp) {
                    if (resp) {
                        vue_this.handleCurrentChange(vue_this.saved_cur_row);
                    }
                });
            });
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
                vue_this.$set(element, "comment", tmp_lic.comment);
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
            this.ltv = _item.ltv;
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
                ltv: vue_this.ltv,
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
                    vue_this.ltv = false;
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

.reason_input {
    width: 90%;
}
</style>
