import {
    call_remote_process
} from '@/plugins/rpc_helper.js'

/**
 * @param {String} company_name
 * @return {Promise<Boolean : need_driver_license>}
 */
export async function getPlanInfo(company_name){
    return call_remote_process("company_management", "get_customize",
        [company_name]);
}
