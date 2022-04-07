<template>
    <van-tag :color="statusColor" class="status-tag">{{statusName}}</van-tag>
</template>

<script>
export default {
    name: "BiddingStatus",
    props: ['status'],
    data(){
        return {
            statusMap : [{
                name : '竞价中',
                color : '#07c160'
            },{
                name : '已结束',
                color : '#666666'
            },{
                name : '取消',
                color : '#ee0a24'
            }]
        }
    },
    computed: {
        statusColor(){
            return this.statusMap[this.status].color
        },
        statusName(){
            return this.statusMap[this.status].name
            // let name = this.statusMap[this.biddingInfo.cur_status].name
            // if(this.biddingInfo.cur_status === 0){
            //     name = this.curTurn.bidding_turn + ' 轮' + name
            // }
            // return name
        },
    }
}
</script>

<style scoped>
.status-tag{
    margin-left: 10px;
}
</style>
