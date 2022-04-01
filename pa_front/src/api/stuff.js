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
 * @param ssid
 * @param status_condition
 * @return {Promise<bidding_params>}
 */
export function getAllBidding(ssid, status_condition = -1){
    return call_remote_process("stuff_info", "get_all_bidding",
        [ssid, status_condition]);
}
