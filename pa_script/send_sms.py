#! /usr/bin/python3
# -*- coding: utf-8 -*-
# This file is auto-generated, don't edit it. Thanks.
import sys

from typing import List
import os

from alibabacloud_dysmsapi20170525.client import Client as Dysmsapi20170525Client
from alibabacloud_tea_openapi import models as open_api_models
from alibabacloud_dysmsapi20170525 import models as dysmsapi_20170525_models


class Sample:
    def __init__(self):
        pass

    @staticmethod
    def create_client(
        access_key_id: str,
        access_key_secret: str,
    ) -> Dysmsapi20170525Client:
        """
        使用AK&SK初始化账号Client
        @param access_key_id:
        @param access_key_secret:
        @return: Client
        @throws Exception
        """
        config = open_api_models.Config(
            # 您的AccessKey ID,
            access_key_id=access_key_id,
            # 您的AccessKey Secret,
            access_key_secret=access_key_secret
        )
        # 访问的域名
        config.endpoint = 'dysmsapi.aliyuncs.com'
        return Dysmsapi20170525Client(config)

    @staticmethod
    def main(
        args: List[str],
    ) -> None:
        client = Sample.create_client(os.getenv('ALI_KEY_ID'), os.getenv('ALI_KEY_SEC'))
        send_sms_request = dysmsapi_20170525_models.SendSmsRequest(
            phone_numbers=args[0],
            sign_name='掌易助理',
            template_code='SMS_215117138',
            template_param='{"code":"' + args[1] + '"}'
        )
        # 复制代码运行请自行打印 API 的返回值
        ret_val = client.send_sms(send_sms_request)
        print (ret_val.body)
        return ret_val.body.message == "OK"


    @staticmethod
    async def main_async(
        args: List[str],
    ) -> None:
        client = Sample.create_client('accessKeyId', 'accessKeySecret')
        send_sms_request = dysmsapi_20170525_models.SendSmsRequest(
            phone_numbers='18911992582',
            sign_name='掌易助理',
            template_code='SMS_215117138',
            template_param='{"code":"566784"}'
        )
        # 复制代码运行请自行打印 API 的返回值
        await client.send_sms_async(send_sms_request)


if __name__ == '__main__':
    if Sample.main(sys.argv[1:]):
        sys.exit(0)
    else:
        sys.exit(1)
