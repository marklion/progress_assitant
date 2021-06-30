var cli_args = require('commander');
const thrift = require('thrift');
var sha256 = require('js-sha256');
const pa_rpc_open_api_management = require('./gen_code/open_api_management')
'use strict';

function request_rpc(service_name, process_name, params) {
    var mp = new thrift.Multiplexer();
    var options = {
        transport: thrift.TBufferedTransport,
        protocol: thrift.TJSONProtocol,
        path: "/pa_web/pa_rpc/",
        headers: { "Connection": "close" },
        https: cli.opts().hostname == 'localhost'?false:true,
    };
    var connection = thrift.createHttpConnection(cli.opts().hostname, parseInt(cli.opts().port), options);
    var client = mp.createClient(service_name, eval("pa_rpc_" + service_name), connection);
    return new Promise(function (resolve, reject) {
        client[process_name].apply(client, params).then(function (resp) {
            resolve(resp);
        }).catch(function (err) {
            reject(err);
        });
    });
}
var cli = cli_args
    .usage('[options] <command> [args]')
    .version('v1.0')
    .showHelpAfterError()
    .addOption(new cli_args.Option('-H, --hostname <hostname>', 'specify remote hostname').hideHelp().default('www.d8sis.cn'))
    .addOption(new cli_args.Option('-P, --port <port>', 'specify remote port').hideHelp().default('443'));

cli.command('register')
    .requiredOption('-c, --company <company>', 'specify the company name')
    .requiredOption('-e, --email <email>', 'specify the email')
    .requiredOption('-p, --password <password>', 'specify the password')
    .action(async (sub_args, cmd) => {
        try {
            var sha_password = sha256(sub_args.password);
            var result = await request_rpc("open_api_management", 'register_api_user', [sub_args.company, sub_args.email, sha_password]);
            if (result) {
                console.log('verify code was sent to your email address, please check it and use command "verify -e <email> -C <code>" to finish register');
            }
        } catch (error) {
            console.log(error.msg);
        }
    });

cli.command('verify')
    .requiredOption('-e, --email <email>', 'specify the email')
    .requiredOption('-C, --code <code>', 'specify the code')
    .action(async (sub_args) => {
        try {
            var result = await request_rpc("open_api_management", 'verify_email_code', [sub_args.email, sub_args.code]);
            if (result) {
                console.log('success');
            }
            else {
                console.log('failure');
            }
        } catch (error) {
            console.log(error.msg);
        }
    });

cli.command('unregister')
    .requiredOption('-e, --email <email>', 'specify the email')
    .requiredOption('-p, --password <password>', 'specify the password')
    .action(async (sub_args) => {
        var sha_password = sha256(sub_args.password);
        try {
            var result = await request_rpc("open_api_management", 'unregister_api_user', [sub_args.email, sha_password]);
            if (result) {
                console.log('success');
            }
            else {
                console.log('failure');
            }
        } catch (error) {
            console.log(error.msg);
        }
    });

cli.command('token')
    .requiredOption('-e, --email <email>', 'specify the email')
    .requiredOption('-p, --password <password>', 'specify the password')
    .action(async (sub_args)=>{
        var sha_password = sha256(sub_args.password);
        try {
            var result = await request_rpc('open_api_management', 'get_token', [sub_args.email, sha_password]);
            console.log('String in below line is the api token, please remember it. It will change once the token command issued');
            console.log(result);
        } catch (error) {
            console.log(error.msg);
        }
    });

cli_args.parse(process.argv);
