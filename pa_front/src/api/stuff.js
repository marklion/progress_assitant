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
    console.log(bidding)
    return call_remote_process("stuff_info", "create_bidding",
        [ssid, bidding]);
}
