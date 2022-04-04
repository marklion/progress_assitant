<template>
<div>
    <van-cell v-for="item in biddingList" :label="'截止：'+item.end_time" :key="item.id"
              is-link :to="{name : 'BiddingInfo', params : {id : item.id}}">
        <!-- 使用 title 插槽来自定义标题 -->
        <template #title>
            <span class="custom-title">{{ item.stuff_name }}</span>
            <BiddingStatus :status="item.cur_status"></BiddingStatus>
        </template>
    </van-cell>
</div>
</template>

<script>
import {getAllBidding} from '@/api/stuff'
import BiddingStatus from "@/views/bidding/components/BiddingStatus";

export default {
    name: "BiddingList",
    components : {
        BiddingStatus
    },
    data(){
        return {
            ssid : '',
            biddingList : [],
        }
    },
    async mounted(){
        this.ssid = this.$cookies.get('pa_ssid')
        this.biddingList = await getAllBidding(this.ssid)
        // console.log(this.biddingList)
    },
    methods: {

    }
}
</script>

<style scoped>

</style>
