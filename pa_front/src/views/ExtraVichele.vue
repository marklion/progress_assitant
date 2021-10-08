<template>
<div class="extra_vichele_show" v-if="is_login">
    <van-tabs v-model="active">
        <van-tab title="新申请">
            <van-form @submit="onSubmit">
                <van-divider>承运信息</van-divider>
                <van-field v-model="user_name" label="提交人" placeholder="您的姓名" :rules="[{ required: true, message: '请填写姓名' }]" />
                <van-field v-model="user_phone" label="电话" type="tel" placeholder="您的手机号" :rules="[{ required: true, message: '请填写手机号' }]" />
                <history-input search_key="company_name" v-model="new_form.company_name" :rules="[{ required: true, message: '请填写公司名' }]"></history-input>
                <history-input search_key="destination" v-model="new_form.destination" :rules="[{ required: true, message: '请填写公司名' }]"></history-input>
                <history-input search_key="transfor_company" v-model="new_form.transfor_company" :rules="[{ required: true, message: '请填写公司名' }]"></history-input>
                <history-input search_key="stuff_name" v-model="new_form.stuff_name" :rules="[{ required: true, message: '请填写货物名称' }]"></history-input>
                <van-field v-model="new_form.price" label="单价" type="number" placeholder="请输入货品单价" :rules="[{ required: true, message: '请填写货品单价' }]" />
                <van-field center readonly clickable name="datetimePicker" :value="new_form.date" label="到厂日期" placeholder="点击选择日期" @click="show_time_picker = true">
                    <template #right-icon>
                        <van-tag type="primary">{{plan_time_easy}}</van-tag>
                    </template>
                </van-field>
                <van-popup v-model="show_time_picker" position="bottom">
                    <van-datetime-picker type="date" title="请选择日期" :min-date="new Date()" @cancel="show_time_picker = false" @confirm="confirm_time" />
                </van-popup>
                <van-divider>车辆信息</van-divider>
                <van-dialog v-model="team_brief_show" title="车队信息" :showConfirmButton="false" closeOnClickOverlay>
                    <van-form @submit="finish_team_brief">
                        <van-field v-model="team_brief_count" type="number" name="重量" label="重量" placeholder="请输入重量" :rules="[{ required: true, message: '请填写重量' }]" />
                        <van-field name="switch" label="多次进厂">
                            <template #input>
                                <van-switch v-model="team_brief_multi" size="20" />
                            </template>
                        </van-field>
                        <div style="margin: 16px;">
                            <van-button round block type="info" native-type="submit">提交</van-button>
                        </div>
                    </van-form>
                </van-dialog>
                <van-dialog v-model="add_single_vichele_show" title="选择车辆" :showConfirmButton="false" closeOnClickOverlay>
                    <div style="height: 70vh;overflow: auto;">
                        <van-search v-model="single_search_key" placeholder="请输入车牌号" />
                        <van-cell v-for="(single_vichele, index) in vichele_searched" :key="index" :title="single_vichele.main_vichele_number">
                            <template #right-icon>
                                <van-button type="primary" size="small" @click="add_single_vichele(single_vichele)">选择</van-button>
                            </template>
                        </van-cell>
                    </div>
                </van-dialog>
                <van-row type="flex" align="center" justify="space-between">
                    <van-col :span="12">
                        <van-button type="info" size="small" round native-type="button" block @click="team_brief_show= true">选择车队</van-button>
                    </van-col>
                    <van-col :span="12">
                        <van-button type="primary" size="small" round native-type="button" block @click="add_single_vichele_show = true">选择车辆</van-button>
                    </van-col>
                </van-row>
                <van-action-sheet v-model="show_select_vichele_team" :actions="all_vichele_team" @select="select_team" />
                <div class="vichele_show" v-for="(single_vichele, index) in new_vichele" :key="index" ref="new_vichele">
                    <history-input search_key="main_vichele_number" v-model="single_vichele.main_vichele_number" :formatter="convert_bigger" :rules="[{ required: true, message: '请填写车牌号' }, {pattern: vichele_number_patten, message: '请填写正确的车牌号'}]"></history-input>
                    <history-input search_key="behind_vichele_number" v-model="single_vichele.behind_vichele_number" :formatter="convert_bigger" :rules="[{validator:have_to_have_gua, message: '请输入正确挂车'}]"></history-input>
                    <van-field v-model="single_vichele.count" type="number" name="重量" label="重量（吨）" placeholder="重量" :rules="[{ required: true, message: '请填写重量' }]" />
                    <history-input search_key="comment" v-model="single_vichele.comment"></history-input>
                    <history-input search_key="driver_name" v-model="single_vichele.driver_name" :rules="[{ required: true, message: '请填写司机姓名' }]"></history-input>
                    <history-input search_key="driver_id" v-model="single_vichele.driver_id" :rules="[{ required: true, message: '请填写司机身份证' }, {pattern:/^[1-9]\d{5}(18|19|20|(3\d))\d{2}((0[1-9])|(1[0-2]))(([0-2][1-9])|10|20|30|31)\d{3}[0-9Xx]$/, message:'请输入正确的身份证'}]"></history-input>
                    <history-input search_key="driver_phone" v-model="single_vichele.driver_phone" :rules="[{ required: true, message: '请填写司机电话' }, {pattern:/^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/, message:'请输入正确手机号'}]"></history-input>
                    <van-field name="switch" label="多次进厂">
                        <template #input>
                            <van-switch v-model="single_vichele.repeated" size="20" />
                        </template>
                    </van-field>
                    <van-button type="danger" block size="small" native-type="button" plain round @click="remove_vichele(index)">移除</van-button>
                </div>
                <van-row type="flex" align="center" justify="right">
                    <van-col :offset="20">
                        <van-button size="small" type="primary" native-type="button" @click="add_vichele">增加</van-button>
                    </van-col>
                </van-row>
                <div style="margin: 16px;">
                    <van-button round block type="info" native-type="submit">提交</van-button>
                </div>
            </van-form>
        </van-tab>
        <van-tab title="历史申请">
            <van-list ref="all_record" v-model="loading" :finished="finished" finished-text="没有更多了" @load="onLoad">
                <div v-for="(item,index) in created_apply" :key="index" class="one_record_show" :id="item.id" ref="single_record" :class="{highlight_record:$route.query.pos == item.id}">
                    <van-cell center :label="item.stuff_name + '(' + item.destination + ')'">
                        <template #title>
                            <div>{{item.main_vichele_number}}</div>
                            <div v-if="item.behind_vichele_number">{{item.behind_vichele_number}}</div>
                        </template>
                        <div>单价:{{item.price}}</div>
                        <div>发货净重{{item.count}}吨</div>
                        <div v-if="item.status == 2">收货净重{{item.j_weight}}吨</div>
                        <div v-if="item.comment">备注：{{item.comment}}</div>
                        <template #right-icon>
                            <div class="opt_button_show" v-if="item.status == 0 || item.status != 2">
                                <van-button v-if="item.status == 0" type="info" block size="small" @click="update_vichele(item)">修改</van-button>
                                <van-button v-if="item.status != 2" type="danger" block size="small" @click="delete_vichele(item)">取消</van-button>
                            </div>
                        </template>
                    </van-cell>
                    <div style="margin-left:15px;">
                        <van-row type="flex" align="center" :gutter="10">
                            <van-col>
                                <van-tag plain type="primary">进厂时间：{{item.date}}</van-tag>
                            </van-col>
                            <van-col>
                                <van-tag plain type="warning" v-if="item.repeated">多次进厂</van-tag>
                            </van-col>
                            <van-col>
                                <van-tag v-if="item.status == 1" plain type="success">已确认</van-tag>
                                <van-tag v-else-if="item.status == 2" plain type="primary">已确认</van-tag>
                                <van-tag v-else plain type="danger">未确认</van-tag>
                            </van-col>
                        </van-row>
                        <van-row v-if="item.status == 2" type="flex" align="center" :gutter="10">
                            <van-col>皮重</van-col>
                            <van-col>{{item.p_weight}}吨</van-col>
                            <van-col>毛重</van-col>
                            <van-col>{{item.m_weight}}吨</van-col>
                        </van-row>
                    </div>
                </div>
            </van-list>
            <van-dialog v-model="change_show" title="修改车辆信息" :showConfirmButton="false" closeOnClickOverlay>
                <van-form @submit="onChange">
                    <van-field center readonly clickable name="datetimePicker" :value="vichele_update_info.date" label="到厂日期" placeholder="点击选择日期" @click="show_time_update_picker = true">
                    </van-field>
                    <van-popup v-model="show_time_update_picker" position="bottom">
                        <van-datetime-picker type="date" title="请选择日期" :min-date="new Date()" @cancel="show_time_update_picker = false" @confirm="confirm_update_time" />
                    </van-popup>
                    <history-input search_key="main_vichele_number" v-model="vichele_update_info.main_vichele_number" :formatter="convert_bigger" :rules="[{ required: true, message: '请填写车牌号' }, {pattern: vichele_number_patten, message: '请填写正确的车牌号'}]"></history-input>
                    <history-input search_key="behind_vichele_number" v-model="vichele_update_info.behind_vichele_number" :formatter="convert_bigger" :rules="[{validator:have_to_have_gua, message: '请输入正确挂车'}]"></history-input>
                    <history-input search_key="stuff_name" v-model="vichele_update_info.stuff_name" :rules="[{ required: true, message: '请填写货物名称' }]"></history-input>
                    <van-field v-model="vichele_update_info.price" label="单价" type="number" placeholder="请输入货品单价" :rules="[{ required: true, message: '请填写货品单价' }]" />
                    <van-field v-model="vichele_update_info.count" type="number" name="重量" label="重量（吨）" placeholder="重量" :rules="[{ required: true, message: '请填写重量' }]" />
                    <history-input search_key="comment" v-model="vichele_update_info.comment"></history-input>
                    <div style="margin: 16px;">
                        <van-button round block type="info" native-type="submit">提交</van-button>
                    </div>
                </van-form>
            </van-dialog>
        </van-tab>
        <van-tab title="车队维护">
            <van-grid>
                <van-grid-item v-for="(single_vichele_team, index) in all_vichele_team" :key="index" icon="logistics" :text="single_vichele_team.name" @click="current_vichele_team = single_vichele_team" />
                <van-grid-item v-if="all_vichele_team.length < 32" @click="add_vichele_team_show = true" icon="plus" />
            </van-grid>
            <div v-if="current_vichele_team.id != 0">
                <van-field v-model="current_vichele_team.name" label="车队名">
                    <template #button>
                        <van-button size="small" type="info" @click="update_vichele_team">改名</van-button>
                        <van-button size="small" type="danger" @click="remove_vichele_team">删除</van-button>
                    </template>
                </van-field>
                <van-divider>车队成员</van-divider>
                <van-button type="primary" block round icon="plus" @click="add_team_member_show = true">新增车辆</van-button>
                <div class="team_member_show" v-for="(single_member, index) in current_vichele_team.members" :key="index">
                    <van-field v-model="single_member.main_vichele_number" name="主车牌" label="主车牌" :formatter="convert_bigger" placeholder="请输入主车牌" :rules="[{ required: true, message: '请填写车牌号' }, {pattern: vichele_number_patten, message: '请填写正确的车牌号'}]" />
                    <van-field v-model="single_member.behind_vichele_number" name="挂车牌" label="挂车牌" :formatter="convert_bigger" placeholder="请输入挂车牌" :rules="[{validator:have_to_have_gua, message: '请输入正确挂车'}]" />
                    <van-field v-model="single_member.driver_name" name="司机姓名" label="司机姓名" placeholder="请输入司机姓名" :rules="[{ required: true, message: '请填写司机姓名' }]" />
                    <van-field v-model="single_member.driver_phone" name="司机电话" label="司机电话" placeholder="请输入司机电话" :rules="[{ required: true, message: '请填写司机电话'}, {pattern:/^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/, message:'请输入正确手机号'}]" />
                    <van-field v-model="single_member.driver_id" name="司机身份证" label="司机身份证" placeholder="请输入司机身份证" :rules="[{ required: true, message: '请填写司机身份证'}, {pattern:/^[1-9]\d{5}(18|19|20|(3\d))\d{2}((0[1-9])|(1[0-2]))(([0-2][1-9])|10|20|30|31)\d{3}[0-9Xx]$/, message:'请输入正确的身份证'}]" />
                    <van-row type="flex" align="center" justify="space-between">
                        <van-col :span="12">
                            <van-button size="small" round block type="warning" @click="update_vichele_team">更新</van-button>
                        </van-col>
                        <van-col :span="12">
                            <van-button size="small" round block type="danger" @click="remove_team_member(index)">删除</van-button>
                        </van-col>
                    </van-row>
                </div>
            </div>
            <van-dialog v-model="add_vichele_team_show" title="新增车队" :showConfirmButton="false" closeOnClickOverlay>
                <van-form @submit="add_vichele_team">
                    <van-field v-model="new_vichele_team_name" name="车队名" label="车队名" placeholder="请输入车队名" :rules="[{ required: true, message: '请填写车队名' }]" />
                    <div style="margin: 16px;">
                        <van-button round block type="info" native-type="submit">提交</van-button>
                    </div>
                </van-form>
            </van-dialog>
            <van-dialog v-model="add_team_member_show" title="新增车辆" :showConfirmButton="false" closeOnClickOverlay>
                <van-form @submit="add_team_member">
                    <van-field v-model="new_team_member.main_vichele_number" name="主车牌" label="主车牌" :formatter="convert_bigger" placeholder="请输入主车牌" :rules="[{ required: true, message: '请填写车牌号' }, {pattern: vichele_number_patten, message: '请填写正确的车牌号'}]" />
                    <van-field v-model="new_team_member.behind_vichele_number" name="挂车牌" label="挂车牌" :formatter="convert_bigger" placeholder="请输入挂车牌" :rules="[{validator:have_to_have_gua, message: '请输入正确挂车'}]" />
                    <van-field v-model="new_team_member.driver_name" name="司机姓名" label="司机姓名" placeholder="请输入司机姓名" :rules="[{ required: true, message: '请填写司机姓名' }]" />
                    <van-field v-model="new_team_member.driver_phone" name="司机电话" label="司机电话" placeholder="请输入司机电话" :rules="[{ required: true, message: '请填写司机电话'}, {pattern:/^(13[0-9]|14[01456879]|15[0-35-9]|16[2567]|17[0-8]|18[0-9]|19[0-35-9])\d{8}$/, message:'请输入正确手机号'}]" />
                    <van-field v-model="new_team_member.driver_id" name="司机身份证" label="司机身份证" placeholder="请输入司机身份证" :rules="[{ required: true, message: '请填写司机身份证'}, {pattern:/^[1-9]\d{5}(18|19|20|(3\d))\d{2}((0[1-9])|(1[0-2]))(([0-2][1-9])|10|20|30|31)\d{3}[0-9Xx]$/, message:'请输入正确的身份证'}]" />
                    <div style="margin: 16px;">
                        <van-button round block type="info" native-type="submit">提交</van-button>
                    </div>
                </van-form>
            </van-dialog>
        </van-tab>
    </van-tabs>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Form
} from 'vant';
import {
    Field
} from 'vant';
import {
    Button
} from 'vant';
import {
    Divider
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
    List
} from 'vant';
import {
    Cell,
    CellGroup
} from 'vant';
import HistoryInput from '../components/HistoryInput.vue'
import {
    DatetimePicker
} from 'vant';
import {
    Tag
} from 'vant';
import {
    Dialog
} from 'vant';

import {
    Checkbox,
    CheckboxGroup
} from 'vant';
import {
    Switch
} from 'vant';
import {
    Grid,
    GridItem
} from 'vant';
import {
    ActionSheet
} from 'vant';
import {
    Search
} from 'vant';
import PinyinMatch from 'pinyin-match';

Vue.use(Search);
Vue.use(ActionSheet);
Vue.use(Grid);
Vue.use(GridItem);
Vue.use(Switch);

Vue.use(Checkbox);
Vue.use(CheckboxGroup);
// 全局注册
Vue.use(Dialog);
Vue.use(Tag);
Vue.use(DatetimePicker);
Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(List);
Vue.use(Tab);
Vue.use(Tabs);
Vue.use(Col);
Vue.use(Row);
Vue.use(Divider);
Vue.use(Button);
Vue.use(Form);
Vue.use(Field);
export default {
    name: 'ExtraVichele',
    components: {
        'history-input': HistoryInput,
    },
    computed: {
        plan_time_easy: function () {
            var setted_time = new Date(/\d{4}-\d{1,2}-\d{1,2}/g.exec(this.new_form.date)[0]);
            var current_time = new Date();
            current_time.setHours(0, 0, 0, 0);
            var ret = "";
            var bigger_than = parseInt((setted_time.getTime() - current_time.getTime()) / (1000 * 60 * 60 * 24));
            if (bigger_than == 0) {
                ret = "今天";
            } else if (bigger_than == 1) {
                ret = "明天";
            } else {
                ret = bigger_than.toString() + "天后";
            }

            return ret;
        },
        vichele_searched: function () {
            var vue_this = this;
            var all_vicheles = [];
            var ret = [];
            vue_this.all_vichele_team.forEach(team_ele => {
                team_ele.members.forEach(element => {
                    all_vicheles.push(element);
                });
            });

            if (vue_this.single_search_key.length <= 0) {
                ret = all_vicheles;
            } else {
                all_vicheles.forEach(element => {
                    if (PinyinMatch.match(element.main_vichele_number, vue_this.single_search_key)) {
                        ret.push(element);
                    }
                });
            }

            return ret;
        },
    },
    data: function () {
        return {
            single_search_key: '',
            add_single_vichele_show: false,
            team_brief_show: false,
            team_brief_count: 0,
            team_brief_multi: false,
            show_select_vichele_team: false,
            add_team_member_show: false,
            new_team_member: {
                main_vichele_number: '',
                behind_vichele_number: '',
                driver_name: '',
                driver_id: '',
                driver_phone: '',
            },
            current_vichele_team: {
                id: 0,
                name: '',
                members: [],
            },
            new_vichele_team_name: "",
            add_vichele_team_show: false,
            all_vichele_team: [],
            user_name: '',
            user_phone: '',
            active: 0,
            show_time_update_picker: false,
            change_show: false,
            vichele_update_info: {
                date: '',
                main_vichele_number: '',
                behind_vichele_number: '',
                count: '',
                comment: '',
                stuff_name: '',
            },
            show_time_picker: false,
            vichele_number_patten: /^(京[A-HJ-NPQY]|沪[A-HJ-N]|津[A-HJ-NPQR]|渝[A-DFGHN]|冀[A-HJRST]|晋[A-FHJ-M]|蒙[A-HJKLM]|辽[A-HJ-NP]|吉[A-HJK]|黑[A-HJ-NPR]|苏[A-HJ-N]|浙[A-HJKL]|皖[A-HJ-NP-S]|闽[A-HJK]|赣[A-HJKLMS]|鲁[A-HJ-NP-SUVWY]|豫[A-HJ-NP-SU]|鄂[A-HJ-NP-S]|湘[A-HJ-NSU]|粤[A-HJ-NP-Y]|桂[A-HJ-NPR]|琼[A-F]|川[A-HJ-MQ-Z]|贵[A-HJ]|云[AC-HJ-NP-SV]|藏[A-HJ]|陕[A-HJKVU]|甘[A-HJ-NP]|青[A-H]|宁[A-E]|新[A-HJ-NP-S])([0-9A-HJ-NP-Z]{4}[0-9A-HJ-NP-Z挂试]|[0-9]{4}学|[A-D0-9][0-9]{3}警|[DF][0-9A-HJ-NP-Z][0-9]{4}|[0-9]{5}[DF])$|^WJ[京沪津渝冀晋蒙辽吉黑苏浙皖闽赣鲁豫鄂湘粤桂琼川贵云藏陕甘青宁新]?[0-9]{4}[0-9JBXTHSD]$|^(V[A-GKMORTV]|K[A-HJ-NORUZ]|H[A-GLOR]|[BCGJLNS][A-DKMNORVY]|G[JS])[0-9]{5}$|^[0-9]{6}使$|^([沪粤川渝辽云桂鄂湘陕藏黑]A|闽D|鲁B|蒙[AEH])[0-9]{4}领$|^粤Z[0-9A-HJ-NP-Z][0-9]{3}[港澳]$/i,
            loading: false,
            finished: false,
            is_login: false,
            new_form: {
                company_name: '',
                destination: '',
                date: this.formatDateTime(new Date()),
                transfor_company: '',
                stuff_name: '',
                price: '',
            },
            new_vichele: [{
                main_vichele_number: '',
                behind_vichele_number: '',
                count: '',
                comment: '',
                stuff_name: '',
                repeated: false,
                driver_phone: '',
                driver_id: '',
                driver_name: '',
            }],
            created_apply: [],
        };
    },
    watch: {
        loading: function (_loading) {
            var vue_this = this;
            if (!_loading && vue_this.$route.query.pos) {
                this.$nextTick(() => {
                    vue_this.$refs.single_record[vue_this.$route.query.pos].scrollIntoView();
                });

            }
        },
    },
    methods: {
        add_single_vichele: function(_vichele) {
            this.new_vichele.push(_vichele);
            this.add_single_vichele_show = false;
        },
        remove_vichele_team: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "del_vichele_team", [vue_this.$cookies.get('silent_id'), vue_this.current_vichele_team.id]).then(function (resp) {
                if (resp) {
                    vue_this.init_self_vichele_team();
                    vue_this.current_vichele_team.id = 0;
                }
            });
        },
        finish_team_brief: function () {
            this.show_select_vichele_team = true;
            this.team_brief_show = false;
        },
        remove_vichele: function (_index) {
            var vue_this = this;
            vue_this.new_vichele.splice(_index, 1);
            vue_this.$refs.new_vichele.forEach(element => {
                element.style["border-color"] = 'blue';
            });
        },
        select_team: function (_team) {
            var vue_this = this;
            _team.members.forEach((element) => {
                if (!vue_this.new_vichele.find((value) => {
                        if (value.main_vichele_number == element.main_vichele_number) {
                            return true;
                        } else {
                            return false;
                        }
                    })) {
                    vue_this.new_vichele.push({
                        main_vichele_number: element.main_vichele_number,
                        behind_vichele_number: element.behind_vichele_number,
                        count: parseFloat(vue_this.team_brief_count),
                        comment: '',
                        repeated: vue_this.team_brief_multi,
                        driver_phone: element.driver_phone,
                        driver_id: element.driver_id,
                        driver_name: element.driver_name,
                    });

                }
            });
            vue_this.show_select_vichele_team = false;
        },
        remove_team_member: function (_index) {
            var vue_this = this;
            vue_this.current_vichele_team.members.splice(_index, 1);
            vue_this.update_vichele_team();
        },
        add_team_member: function () {
            var vue_this = this;
            vue_this.current_vichele_team.members.push(vue_this.new_team_member);
            vue_this.update_vichele_team();
            vue_this.add_team_member_show = false;
        },
        update_vichele_team: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "update_vichele_team", [vue_this.$cookies.get('silent_id'), vue_this.current_vichele_team]).finally(function () {
                var old_index = vue_this.current_vichele_team.id;
                vue_this.init_self_vichele_team(() => {
                    vue_this.all_vichele_team.forEach(element => {
                        if (element.id == old_index) {
                            vue_this.current_vichele_team = element;
                        }
                    });
                });
            });
        },
        add_vichele_team: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "create_vichele_team", [vue_this.$cookies.get('silent_id'), {
                name: vue_this.new_vichele_team_name
            }]).then(function (resp) {
                if (resp) {
                    vue_this.init_self_vichele_team(() => {
                        vue_this.current_vichele_team = vue_this.all_vichele_team[vue_this.all_vichele_team.length - 1];
                    });
                    vue_this.add_vichele_team_show = false;
                }
            });
        },
        init_self_vichele_team: function (_after_hook) {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "get_all_vichele_team", [vue_this.$cookies.get('silent_id')]).then(function (resp) {
                vue_this.all_vichele_team = [];
                resp.forEach((element, index) => {
                    vue_this.$set(vue_this.all_vichele_team, index, element);
                });
                if (_after_hook) {
                    _after_hook();
                }
            });
        },
        delete_vichele: function (_item) {
            var vue_this = this;
            Dialog({
                title: '取消进厂申请',
                closeOnClickOverlay: true,
                message: '确定要取消 ' + _item.main_vichele_number + ' ' + _item.behind_vichele_number + ' 吗'
            }).then(function () {
                vue_this.$call_remote_process('vichele_management', 'delete_vichele_info', [vue_this.$cookies.get('silent_id'), _item.id]).then(function (resp) {
                    if (resp) {
                        vue_this.created_apply = [];
                        vue_this.finished = false;
                        vue_this.$refs.all_record.check();
                    }
                });
            });

        },
        onChange: function () {
            var vue_this = this;
            vue_this.vichele_update_info.count = parseFloat(vue_this.vichele_update_info.count);
            vue_this.$call_remote_process("vichele_management", 'update_vichele_info', [vue_this.$cookies.get('silent_id'), vue_this.vichele_update_info]).then(function (resp) {
                if (resp) {
                    vue_this.created_apply = [];
                    vue_this.finished = false;
                    vue_this.$refs.all_record.check();
                    vue_this.change_show = false;
                }
            });
        },
        update_vichele: function (_vichele) {
            this.vichele_update_info = JSON.parse(JSON.stringify(_vichele));
            this.change_show = true;
        },
        formatDateTime: function (date) {
            var y = date.getFullYear();
            var m = date.getMonth() + 1;
            m = m < 10 ? ('0' + m) : m;
            var d = date.getDate();
            d = d < 10 ? ('0' + d) : d;
            return y + '-' + m + '-' + d;
        },
        confirm_time: function (_time) {
            this.new_form.date = this.formatDateTime(_time);
            this.show_time_picker = false;
        },
        confirm_update_time: function (_time) {
            this.vichele_update_info.date = this.formatDateTime(_time);
            this.show_time_update_picker = false;
        },
        convert_bigger: function (_value) {
            return _value.toLocaleUpperCase();
        },
        have_to_have_gua: function (value) {
            var ret = false;
            if (!value) {
                ret = true;
            } else if (this.vichele_number_patten.test(value)) {
                if (value.split('')[value.length - 1] == "挂") {
                    ret = true;
                }
            }
            return ret;
        },
        onLoad: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", 'get_created_vichele_info', [vue_this.$cookies.get('silent_id'), vue_this.created_apply.length]).then(function (resp) {
                resp.forEach(element => {
                    vue_this.created_apply.push(element);
                });
                if (resp.length < 15) {
                    vue_this.finished = true;
                }
                vue_this.loading = false;
            });
        },
        onSubmit: function () {
            var vue_this = this;
            vue_this.update_user_info();
            var extra_vichele = [];
            vue_this.new_vichele.forEach(element => {
                extra_vichele.push({
                    company_name: vue_this.new_form.company_name,
                    destination: vue_this.new_form.destination,
                    date: vue_this.new_form.date,
                    transfor_company: vue_this.new_form.transfor_company,
                    main_vichele_number: element.main_vichele_number,
                    behind_vichele_number: element.behind_vichele_number,
                    count: parseFloat(element.count),
                    comment: element.comment,
                    stuff_name: vue_this.new_form.stuff_name,
                    repeated: element.repeated,
                    driver_phone: element.driver_phone,
                    driver_id: element.driver_id,
                    driver_name: element.driver_name,
                    price: parseFloat(vue_this.new_form.price),
                });
            });
            vue_this.$call_remote_process("vichele_management", 'create_vichele_info', [vue_this.$cookies.get('silent_id'), extra_vichele]).then(function (resp) {
                if (resp) {
                    vue_this.created_apply = [];
                    vue_this.finished = false;
                    vue_this.active = 1;
                    vue_this.$refs.all_record.check();
                }
            }).catch(err => {
                var main_vichele_number = err.msg.split('重复派出')[0];
                var focus_index = vue_this.new_vichele.findIndex(value => {
                    return value.main_vichele_number == main_vichele_number;
                });
                if (focus_index >= 0) {
                    vue_this.$refs.new_vichele[focus_index].style["border-color"] = 'red';
                    vue_this.$refs.new_vichele[focus_index].scrollIntoView();
                }
            });
        },
        login: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", "silent_login", [vue_this.$route.query.code]).then(function (resp) {
                vue_this.$cookies.set('silent_id', resp, '5d');
                window.location.replace("/pa_web/extra_vichele");
            });
        },
        add_vichele: function () {
            this.new_vichele.push({
                main_vichele_number: '',
                behind_vichele_number: '',
                count: "",
                comment: '',
                stuff_name: '',
                repeated: false,
                driver_phone: '',
                driver_id: '',
                driver_name: '',
            });
        },
        fetch_user_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", 'get_silent_user_info', [vue_this.$cookies.get('silent_id')]).then(function (resp) {
                vue_this.user_name = resp.name;
                vue_this.user_phone = resp.phone;
            });
        },
        update_user_info: function () {
            var vue_this = this;
            vue_this.$call_remote_process("vichele_management", 'set_silent_user_info', [vue_this.$cookies.get('silent_id'), {
                name: vue_this.user_name,
                phone: vue_this.user_phone
            }]).then(function () {
                vue_this.fetch_user_info();
            })
        },
    },
    beforeMount: function () {
        var vue_this = this;

        if (this.$route.query.pos) {
            vue_this.active = 1;
        }
        if (vue_this.$route.query.code) {
            vue_this.login();
        } else {
            vue_this.$call_remote_process("vichele_management", 'verify_login', [vue_this.$cookies.get('silent_id')]).then(function (resp) {
                if (resp) {
                    vue_this.is_login = true;
                    vue_this.fetch_user_info();
                } else {
                    window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web%2fextra_vichele&response_type=code&scope=snsapi_base&state=STATE#wechat_redirect"
                }
            });
        }
        vue_this.init_self_vichele_team();
    },

}
</script>

<style scoped>
.vichele_show {
    border: 2px solid blue;
    border-radius: 15px;
    margin-left: 10px;
    margin-right: 10px;
    margin-bottom: 10px;
    margin-top: 10px;
    padding-top: 8px;
}

.opt_button_show {
    margin-left: 5px;
    width: 60px;
}

.one_record_show {
    border: 1px solid green;
    border-radius: 8px;
    margin-bottom: 10px;
    padding-top: 3px;
    padding-bottom: 6px;
}

.highlight_record {
    border: 1px solid red;
}

.team_member_show {
    border: 1px solid rgb(0, 255, 128);
    margin-top: 10px;
    padding-left: 3px;
}
</style>
