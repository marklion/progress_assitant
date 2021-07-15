const express = require('express');
const app = express();
const port = 9098;
const thrift = require('thrift');
const pa_rpc_open_api_management= require('./gen_code/open_api_management')
'use strict';

const fs = require('fs');


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
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var transformation = await request_rpc("open_api_management", "get_today_transformation", [token]);
        ret.err_msg = "";
        ret.transformation = [];
        transformation.forEach(element => {
            var id_string = parseInt(element.id);
            ret.transformation.push({
                id: id_string + (element.is_sale?'S':'B'),
                mv: element.main_vichele_number,
                bv: element.behind_vichele_number,
                stuff_name: element.stuff_name,
                enter_count:element.enter_count,
                company_name: element.company_name,
                extra_info: {
                    driver_name:element.extra_info.driver_name,
                    driver_phone:element.extra_info.driver_phone,
                    driver_id:element.extra_info.driver_id,
                },
                is_sale: element.is_sale,
            });
        });
    } catch (error) {
        ret = { err_msg: error.msg };
    }
    res.send(ret);
});

app.get('/pa_rest/push_count/:id', async (req, res) => {
    var id = req.params.id;
    var token = req.query.token;
    var count = parseFloat(req.query.count);
    var ret = { err_msg: '无权限' };
    var real_id = parseInt(id);
    var is_sale = id[id.length - 1] == 'S' ? true : false;
    try {
        var resp = await request_rpc("open_api_management", 'push_exit_count', [real_id, count, is_sale, token]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }
    res.send(ret);
});

app.listen(port, () => {
    console.log('rest is runing');
});