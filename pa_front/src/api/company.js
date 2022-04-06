import {
    call_remote_process, call_remote_process_no_toast
} from '@/plugins/rpc_helper.js'

/**
 * @param {String} company_name
 * @return {Promise<Boolean : need_driver_license>}
 */
export function getCompanyConfig(company_name){
    return call_remote_process("company_management", "get_customize",
        [company_name]);
}

/**
 * 获取所有签约公司By ssid
 * @param {String} ssid
 * @param {Boolean} toast
 */
export function getAllContract(ssid, toast = true){
    let call = call_remote_process
    if(!toast){
        call = call_remote_process_no_toast
    }
    return call("company_management", "get_all_contract", [ssid])
}
