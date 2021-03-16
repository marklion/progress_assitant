export function get_client(_service_interface) {
        // eslint-disable-next-line
    var mp1 = new Thrift.Multiplexer();
        // eslint-disable-next-line
    var transport = new Thrift.Transport("https://www.d8sis.cn/pa_web/pa_rpc/");
        // eslint-disable-next-line
    var protocol = new Thrift.Protocol(transport);
        // eslint-disable-next-line
    var client = mp1.createClient(_service_interface, eval(_service_interface + 'Client'), transport);
    return client;
}
