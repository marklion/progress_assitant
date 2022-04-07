<template>
<div>
    <div class="bidding-ctn" v-if="biddingInfo">
        <BiddingCard :biddingInfo="biddingInfo" :curTurn="curTurn"></BiddingCard>

        <van-collapse v-if="!user.buyer" v-model="activeNames" class="bidding-turns-ctn">
            <van-collapse-item v-for="turn in biddingInfo.all_status" v-bind:key="turn.bidding_turn"
                               :name="turn.bidding_turn">
                <template #title>
                    <span class="custom-title">{{turn.bidding_turn + ' 轮报价'}}</span>
                    <BiddingStatus :status="turn.status"></BiddingStatus>
                </template>

                <van-cell
                    v-for="customers in turn.all_customers_price" v-bind:key="customers.company_name"
                    :value="customers.price"
                    :label="customers.timestamp"
                >
                    <template #title>
                        <span>{{customers.company_name}}</span>
                        <van-icon v-if="customers.company_name === turn.cur_top_customer"
                                  color="#ee0a24"
                                  size="20"
                                  name="award" />
                    </template>
                </van-cell>
            </van-collapse-item>
            <div style="margin: 10px;">
                <van-button round block type="danger" @click="onCloseBidding" :disabled="biddingInfo.cur_status !== 0">终止竞价</van-button>
            </div>
        </van-collapse>
        <div v-else class="bidding-turns-ctn">
            <van-cell-group inset style="margin-bottom: 10px">
                <van-cell v-for="turn in biddingInfo.all_status" v-bind:key="turn.bidding_turn"
                          :title="turn.bidding_turn + ' 轮报价'"
                          :value="biddingRecord(turn).price"
                          :label="'报价时间：' + biddingRecord(turn).timestamp"
                ></van-cell>
            </van-cell-group>
            <van-form validate-first @submit="onSubmitPrice">
                <van-field v-model="formData.price" name="price" label="报价" placeholder="请填写您本轮报价金额"
                           required :rules="[{ required: true},{validator: validPrice, message: '请在报价区间内进行报价'}]" />
                <p class="price-hint"><van-icon name="info-o" /> 参与竞价将从公司账户扣除 {{biddingInfo.deposit}} 元保证金，竞价单结束后自动退还</p>
                <div style="margin: 16px;">
                    <van-button round block type="info" native-type="submit">提交</van-button>
                </div>
            </van-form>
        </div>
    </div>
    <div v-else>
        <van-empty :image="emptyImage" :description="emptyHint" />
    </div>
</div>
</template>

<script>

import BiddingStatus from "@/views/bidding/components/BiddingStatus";
import BiddingCard from "@/views/bidding/components/BiddingCard";
import {getBiddingById, callBidding, closeBidding} from '@/api/stuff'
import {getUserInfo} from "@/api/user";
import {Dialog} from 'vant'

export default {
    name: "BiddingInfo",
    components : {
        BiddingCard,
        BiddingStatus
    },
    data(){
        return {
            emptyImage : 'network',
            emptyHint : '数据加载中',
            ssid : '',
            user : {
                buyer : false
            },
            biddingInfo : null,
            activeNames : [],

            formData : {
                price : undefined
            }
        }
    },
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
        }
    },
    async mounted(){
        this.ssid = this.$cookies.get('pa_ssid')
        this.user = await getUserInfo(this.ssid)
        this.biddingId = this.$route.params.id
        await this.loadInitData()
    },

    methods: {
        async loadInitData(){
            let biddingInfo = await getBiddingById(this.ssid, this.biddingId)
            if(this.user.buyer && !this.curTurn.all_customers_price.includes(this.user.company)){
                this.emptyImage = 'error'
                this.emptyHint = '抱歉，您不能参与本轮竞价'
                return
            }
            this.biddingInfo = biddingInfo
            let defaultOpenTurn = this.curTurn
            this.activeNames.push(defaultOpenTurn && defaultOpenTurn.bidding_turn)
        },
        biddingRecord(turn){
            return turn.all_customers_price.find(item => item.company_name === this.user.company)
        },
        validPrice(value){
            return value >= this.biddingInfo.min_price && value <= this.biddingInfo.max_price
        },
        async onSubmitPrice(){
            try{
                await Dialog.confirm({
                    title: '提示',
                    message: '出价后不可更改，请慎重报价，确认提交吗？',
                })
                // on confirm
                console.log(this.ssid, this.biddingId, this.formData.price)
                await callBidding(this.ssid, +this.biddingId, +this.formData.price)
                await this.loadInitData()
                this.formData.price = 0
            }catch(err){
                console.log(err)
            }
        },
        async onCloseBidding(){
            await Dialog.confirm({
                title:'提示',
                message: '确定终止本次竞价吗'
            })
            await closeBidding(this.ssid, +this.biddingId)
            await this.loadInitData()
        }
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
.price-hint{
    padding: 0 10px;
    color: #b6b6b9;
    font-size: 14px;
    line-height: 20px;
}
</style>
