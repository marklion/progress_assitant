import {
    compressAccurately
} from 'image-conversion';
import {
    call_remote_process
} from '@/plugins/rpc_helper.js'
// import _ from 'lodash'

function convert2Base64(_file){
    return new Promise((resolve, reject) => {
        let reader = new FileReader();
        reader.readAsDataURL(_file);
        reader.onloadend = function () {
            let result = this.result;
            let file_content = result.split(';base64,')[1];
            resolve(file_content);
        }
        reader.onerror = event => {reject(event)}
    })
}

/**
 *
 * @param {File} file
 * @param {String} silent_id
 * @param {String} expire_date
 * @return {Promise<DriverLicenseInfo>}
 */
export async function addDriverLicense(file, silent_id, expire_date){
    let compressFile = await compressAccurately(file, 400);
    let base64File = await convert2Base64(compressFile);
    let driverLicenseInfo = await call_remote_process("stuff_plan_management", "add_driver_license",
        [silent_id, base64File, expire_date]);
    return driverLicenseInfo;
}
