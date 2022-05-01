import {
    compressAccurately
} from 'image-conversion';
import {
    call_remote_process
} from '@/plugins/rpc_helper.js'

function convert2Base64(_file) {
    return new Promise((resolve, reject) => {
        let reader = new FileReader();
        reader.readAsDataURL(_file);
        reader.onloadend = function () {
            let result = this.result;
            let file_content = result.split(';base64,')[1];
            resolve(file_content);
        }
        reader.onerror = event => {
            reject(event)
        }
    })
}

/**
 * @param {File} file
 * @param {String} silent_id
 * @param {String} expire_date
 * @return {Promise<DriverLicenseInfo>}
 */
export async function addDriverLicense(file, silent_id, expire_date) {
    let compressFile = await compressAccurately(file, 400);
    let base64File = await convert2Base64(compressFile);

    return await call_remote_process("stuff_plan_management", "add_driver_license",
        [silent_id, base64File, expire_date]);
}

/**
 * @param {String} silent_id
 * @return {Promise<Array<DriverLicenseInfo>>}
 */

export function getLicenseBySilentId(silent_id) {
    return call_remote_process("stuff_plan_management", "get_self_all_license_info",
        [silent_id]);
}

/**
 * @param {String} ssid
 * @param {String} phone
 * @return {Promise<Array<DriverLicenseInfo>>}
 */
export function getAllLicenseInfoByDriverPhone(ssid, phone) {
    return call_remote_process("stuff_plan_management", "get_all_license_info_by_driver_phone",
        [ssid, phone]);
}

/**
 * @param {String} silent_id
 * @param {String} ssid
 * @param {DriverLicenseInfo} driver_license_info
 * @return {Promise<Boolean>}
 */
export function updateLicenseExpireDate(silent_id, ssid, driver_license_info) {
    return call_remote_process('stuff_plan_management', 'update_driver_license',
        [silent_id, ssid, driver_license_info]);
}

/**
 *
 * @param {String} silent_id
 * @param {String} license_id
 * @return {Promise<Boolean>}
 */
export function delLicense(silent_id, license_id) {
    return call_remote_process('stuff_plan_management', 'del_driver_license',
        [silent_id, license_id]);
}

export async function addVehicleLicense(file, silent_id, expire_date, plate_no) {
    let compressFile = await compressAccurately(file, 400);
    let base64File = await convert2Base64(compressFile);

    return await call_remote_process("stuff_plan_management", "add_vehicle_license",
        [silent_id, base64File, expire_date, plate_no]);
}

export function delVehicleLicense(silent_id, license_id) {
    return call_remote_process('stuff_plan_management', 'del_vehicle_license',
        [silent_id, license_id]);
}

export function getVehicleLicenseByPlateNo(plate_no) {
    return call_remote_process("stuff_plan_management", "get_license_by_vehicle_number",
        [plate_no]);
}
