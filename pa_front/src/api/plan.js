import {
    call_remote_process
} from '@/plugins/rpc_helper.js'

/**
 * @param {String} plan_id
 * @return {Promise<PlanDetail>}
 */
export async function getPlanInfo(plan_id){
    return await call_remote_process("stuff_plan_management", "get_plan",
        [plan_id]);
}
