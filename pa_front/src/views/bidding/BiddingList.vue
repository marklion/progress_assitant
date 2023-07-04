<template>
<div>
    <van-cell v-for="item in biddingList" :label="'截止：'+getEndTime(item)" :key="item.id" is-link :to="{name : 'BiddingInfo', params : {id : item.id}}">
        <!-- 使用 title 插槽来自定义标题 -->
        <template #title>
            <span class="custom-title">{{ item.stuff_name }}</span>
            <BiddingStatus :status="item.cur_status" :begin_time="item.begin_time"></BiddingStatus>
        </template>
    </van-cell>
</div>
</template>

<script>
import {
    getAllBidding
} from '@/api/stuff'
import BiddingStatus from "@/views/bidding/components/BiddingStatus";

export default {
    name: "BiddingList",
    components: {
        BiddingStatus
    },
    data() {
        return {
            ssid: '',
            biddingList: [],
        }
    },
    async mounted() {
        this.ssid = this.$cookies.get('pa_ssid')
        let list = await getAllBidding(this.ssid)
        this.biddingList = list.sort((a, b) => b.id - a.id)

    },
    methods: {
        getEndTime(bidding) {
            if (bidding.all_status && bidding.all_status.length > 0) {
                let index = bidding.all_status.length - 1
                return bidding.all_status[index].end_time || '未定义';
            } else {
                return '未定义'
            }
        }
    }
}
</script>

<style scoped>

</style>
