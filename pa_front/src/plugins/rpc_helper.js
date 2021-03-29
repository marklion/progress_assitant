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
