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

trans_mini_path = '/pages/index/index?enter_url=/driver_register/'
extra_vehicle_path = '/pages/index/index?enter_url=/extra_vichele/'

@robot.text
def plan_search(message):
    ret = "查看历史消息获取掌易助理操作文档。\n"
    ret += "点击" + '<a href="http://www.qq.com" data-miniprogram-appid="wxfbf41c757510dc4c" data-miniprogram-path="/pages/index/index">进入掌易</a>可以报计划或审批计划\n'
    ret += '司机点击<a href="http://www.qq.com" data-miniprogram-appid="wxfbf41c757510dc4c" data-miniprogram-path="' + trans_mini_path + '">承运信息</a>可以排号或上传磅单\n'
    ret += '供应商点击<a href="http://www.qq.com" data-miniprogram-appid="wxfbf41c757510dc4c" data-miniprogram-path="' + extra_vehicle_path + '">送货进厂</a>可以申请车辆送货进厂\n'

    return ret

@robot.key_click("work_ticket")
def work_ticket_open():
    return "复制链接到电脑浏览器即可提交工单：https://kegj20221117013909045.pingcode.com/ship/PRD_ZYZL/portal/1"

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
        "type": "miniprogram",
        "name": "承运信息",
        "appid":"wxfbf41c757510dc4c",
        "url": "https://www.d8sis.cn/pa_web/driver_register/",
        "pagepath": trans_mini_path
    }, {
        "type": "miniprogram",
        "name": "送货进厂",
        "url": "https://www.d8sis.cn/pa_web/extra_vichele/",
        "appid": "wxfbf41c757510dc4c",
        "pagepath": extra_vehicle_path
    },{
        "name":"联系我们",
        "sub_button":[
            {
                "type":"view",
                "name":"在线客服",
                "url":"https://work.weixin.qq.com/kfid/kfc2be35e14aff69f5f"
            },{
                "type":"click",
                "name":"提交工单",
                "key":"work_ticket"
            }
        ]
    }]
})

robot.run()
