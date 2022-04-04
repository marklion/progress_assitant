<template>
<div class="bidding-ctn" v-if="biddingInfo">
    <BiddingStat :biddingInfo="biddingInfo"></BiddingStat>

    <van-collapse v-model="activeNames" class="bidding-turns-ctn">
        <van-collapse-item v-for="turn in biddingInfo.all_status" v-bind:key="turn.bidding_turn"
                           :name="turn.bidding_turn">
            <template #title>
                <span class="custom-title">{{turn.bidding_turn + ' 轮竞价'}}</span>
                <BiddingStatus :status="turn.status"></BiddingStatus>
            </template>

            <van-cell
                v-for="customers in turn.all_customers_price" v-bind:key="customers.company_name"
                :value="customers.price"
                :label="customers.timestamp"
            >
                <template #title>
                    <span>{{customers.company_name}}</span>
                    <van-icon v-if="customers.company_name == turn.cur_top_customer"
                              color="#ee0a24"
                              size="20"
                              name="award" />
                </template>
            </van-cell>
        </van-collapse-item>
    </van-collapse>
</div>
</template>

<script>
import {getBiddingById} from '@/api/stuff'
import BiddingStatus from "@/views/bidding/components/BiddingStatus";
import BiddingStat from "@/views/bidding/components/BiddingStat";


export default {
    name: "BiddingInfo",
    components : {
        BiddingStat,
        BiddingStatus
    },
    data(){
        return {
            ssid : '',
            biddingInfo : null,
            activeNames : [],
        }
    },
    async mounted(){
        this.ssid = this.$cookies.get('pa_ssid')
        this.biddingId = this.$route.params.id
        this.biddingInfo = await getBiddingById(this.ssid, this.biddingId)
        let defaultOpenTurn = this.biddingInfo.all_status.find(i => i.status === 1)
        this.activeNames.push(defaultOpenTurn && defaultOpenTurn.bidding_turn)
    },

    methods: {

    }
}
</script>

<style scoped>
.bidding-ctn{
    padding-top: 10px;
    background-color: #f7f8fa;
}
.bidding-turns-ctn{
    margin-top: 10px;
}
.bidding-winner{
    color: #ee0a24;
}
</style>
