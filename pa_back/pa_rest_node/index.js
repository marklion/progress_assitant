const express = require('express');
const app = express();
const port = 9098;
const thrift = require('thrift');
const pa_rpc_open_api_management= require('./gen_code/open_api_management')
'use strict';

const fs = require('fs');

app.use(express.json()) // for parsing application/json
app.use(express.urlencoded({ extended: true })) // for parsing application/x-www-form-urlencoded
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

app.get('/pa_rest/push_arrange/:id', async (req, res) => {
    var id = req.params.id;
    var token = req.query.token;
    var order = req.query.order;
    var location = req.query.location?req.query.location:'';
    var ret = { err_msg: '无权限' };
    var real_id = parseInt(id);
    var is_sale = id[id.length - 1] == 'S' ? true : false;
    try {
        var resp = await request_rpc("open_api_management", 'push_arrange', [real_id, order, is_sale, location, token]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }
    res.send(ret);
});

app.post('/pa_rest/call_vehicle', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };

    try {
        var resp = await request_rpc("open_api_management", 'proc_call_vehicle', [req.body, token]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.post('/pa_rest/vehicle_info', async (req, res) => {
    var token = req.query.token;
    var ret = { err_msg: '无权限' };

    var plateNo = req.body.plateNo;
    var driverId = req.body.driverId?req.body.driverId:"";
    try {
        var resp = await request_rpc("open_api_management", 'proc_vehicle_info', [plateNo, driverId, token]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.get('/pa_rest/all_vehicle_info', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'proc_all_vehicle_info', [token]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.post('/pa_rest/push_weight', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'proc_push_weight', [req.body, token]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.post('/pa_rest/push_blacklist', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        if (req.body.state == 0)
        {
            var resp = await request_rpc("open_api_management", 'proc_add_black_list', [req.body.type, req.body.target, req.body.reason, req.body.expireDate, token]);
            if (resp) {
                ret.err_msg = "";
            }
        }
        else
        {
            var resp = await request_rpc("open_api_management", 'proc_del_black_list', [req.body.type, req.body.target, token]);
            if (resp) {
                ret.err_msg = "";
            }
        }

    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.post('/pa_rest/push_base', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var add_flag = true;
        if (req.body.state == 1)
        {
            add_flag = false;
        }
        delete req.body.state;
        if (add_flag)
        {
            var resp = await request_rpc("open_api_management", 'proc_add_base_info', [req.body, token]);
            if (resp) {
                ret.err_msg = "";
            }
        }
        else
        {
            var resp = await request_rpc("open_api_management", 'proc_del_base_info', [req.body, token]);
            if (resp) {
                ret.err_msg = "";
            }
        }

    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.listen(port, () => {
    console.log('rest is runing');
});