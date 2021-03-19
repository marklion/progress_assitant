<template>
<div class="home">
    <div class="today_price_show">
        今日报价
    </div>
    <van-card v-for="(single_stuff, index) in today_stuff" :key="index" :price="single_stuff.price" :desc="single_stuff.company" :title="single_stuff.name">
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
        };
    },
    beforeMount() {
        var vue_this = this;
        this.$get_client("stuff_info").get_today().then(function (resp) {
            resp.forEach((element, index) => {
                vue_this.$set(vue_this.today_stuff, index, element)
            });
        }).catch(function (err) {
            console.log(err);
        });
    },
    methods: {
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
