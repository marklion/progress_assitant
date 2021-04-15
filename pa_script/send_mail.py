#!/usr/bin/python3

import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.mime.application import MIMEApplication
from email.header import Header
import sys
import os

def send_mail(_to_user, _title, _content, _attachments):
    smtpObj = smtplib.SMTP_SSL('smtp.qiye.aliyun.com', 465)
    ret = False
    try :
        smtpObj.login('postmaster@d8sis.cn', os.getenv('MAIL_PWD'))
        message = MIMEMultipart()
        message['From'] = Header('掌易助理<postmaster@d8sis.cn>', 'utf-8')   # 发送者
        message['To'] = _to_user       # 接收者
        subject = _title
        message['Subject'] = Header(subject, 'utf-8')
        content = MIMEText(_content, 'plain', 'utf-8')
        message.attach(content)
        with open(_attachments, 'rb') as f:
            att_file = f.read()
        file_content = MIMEApplication(att_file)
        file_content.add_header('Content-Disposition', 'attachment',  filename="计划导出表.xlsx")
        message.attach(file_content)

        smtpObj.sendmail('postmaster@d8sis.cn',
                        [_to_user], message.as_string())
        ret = True
    except smtplib.SMTPException:
        ret = False
    
    return ret

if __name__ == '__main__':
    _to_user = sys.argv[1]
    _title = sys.argv[2]
    _content = sys.argv[3]
    _attachments = sys.argv[4]

    if send_mail(_to_user, _title, _content, _attachments):
        sys.exit(0)
    else:
        sys.exit(1)

