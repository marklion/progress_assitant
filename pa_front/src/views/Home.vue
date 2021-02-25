<template>
<div class="home">
    <h1>{{company_name()}}</h1>
    <van-row align="center" type="flex">
        <van-col>
            <van-image round width="60px" height="60px" :src="$store.state.userinfo.logo" />
        </van-col>
        <van-col>
            <div class="name_show">{{$store.state.userinfo.name}}</div>
            <div class="role_show">{{$store.state.userinfo.role}}</div>
        </van-col>
    </van-row>
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
    Col,
    Row
} from 'vant';

Vue.use(Col);
Vue.use(Row);

Vue.use(VanImage);
Vue.use(Button);
export default {
    name: 'Home',
    data: function () {
        return {
            company_name() {
                var ret = "未绑定公司";
                if (this.$store.state.userinfo.company) {
                    ret = this.$store.state.userinfo.company;
                }

                return ret;
            }
        };
    },
    watch: {
        "$store.state.userinfo": function (value) {
            if (!value.company) {
                this.$router.push({
                    name: 'BindCompany',
                    query: {
                        company: this.$route.query.company
                    }
                });
            }
        },
    },
    beforeMount() {
    },
    methods: {},
}
</script>
