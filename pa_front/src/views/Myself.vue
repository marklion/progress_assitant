<template>
<div class="myself_show">
    <van-cell-group>
        <van-cell is-link :to="{name: 'BindCompany'}" value="修改信息">
            <template #title>
                <div>
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
        </van-cell>
    </van-cell-group>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Cell,
    CellGroup,
    Col,
    Row
} from 'vant';
import {
    Image as VanImage
} from 'vant';

Vue.use(Cell);
Vue.use(CellGroup);
Vue.use(Col);
Vue.use(Row);
Vue.use(VanImage);
export default {
    name: 'Myself',
}
</script>

<style>

</style>
