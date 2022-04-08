<template>
    <van-cell-group inset>
        <van-cell :value="'总量：' + biddingInfo.total_count">
            <template #title>
                <span class="custom-title">{{ biddingInfo.stuff_name }}</span>
                <BiddingStatus :status="biddingInfo.cur_status"></BiddingStatus>
            </template>
        </van-cell>
        <van-cell title="竞价区间" :value="biddingRange" :label="'保证金: ' + biddingInfo.deposit + '元'"/>
        <van-cell title="距离截止还剩" :label="curTurn.bidding_turn + ' 轮竞价'">
            <template #default>
                <van-count-down :time="restTime" />
            </template>
        </van-cell>
    </van-cell-group>
</template>

<script>
import BiddingStatus from "@/views/bidding/components/BiddingStatus";
import moment from "moment";

export default {
    name: "BiddingCard",
    components : {
        BiddingStatus
    },
    props:['biddingInfo', 'curTurn'],
    computed: {
        biddingRange(){
            return this.biddingInfo.min_price + ' ~ ' + this.biddingInfo.max_price
        },
        restTime(){
            if(this.biddingInfo.cur_status === 0 ){
                return moment(this.curTurn.end_time) - moment();
            }else{
                return 0
            }

        }
    },
}
</script>

<style scoped>

</style>
