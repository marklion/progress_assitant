<template>
<div class="home">
    <div v-if="has_apply">
        <van-notice-bar scrollable text="请等待管理员批准加入公司" />
        <van-row type="flex" justify="center" align="center">
            <van-button round type="info" icon="replay" @click="refresh_page">刷新</van-button>
        </van-row>
    </div>
    <van-dropdown-menu>
        <van-dropdown-item v-model="company_filter" :options="company_option" @change="select_company" />
        <van-dropdown-item v-model="stuff_filter" :options="stuff_option" />
        <van-dropdown-item v-model="follow_filter" :options="follow_option" />
    </van-dropdown-menu>
    <van-card class="stuff_card_show" v-for="(single_stuff, index) in stuff_need_show" :key="index" :price="single_stuff.is_unfollow?'关注后可见价格':single_stuff.price" :desc="single_stuff.company" :title="single_stuff.name">
        <template #tags v-if="single_stuff.last">
            <van-tag plain type="danger">{{single_stuff.last}}</van-tag>
        </template>
        <template #num v-if="!single_stuff.is_unfollow">
            <van-row type="flex" justify="end" :gutter="10">
                <van-col>
                    <van-button round size="small" icon="down" type="warning" @click="nav_to_plan(single_stuff.type_id, false)">导入计划</van-button>
                </van-col>
                <van-col>
                    <van-button round size="small" icon="plus" type="primary" @click="nav_to_plan(single_stuff.type_id, true)">报计划</van-button>
                </van-col>
            </van-row>
        </template>
    </van-card>

</div>
</template>

<script>
import Vue from 'vue';
import {
    Button
} from 'vant';
import {
    Image as VanImage
} from 'vant';

import {
    Card
} from 'vant';
import {
    Tag
} from 'vant';
import {
    DropdownMenu,
    DropdownItem
} from 'vant';
import {
    NoticeBar
} from 'vant';

Vue.use(NoticeBar);
Vue.use(DropdownMenu);
Vue.use(DropdownItem);
Vue.use(Tag);
Vue.use(Card);

Vue.use(VanImage);
Vue.use(Button);
export default {
    name: 'Home',
    components: {},
    data: function () {
        return {
            today_stuff: [],
            company_option: [{
                text: '全部商家',
                value: 0
            }],
            stuff_option: [{
                text: '全部货品',
                value: 0,
            }],
            follow_option: [{
                text: '已关注',
                value: 0,
            }, {
                text: '未关注',
                value: 1,
            }, {
                text: '全部',
                value: 2,
            }],
            company_filter: 0,
            stuff_filter: 0,
            follow_filter: 0,
            has_apply: false,
        };
    },
    computed: {
        stuff_need_show: function () {
            var vue_this = this;
            var ret = [];
            this.today_stuff.forEach((element) => {
                if ((element.company == vue_this.company_option.find((item) => {
                        return item.value == vue_this.company_filter
                    }).text || vue_this.company_filter == 0) &&
                    (element.name == vue_this.stuff_option.find((item) => {
                        return item.value == vue_this.stuff_filter
                    }).text || vue_this.stuff_filter == 0)) {
                    ret.push(element);
                }
            });
            var tmpret = ret;
            ret = [];
            tmpret.forEach(element => {
                if (vue_this.follow_filter == 2) {
                    ret.push(element);
                } else if (vue_this.follow_filter == 1 && element.is_unfollow) {
                    ret.push(element);
                } else if (vue_this.follow_filter == 0 && !element.is_unfollow) {
                    ret.push(element);
                }
            });
            return ret;
        },
    },
    beforeMount() {
        var vue_this = this;
        this.$call_remote_process("stuff_info", 'get_today', [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            resp.forEach((element, index) => {
                vue_this.$set(vue_this.today_stuff, index, element)
            });
            vue_this.$call_remote_process("stuff_info", "get_today_unfollow", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                resp.forEach(element => {
                    var unfollow_stuff = element;
                    unfollow_stuff.is_unfollow = true;
                    vue_this.today_stuff.push(unfollow_stuff);
                });
                vue_this.orgnize_company();
                vue_this.orgnize_stuff(vue_this.company_option.find((item) => {
                    return item.value == vue_this.company_filter
                }).text);
            });
        });
        vue_this.$call_remote_process("user_management", "has_apply", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
            vue_this.has_apply = resp;
        });
    },
    methods: {
        refresh_page: function () {
            window.location.replace("/pa_web/");
        },
        select_company: function () {
            var vue_this = this;
            this.orgnize_stuff(vue_this.company_option.find((item) => {
                return item.value == vue_this.company_filter
            }).text);
        },
        orgnize_company: function () {
            var vue_this = this;
            vue_this.company_option = [{
                text: '全部商家',
                value: 0
            }];
            vue_this.today_stuff.forEach((element) => {
                if (vue_this.company_option.findIndex((item) => {
                        return item.text == element.company
                    }) == -1) {
                    vue_this.company_option.push({
                        text: element.company,
                        value: vue_this.company_option.length,
                    });
                }
            });
        },
        orgnize_stuff: function (_company) {
            var vue_this = this;
            vue_this.stuff_option = [{
                text: '全部货品',
                value: 0,
            }];
            vue_this.today_stuff.forEach((element) => {
                if (element.company == _company || _company == '全部商家') {
                    if (-1 == vue_this.stuff_option.findIndex((item) => {
                            return item.text == element.name;
                        })) {
                        vue_this.stuff_option.push({
                            text: element.name,
                            value: vue_this.stuff_option.length
                        });
                    }
                }
            });
        },
        nav_to_plan: function (_type_id, is_created) {
            var vue_this = this;
            if (this.$store.state.is_login) {
                vue_this.$call_remote_process("company_management", "get_all_attachment", [vue_this.$cookies.get('pa_ssid')]).then(function (resp) {
                    if (resp.length > 0) {
                        if (is_created) {
                            vue_this.$router.push({
                                name: 'StuffPlan',
                                params: {
                                    type_id: _type_id
                                }
                            });
                        } else {
                            vue_this.$router.push({
                                name: 'PlanImport',
                                params: {
                                    type_id: _type_id
                                }
                            });
                        }
                    } else {
                        vue_this.$router.push({
                            name: 'BoundInfo'
                        });
                    }
                });
            } else {
                window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web%2flogin_mp&response_type=code&scope=snsapi_userinfo&state=STATE#wechat_redirect"
            }
        }
    },
}
</script>

<style scoped>
.stuff_card_show /deep/ .van-card__title {
    font-size: 18px;
    line-height: 20px;
    font-weight: bold;
}

.stuff_card_show /deep/ .van-card__desc {
    font-size: 16px;
}
</style>
