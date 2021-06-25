const express = require('express');
const app = express();
const port = 9098;
const thrift = require('thrift');
const pa_rpc_user_management = require('./gen_code/user_management')
const pa_rpc_stuff_info = require('./gen_code/stuff_info')
const pa_rpc_stuff_plan_management = require('./gen_code/stuff_plan_management')
const pa_rpc_company_management = require('./gen_code/company_management')
'use strict';

const fs = require('fs');

function has_permission(_token, _company) {
    let rawdata = fs.readFileSync('/conf/data_config.json');
    let data_config = JSON.parse(rawdata);
    var company_config = data_config.find((item) => {
        return item.name == _company
    });
    var ret = false;
    if (company_config)
    {
        ret = _token == company_config.api_token;
    }

    return ret;
}



function request_rpc(service_name, process_name, params) {
    var mp = new thrift.Multiplexer();
    var options = {
        transport: thrift.TBufferedTransport,
        protocol: thrift.TJSONProtocol,
        path: "/pa_rpc",
        headers: { "Connection": "close" },
        https: false
    };
    var connection = thrift.createHttpConnection("localhost", 8123, options);
    var client = mp.createClient(service_name, eval("pa_rpc_" + service_name), connection);
    return new Promise(function (resolve, reject) {
        client[process_name].apply(client, params).then(function (resp) {
            resolve(resp);
        }).catch(function (err) {
            reject(err);
        });
    });
}

app.get('/pa_rest/today_transformation', async (req, res) => {
    var company_name = req.query.company_name;
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    if (has_permission(token, company_name)) {
        try {
            var transformation = await request_rpc("stuff_plan_management", "get_today_transformation", [company_name]);
            ret.err_msg = "";
            ret.transformation = [];
            transformation.forEach(element => {
                ret.transformation.push({
                    mv: element.vichele.main_vichele,
                    bv: element.vichele.behind_vichele,
                    dp: element.vichele.driver_phone,
                    dn: element.vichele.driver_name,
                    company_name: element.vichele.company,
                    stuff_name: element.stuff_name,
                });
            });
        } catch (error) {
            ret = { err_msg: error.msg };
        }
    }
    res.send(ret);
});

app.listen(port, () => {
    console.log('rest is runing');
});