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
    name: "BiddingStat",
    components : {
        BiddingStatus
    },
    props:['biddingInfo'],
    computed: {
        /**
         *  return {
         *   bidding_turn, status, all_customers_price : [], cur_top_customer, cur_top_price, end_time
         *  }
         */
        curTurn(){
            switch(this.biddingInfo.cur_status){
                case 0: {
                    return this.biddingInfo.all_status.find(bs => bs.status === 0)
                }
                case 1: {
                    return this.biddingInfo.all_status[this.biddingInfo.all_status.length - 1]
                }
                case 2: {
                    return this.biddingInfo.all_status.find(bs => bs.status === 2)
                }
                default : {
                    return undefined
                }
            }
        },
        biddingRange(){
            return this.biddingInfo.min_price + ' ~ ' + this.biddingInfo.max_price
        },
        restTime(){
            // this.curTurn.end_time
            return moment(this.curTurn.end_time) - moment();
        }
    },
}
</script>

<style scoped>

</style>
