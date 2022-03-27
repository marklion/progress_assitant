import {
    Toast
} from 'vant'
export function get_client(_service_interface) {
        // eslint-disable-next-line
    var mp1 = new Thrift.Multiplexer();
        // eslint-disable-next-line
    var transport = new Thrift.Transport(process.env.VUE_APP_BACK_END_URL + "/pa_rpc/");
        // eslint-disable-next-line
    var protocol = new Thrift.Protocol(transport);
        // eslint-disable-next-line
    var client = mp1.createClient(_service_interface, eval(_service_interface + 'Client'), transport);
    return client;
}

export function call_remote_process(_service_interface,process,args) {
    var client = get_client(_service_interface);
    Toast.allowMultiple();
    const toast1 = Toast.loading({
        forbidClick: true,
        duration: 0
    });
    return new Promise(function (resolve, reject) {
        if(!client[process]){
            let errMsg = `${process} not exist`;
            toast1.clear()
            Toast(errMsg);
            reject(errMsg);
        }
        client[process].apply(client, args).then(function (resp) {
            toast1.clear()
            resolve(resp);
        }).catch(function (err) {
            toast1.clear()
            Toast(err.msg);
            reject(err);
        });
    });

}
export function call_remote_process_no_toast(_service_interface,process,args) {
    var client = get_client(_service_interface);
    return new Promise(function (resolve, reject) {
        client[process].apply(client, args).then(function (resp) {
            resolve(resp);
        }).catch(function (err) {
            console.log(err);
            reject(err);
        });
    });

}
