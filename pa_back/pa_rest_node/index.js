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

app.post('/pa_rest/push_balance', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'proc_push_balance', [req.body, token]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.post('/pa_rest/push_zone_change', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'proc_push_zone_change', [req.body, token]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.post('/pa_rest/push_manual_permit', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'proc_push_manual_permit', [req.body, token]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});
app.get('/pa_rest/get_all_balance', async (req, res) => {
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'get_all_customer_balance', [token]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});
app.post('/pa_rest/export_balance_history', async (req, res) => {
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'export_balance_audit_log', [token, req.body.company_name]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});
app.get('/pa_rest/company_brief', async (req, res) => {
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'get_today_brief_info', [token]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});
app.post('/pa_rest/undo_vehicle', async (req, res) => {
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'undo_vehicle_weight', [token, req.body.id, req.body.auth_code]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});
app.post('/pa_rest/push_p', async (req, res) => {
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'record_p_weight', [token, req.body.id]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});

app.post('/pa_rest/create_plan', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'proc_create_plan', [token, req.body]);
        if (resp) {
            ret.err_msg = "";
            ret.result = { orderNumber: resp };
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);

});
app.post('/pa_rest/cancel_plan', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };
    try {
        var resp = await request_rpc("open_api_management", 'proc_cancel_plan', [token, req.body.orderNumber]);
        if (resp) {
            ret.err_msg = "";
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);

});
app.get('/pa_rest/dc_api', async (req, res) => {
    var token = req.query.verify;

    res.send(token);

});
app.post('/pa_rest/dc_api', async (req, res) => {
    console.log(req.body);
    try {
        await request_rpc("open_api_management", "set_dc_status", [req.body.mac, req.body.status]);
    } catch (error) {
        console.log(error)
    }
    res.send();

});
app.get('/pa_rest/vehicle_detail', async (req, res)=>{
    var token = req.query.token;
    var ret = { err_msg: '无权限' };

    try {
        var resp = await request_rpc("open_api_management", 'get_detail_record_by_vehicle_number', [req.query.plate_no, token]);
        if (resp) {
            ret.err_msg = "";
            ret.result = resp;
        }
    } catch (error) {
        ret = { err_msg: error.msg };
    }

    res.send(ret);
});
app.listen(port, () => {
    console.log('rest is runing');
});