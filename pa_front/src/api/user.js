import {
    call_remote_process
} from '@/plugins/rpc_helper.js'

/**
 * 创建竞价
 * @param {String} ssid
 * @return {Promise<UserInfo>}
 */
export function getUserInfo(ssid){
    return call_remote_process("user_management", "get_user_info",
        [ssid]);
}
