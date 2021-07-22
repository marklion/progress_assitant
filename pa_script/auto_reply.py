#!/usr/bin/python3

import werobot
import json
from thrift import Thrift
from thrift.protocol.TJSONProtocol import TJSONProtocol
from thrift.protocol.TMultiplexedProtocol import TMultiplexedProtocol
from thrift.transport.THttpClient import THttpClient
from gen_code.idl import stuff_plan_management
from gen_code.idl.ttypes import *
from gen_code.idl.constants import *
import os

robot = werobot.WeRoBot(token='123456')


@robot.text
def plan_search(message):
    transport = THttpClient('http://localhost:8123/pa_rpc')
    protocol = TJSONProtocol(transport)
    transport.open()
    multi_protocol = TMultiplexedProtocol(protocol, "stuff_plan_management")
    client = stuff_plan_management.Client(multi_protocol)
    plan_id = client.search_plan_by_driver_phone(message.content)
    transport.close()
    ret = "当前没有手机号为{}的司机需要承运的计划".format(message.content)
    if len(plan_id) != 0:
        ret = '手机号为{}的司机需要承运的计划：\n'.format(message.content)
        for i in plan_id:
            ret += '<a href="https://www.d8sis.cn/pa_web/plan_detail/{}">计划{}</a>\n'.format(
                i.id, i.number)

    return ret

@robot.subscribe
def pop_mini_program(message):
    return '<a href="http://www.qq.com" data-miniprogram-appid="wxfbf41c757510dc4c" data-miniprogram-path="/pages/index/index">进入掌易</a>'

robot.config['HOST'] = '0.0.0.0'
robot.config['PORT'] = 9123
robot.config["APP_ID"] = "wxa390f8b6f68e9c6d"
robot.config["APP_SECRET"] = os.getenv("WECHAT_SECRET")

client = robot.client
client.create_menu({
    "button": [{
        "type": "view",
        "name": "承运信息",
        "url": "https://www.d8sis.cn/pa_web/driver_register/"
    }, {
        "type": "view",
        "name": "送货进厂",
        "url": "https://www.d8sis.cn/pa_web/extra_vichele/"
    }]
})

robot.run()
