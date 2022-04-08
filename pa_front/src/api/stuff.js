import {
    call_remote_process
} from '@/plugins/rpc_helper.js'

/**
 * 创建竞价
 * @param {String} ssid
 * @param {bidding_params} bidding
 * @return {Promise<Boolean>}
 */
export function createBidding(ssid, bidding){
    return call_remote_process("stuff_info", "create_bidding",
        [ssid, bidding]);
}

/**
 * 获取所有竞价单
 * @param {String} ssid
 * @param {Number} status_condition
 * @return {Promise<Array[{bidding_params}]>}
 */
export function getAllBidding(ssid, status_condition = -1){
    return call_remote_process("stuff_info", "get_all_bidding",
        [ssid, status_condition]);
}

/**
 * 获取一个竞价的详情
 * @param ssid
 * @param biddingId
 * @return {Promise<bidding_params>}
 */
export async function getBiddingById(ssid, biddingId){
    return call_remote_process("stuff_info", "get_bidding",
        [ssid, +biddingId]);
}

/**
 * 商家报价
 * @param ssid
 * @param biddingId
 * @param price
 * @return {Promise<Boolean>}
 */
export async function callBidding(ssid, biddingId, price){
    return call_remote_process("stuff_info", "call_bidding",
        [ssid, biddingId, price]);
}

/**
 * 异常终止竞价
 * @param ssid
 * @param biddingId
 * @return {Promise<Boolean>}
 */
export async function closeBidding(ssid, biddingId){
    return call_remote_process("stuff_info", "close_bidding",
        [ssid, biddingId]);
}
