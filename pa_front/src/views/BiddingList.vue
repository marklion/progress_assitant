<template>
<div>
    <van-cell v-for="item in biddingList" :label="'截止：'+item.end_time" :key="item.id" is-link>
        <!-- 使用 title 插槽来自定义标题 -->
        <template #title>
            <span class="custom-title">{{ item.stuff_name }}</span>
            <van-tag :color="statusMap[item.cur_status].color">{{statusMap[item.cur_status].name}}</van-tag>
        </template>
    </van-cell>
</div>
</template>

<script>
import {getAllBidding} from '@/api/stuff'

export default {
    name: "BiddingList",
    data(){
        return {
            ssid : '',
            biddingList : [],
            statusMap : [{
                name : '竞价中',
                color : '#07c160'
            },{
                name : '已结束',
                color : '#666666'
            },{
                name : '异常',
                color : '#ee0a24'
            }]
        }
    },
    async mounted(){
        this.ssid = this.$cookies.get('pa_ssid')
        this.biddingList = await getAllBidding(this.ssid)
        console.log(this.biddingList)
    },
    methods: {

    }
}
</script>

<style scoped>
.custom-title{
    padding: 0 10px 0 0;
}
</style>
