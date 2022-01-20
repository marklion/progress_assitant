<template>
<div class="ticket_item_show">
    <van-row v-if="item_value">
        <van-col :span="6">{{item_name}}:</van-col>
        <van-col :span="18">{{item_value}}</van-col>
    </van-row>
</div>
</template>

<script>
import Vue from 'vue';
import {
    Col,
    Row
} from 'vant';

Vue.use(Col);
Vue.use(Row);
export default {
    name: 'TicketItem',
    props: {
        item_name: String,
        item_value: String,
    },
}
</script>

<style scoped>
.ticket_item_show {
    font-size: 18px;
}
</style>
