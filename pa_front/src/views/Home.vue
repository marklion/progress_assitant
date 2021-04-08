<template>
<div class="home">
    <van-dropdown-menu>
        <van-dropdown-item v-model="company_filter" :options="company_option" @change="select_company" />
        <van-dropdown-item v-model="stuff_filter" :options="stuff_option" />
    </van-dropdown-menu>
    <van-card v-for="(single_stuff, index) in stuff_need_show" :key="index" :price="single_stuff.price" :desc="single_stuff.company" :title="single_stuff.name">
        <template #tags>
            <van-tag plain type="danger">{{single_stuff.last}}</van-tag>
        </template>
        <template #num>
            <van-button round size="small" icon="plus" type="primary" @click="nav_to_plan(single_stuff.type_id)">报计划</van-button>
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
            company_filter: 0,
            stuff_filter: 0,
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
            return ret;
        },
    },
    beforeMount() {
        var vue_this = this;
        this.$get_client("stuff_info").get_today().then(function (resp) {
            resp.forEach((element, index) => {
                vue_this.$set(vue_this.today_stuff, index, element)
            });
            vue_this.orgnize_company();
            vue_this.orgnize_stuff(vue_this.company_option.find((item) => {
                return item.value == vue_this.company_filter
            }).text);
        }).catch(function (err) {
            console.log(err);
            vue_this.$toast(err.msg);
        });
    },
    methods: {
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
        nav_to_plan: function (_type_id) {
            if (this.$store.state.is_login) {
                this.$router.push({
                    name: 'StuffPlan',
                    params: {
                        type_id: _type_id
                    }
                });
            } else {
                window.location.href = "https://open.weixin.qq.com/connect/oauth2/authorize?appid=wxa390f8b6f68e9c6d&redirect_uri=https%3a%2f%2fwww.d8sis.cn%2fpa_web%2flogin_mp&response_type=code&scope=snsapi_userinfo&state=STATE#wechat_redirect"
            }
        }
    },
}
</script>
