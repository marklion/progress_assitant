//
// Autogenerated by Thrift Compiler (0.14.1)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//
if (typeof Int64 === 'undefined' && typeof require === 'function') {
  const Int64 = require('node-int64');
}


gen_exp = class {
  constructor(args) {
    this.msg = null;
    if (args) {
      if (args.msg !== undefined && args.msg !== null) {
        this.msg = args.msg;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.msg = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 0:
          input.skip(ftype);
          break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('gen_exp');
    if (this.msg !== null && this.msg !== undefined) {
      output.writeFieldBegin('msg', Thrift.Type.STRING, 1);
      output.writeString(this.msg);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_info = class {
  constructor(args) {
    this.user_id = null;
    this.name = null;
    this.logo = null;
    this.company = null;
    this.buyer = null;
    this.phone = null;
    if (args) {
      if (args.user_id !== undefined && args.user_id !== null) {
        this.user_id = args.user_id;
      }
      if (args.name !== undefined && args.name !== null) {
        this.name = args.name;
      }
      if (args.logo !== undefined && args.logo !== null) {
        this.logo = args.logo;
      }
      if (args.company !== undefined && args.company !== null) {
        this.company = args.company;
      }
      if (args.buyer !== undefined && args.buyer !== null) {
        this.buyer = args.buyer;
      }
      if (args.phone !== undefined && args.phone !== null) {
        this.phone = args.phone;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.I64) {
          this.user_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.STRING) {
          this.logo = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 4:
        if (ftype == Thrift.Type.STRING) {
          this.company = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 5:
        if (ftype == Thrift.Type.BOOL) {
          this.buyer = input.readBool().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 6:
        if (ftype == Thrift.Type.STRING) {
          this.phone = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('user_info');
    if (this.user_id !== null && this.user_id !== undefined) {
      output.writeFieldBegin('user_id', Thrift.Type.I64, 1);
      output.writeI64(this.user_id);
      output.writeFieldEnd();
    }
    if (this.name !== null && this.name !== undefined) {
      output.writeFieldBegin('name', Thrift.Type.STRING, 2);
      output.writeString(this.name);
      output.writeFieldEnd();
    }
    if (this.logo !== null && this.logo !== undefined) {
      output.writeFieldBegin('logo', Thrift.Type.STRING, 3);
      output.writeString(this.logo);
      output.writeFieldEnd();
    }
    if (this.company !== null && this.company !== undefined) {
      output.writeFieldBegin('company', Thrift.Type.STRING, 4);
      output.writeString(this.company);
      output.writeFieldEnd();
    }
    if (this.buyer !== null && this.buyer !== undefined) {
      output.writeFieldBegin('buyer', Thrift.Type.BOOL, 5);
      output.writeBool(this.buyer);
      output.writeFieldEnd();
    }
    if (this.phone !== null && this.phone !== undefined) {
      output.writeFieldBegin('phone', Thrift.Type.STRING, 6);
      output.writeString(this.phone);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
driver_info = class {
  constructor(args) {
    this.name = null;
    this.phone = null;
    this.id = null;
    if (args) {
      if (args.name !== undefined && args.name !== null) {
        this.name = args.name;
      }
      if (args.phone !== undefined && args.phone !== null) {
        this.phone = args.phone;
      }
      if (args.id !== undefined && args.id !== null) {
        this.id = args.id;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.phone = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.I64) {
          this.id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('driver_info');
    if (this.name !== null && this.name !== undefined) {
      output.writeFieldBegin('name', Thrift.Type.STRING, 1);
      output.writeString(this.name);
      output.writeFieldEnd();
    }
    if (this.phone !== null && this.phone !== undefined) {
      output.writeFieldBegin('phone', Thrift.Type.STRING, 2);
      output.writeString(this.phone);
      output.writeFieldEnd();
    }
    if (this.id !== null && this.id !== undefined) {
      output.writeFieldBegin('id', Thrift.Type.I64, 3);
      output.writeI64(this.id);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
vichele_info_t = class {
  constructor(args) {
    this.number = null;
    this.id = null;
    if (args) {
      if (args.number !== undefined && args.number !== null) {
        this.number = args.number;
      }
      if (args.id !== undefined && args.id !== null) {
        this.id = args.id;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.number = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.I64) {
          this.id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('vichele_info_t');
    if (this.number !== null && this.number !== undefined) {
      output.writeFieldBegin('number', Thrift.Type.STRING, 1);
      output.writeString(this.number);
      output.writeFieldEnd();
    }
    if (this.id !== null && this.id !== undefined) {
      output.writeFieldBegin('id', Thrift.Type.I64, 2);
      output.writeI64(this.id);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_detail = class {
  constructor(args) {
    this.name = null;
    this.last = null;
    this.price = null;
    this.company = null;
    this.type_id = null;
    this.saling = null;
    if (args) {
      if (args.name !== undefined && args.name !== null) {
        this.name = args.name;
      }
      if (args.last !== undefined && args.last !== null) {
        this.last = args.last;
      }
      if (args.price !== undefined && args.price !== null) {
        this.price = args.price;
      }
      if (args.company !== undefined && args.company !== null) {
        this.company = args.company;
      }
      if (args.type_id !== undefined && args.type_id !== null) {
        this.type_id = args.type_id;
      }
      if (args.saling !== undefined && args.saling !== null) {
        this.saling = args.saling;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.last = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.I64) {
          this.price = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 4:
        if (ftype == Thrift.Type.STRING) {
          this.company = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 5:
        if (ftype == Thrift.Type.I64) {
          this.type_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 6:
        if (ftype == Thrift.Type.BOOL) {
          this.saling = input.readBool().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('stuff_detail');
    if (this.name !== null && this.name !== undefined) {
      output.writeFieldBegin('name', Thrift.Type.STRING, 1);
      output.writeString(this.name);
      output.writeFieldEnd();
    }
    if (this.last !== null && this.last !== undefined) {
      output.writeFieldBegin('last', Thrift.Type.STRING, 2);
      output.writeString(this.last);
      output.writeFieldEnd();
    }
    if (this.price !== null && this.price !== undefined) {
      output.writeFieldBegin('price', Thrift.Type.I64, 3);
      output.writeI64(this.price);
      output.writeFieldEnd();
    }
    if (this.company !== null && this.company !== undefined) {
      output.writeFieldBegin('company', Thrift.Type.STRING, 4);
      output.writeString(this.company);
      output.writeFieldEnd();
    }
    if (this.type_id !== null && this.type_id !== undefined) {
      output.writeFieldBegin('type_id', Thrift.Type.I64, 5);
      output.writeI64(this.type_id);
      output.writeFieldEnd();
    }
    if (this.saling !== null && this.saling !== undefined) {
      output.writeFieldBegin('saling', Thrift.Type.BOOL, 6);
      output.writeBool(this.saling);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_apply = class {
  constructor(args) {
    this.name = null;
    this.phone = null;
    this.logo = null;
    this.apply_id = null;
    this.status = null;
    if (args) {
      if (args.name !== undefined && args.name !== null) {
        this.name = args.name;
      }
      if (args.phone !== undefined && args.phone !== null) {
        this.phone = args.phone;
      }
      if (args.logo !== undefined && args.logo !== null) {
        this.logo = args.logo;
      }
      if (args.apply_id !== undefined && args.apply_id !== null) {
        this.apply_id = args.apply_id;
      }
      if (args.status !== undefined && args.status !== null) {
        this.status = args.status;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.phone = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.STRING) {
          this.logo = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 4:
        if (ftype == Thrift.Type.I64) {
          this.apply_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 5:
        if (ftype == Thrift.Type.I64) {
          this.status = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('user_apply');
    if (this.name !== null && this.name !== undefined) {
      output.writeFieldBegin('name', Thrift.Type.STRING, 1);
      output.writeString(this.name);
      output.writeFieldEnd();
    }
    if (this.phone !== null && this.phone !== undefined) {
      output.writeFieldBegin('phone', Thrift.Type.STRING, 2);
      output.writeString(this.phone);
      output.writeFieldEnd();
    }
    if (this.logo !== null && this.logo !== undefined) {
      output.writeFieldBegin('logo', Thrift.Type.STRING, 3);
      output.writeString(this.logo);
      output.writeFieldEnd();
    }
    if (this.apply_id !== null && this.apply_id !== undefined) {
      output.writeFieldBegin('apply_id', Thrift.Type.I64, 4);
      output.writeI64(this.apply_id);
      output.writeFieldEnd();
    }
    if (this.status !== null && this.status !== undefined) {
      output.writeFieldBegin('status', Thrift.Type.I64, 5);
      output.writeI64(this.status);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
plan_confirm_info = class {
  constructor(args) {
    this.timestamp = null;
    this.name = null;
    if (args) {
      if (args.timestamp !== undefined && args.timestamp !== null) {
        this.timestamp = args.timestamp;
      }
      if (args.name !== undefined && args.name !== null) {
        this.name = args.name;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.timestamp = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('plan_confirm_info');
    if (this.timestamp !== null && this.timestamp !== undefined) {
      output.writeFieldBegin('timestamp', Thrift.Type.STRING, 1);
      output.writeString(this.timestamp);
      output.writeFieldEnd();
    }
    if (this.name !== null && this.name !== undefined) {
      output.writeFieldBegin('name', Thrift.Type.STRING, 2);
      output.writeString(this.name);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
pay_confirm_info = class {
  constructor(args) {
    this.timestamp = null;
    this.name = null;
    if (args) {
      if (args.timestamp !== undefined && args.timestamp !== null) {
        this.timestamp = args.timestamp;
      }
      if (args.name !== undefined && args.name !== null) {
        this.name = args.name;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.timestamp = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('pay_confirm_info');
    if (this.timestamp !== null && this.timestamp !== undefined) {
      output.writeFieldBegin('timestamp', Thrift.Type.STRING, 1);
      output.writeString(this.timestamp);
      output.writeFieldEnd();
    }
    if (this.name !== null && this.name !== undefined) {
      output.writeFieldBegin('name', Thrift.Type.STRING, 2);
      output.writeString(this.name);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
vichele_in_plan = class {
  constructor(args) {
    this.main_vichele = null;
    this.behind_vichele = null;
    this.driver_name = null;
    this.driver_phone = null;
    this.count = null;
    this.drop_address = null;
    this.use_for = null;
    if (args) {
      if (args.main_vichele !== undefined && args.main_vichele !== null) {
        this.main_vichele = args.main_vichele;
      }
      if (args.behind_vichele !== undefined && args.behind_vichele !== null) {
        this.behind_vichele = args.behind_vichele;
      }
      if (args.driver_name !== undefined && args.driver_name !== null) {
        this.driver_name = args.driver_name;
      }
      if (args.driver_phone !== undefined && args.driver_phone !== null) {
        this.driver_phone = args.driver_phone;
      }
      if (args.count !== undefined && args.count !== null) {
        this.count = args.count;
      }
      if (args.drop_address !== undefined && args.drop_address !== null) {
        this.drop_address = args.drop_address;
      }
      if (args.use_for !== undefined && args.use_for !== null) {
        this.use_for = args.use_for;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.STRING) {
          this.main_vichele = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.behind_vichele = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.STRING) {
          this.driver_name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 4:
        if (ftype == Thrift.Type.STRING) {
          this.driver_phone = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 5:
        if (ftype == Thrift.Type.DOUBLE) {
          this.count = input.readDouble().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 6:
        if (ftype == Thrift.Type.STRING) {
          this.drop_address = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 7:
        if (ftype == Thrift.Type.STRING) {
          this.use_for = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('vichele_in_plan');
    if (this.main_vichele !== null && this.main_vichele !== undefined) {
      output.writeFieldBegin('main_vichele', Thrift.Type.STRING, 1);
      output.writeString(this.main_vichele);
      output.writeFieldEnd();
    }
    if (this.behind_vichele !== null && this.behind_vichele !== undefined) {
      output.writeFieldBegin('behind_vichele', Thrift.Type.STRING, 2);
      output.writeString(this.behind_vichele);
      output.writeFieldEnd();
    }
    if (this.driver_name !== null && this.driver_name !== undefined) {
      output.writeFieldBegin('driver_name', Thrift.Type.STRING, 3);
      output.writeString(this.driver_name);
      output.writeFieldEnd();
    }
    if (this.driver_phone !== null && this.driver_phone !== undefined) {
      output.writeFieldBegin('driver_phone', Thrift.Type.STRING, 4);
      output.writeString(this.driver_phone);
      output.writeFieldEnd();
    }
    if (this.count !== null && this.count !== undefined) {
      output.writeFieldBegin('count', Thrift.Type.DOUBLE, 5);
      output.writeDouble(this.count);
      output.writeFieldEnd();
    }
    if (this.drop_address !== null && this.drop_address !== undefined) {
      output.writeFieldBegin('drop_address', Thrift.Type.STRING, 6);
      output.writeString(this.drop_address);
      output.writeFieldEnd();
    }
    if (this.use_for !== null && this.use_for !== undefined) {
      output.writeFieldBegin('use_for', Thrift.Type.STRING, 7);
      output.writeString(this.use_for);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_plan = class {
  constructor(args) {
    this.type_id = null;
    this.count = null;
    this.vichele_info = null;
    this.plan_id = null;
    this.created_by = null;
    this.plan_time = null;
    this.created_time = null;
    this.name = null;
    this.price = null;
    this.status = null;
    this.comment = null;
    this.plan_confirm = null;
    this.pay_confirm = null;
    this.pay_info = null;
    this.pay_timestamp = null;
    this.close_timestamp = null;
    this.close_by = null;
    this.except_close_by = null;
    this.except_close_timestamp = null;
    this.except_close_reason = null;
    this.proxy_company = null;
    if (args) {
      if (args.type_id !== undefined && args.type_id !== null) {
        this.type_id = args.type_id;
      }
      if (args.count !== undefined && args.count !== null) {
        this.count = args.count;
      }
      if (args.vichele_info !== undefined && args.vichele_info !== null) {
        this.vichele_info = Thrift.copyList(args.vichele_info, [vichele_in_plan]);
      }
      if (args.plan_id !== undefined && args.plan_id !== null) {
        this.plan_id = args.plan_id;
      }
      if (args.created_by !== undefined && args.created_by !== null) {
        this.created_by = args.created_by;
      }
      if (args.plan_time !== undefined && args.plan_time !== null) {
        this.plan_time = args.plan_time;
      }
      if (args.created_time !== undefined && args.created_time !== null) {
        this.created_time = args.created_time;
      }
      if (args.name !== undefined && args.name !== null) {
        this.name = args.name;
      }
      if (args.price !== undefined && args.price !== null) {
        this.price = args.price;
      }
      if (args.status !== undefined && args.status !== null) {
        this.status = args.status;
      }
      if (args.comment !== undefined && args.comment !== null) {
        this.comment = args.comment;
      }
      if (args.plan_confirm !== undefined && args.plan_confirm !== null) {
        this.plan_confirm = new plan_confirm_info(args.plan_confirm);
      }
      if (args.pay_confirm !== undefined && args.pay_confirm !== null) {
        this.pay_confirm = new pay_confirm_info(args.pay_confirm);
      }
      if (args.pay_info !== undefined && args.pay_info !== null) {
        this.pay_info = args.pay_info;
      }
      if (args.pay_timestamp !== undefined && args.pay_timestamp !== null) {
        this.pay_timestamp = args.pay_timestamp;
      }
      if (args.close_timestamp !== undefined && args.close_timestamp !== null) {
        this.close_timestamp = args.close_timestamp;
      }
      if (args.close_by !== undefined && args.close_by !== null) {
        this.close_by = args.close_by;
      }
      if (args.except_close_by !== undefined && args.except_close_by !== null) {
        this.except_close_by = args.except_close_by;
      }
      if (args.except_close_timestamp !== undefined && args.except_close_timestamp !== null) {
        this.except_close_timestamp = args.except_close_timestamp;
      }
      if (args.except_close_reason !== undefined && args.except_close_reason !== null) {
        this.except_close_reason = args.except_close_reason;
      }
      if (args.proxy_company !== undefined && args.proxy_company !== null) {
        this.proxy_company = args.proxy_company;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.I64) {
          this.type_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.DOUBLE) {
          this.count = input.readDouble().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.LIST) {
          this.vichele_info = [];
          const _rtmp31 = input.readListBegin();
          const _size0 = _rtmp31.size || 0;
          for (let _i2 = 0; _i2 < _size0; ++_i2) {
            let elem3 = null;
            elem3 = new vichele_in_plan();
            elem3.read(input);
            this.vichele_info.push(elem3);
          }
          input.readListEnd();
        } else {
          input.skip(ftype);
        }
        break;
        case 4:
        if (ftype == Thrift.Type.I64) {
          this.plan_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 5:
        if (ftype == Thrift.Type.I64) {
          this.created_by = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 6:
        if (ftype == Thrift.Type.STRING) {
          this.plan_time = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 7:
        if (ftype == Thrift.Type.I64) {
          this.created_time = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 8:
        if (ftype == Thrift.Type.STRING) {
          this.name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 9:
        if (ftype == Thrift.Type.DOUBLE) {
          this.price = input.readDouble().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 10:
        if (ftype == Thrift.Type.I64) {
          this.status = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 11:
        if (ftype == Thrift.Type.STRING) {
          this.comment = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 12:
        if (ftype == Thrift.Type.STRUCT) {
          this.plan_confirm = new plan_confirm_info();
          this.plan_confirm.read(input);
        } else {
          input.skip(ftype);
        }
        break;
        case 13:
        if (ftype == Thrift.Type.STRUCT) {
          this.pay_confirm = new pay_confirm_info();
          this.pay_confirm.read(input);
        } else {
          input.skip(ftype);
        }
        break;
        case 14:
        if (ftype == Thrift.Type.STRING) {
          this.pay_info = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 15:
        if (ftype == Thrift.Type.STRING) {
          this.pay_timestamp = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 16:
        if (ftype == Thrift.Type.STRING) {
          this.close_timestamp = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 17:
        if (ftype == Thrift.Type.STRING) {
          this.close_by = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 18:
        if (ftype == Thrift.Type.STRING) {
          this.except_close_by = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 19:
        if (ftype == Thrift.Type.STRING) {
          this.except_close_timestamp = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 20:
        if (ftype == Thrift.Type.STRING) {
          this.except_close_reason = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 21:
        if (ftype == Thrift.Type.STRING) {
          this.proxy_company = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('stuff_plan');
    if (this.type_id !== null && this.type_id !== undefined) {
      output.writeFieldBegin('type_id', Thrift.Type.I64, 1);
      output.writeI64(this.type_id);
      output.writeFieldEnd();
    }
    if (this.count !== null && this.count !== undefined) {
      output.writeFieldBegin('count', Thrift.Type.DOUBLE, 2);
      output.writeDouble(this.count);
      output.writeFieldEnd();
    }
    if (this.vichele_info !== null && this.vichele_info !== undefined) {
      output.writeFieldBegin('vichele_info', Thrift.Type.LIST, 3);
      output.writeListBegin(Thrift.Type.STRUCT, this.vichele_info.length);
      for (let iter4 in this.vichele_info) {
        if (this.vichele_info.hasOwnProperty(iter4)) {
          iter4 = this.vichele_info[iter4];
          iter4.write(output);
        }
      }
      output.writeListEnd();
      output.writeFieldEnd();
    }
    if (this.plan_id !== null && this.plan_id !== undefined) {
      output.writeFieldBegin('plan_id', Thrift.Type.I64, 4);
      output.writeI64(this.plan_id);
      output.writeFieldEnd();
    }
    if (this.created_by !== null && this.created_by !== undefined) {
      output.writeFieldBegin('created_by', Thrift.Type.I64, 5);
      output.writeI64(this.created_by);
      output.writeFieldEnd();
    }
    if (this.plan_time !== null && this.plan_time !== undefined) {
      output.writeFieldBegin('plan_time', Thrift.Type.STRING, 6);
      output.writeString(this.plan_time);
      output.writeFieldEnd();
    }
    if (this.created_time !== null && this.created_time !== undefined) {
      output.writeFieldBegin('created_time', Thrift.Type.I64, 7);
      output.writeI64(this.created_time);
      output.writeFieldEnd();
    }
    if (this.name !== null && this.name !== undefined) {
      output.writeFieldBegin('name', Thrift.Type.STRING, 8);
      output.writeString(this.name);
      output.writeFieldEnd();
    }
    if (this.price !== null && this.price !== undefined) {
      output.writeFieldBegin('price', Thrift.Type.DOUBLE, 9);
      output.writeDouble(this.price);
      output.writeFieldEnd();
    }
    if (this.status !== null && this.status !== undefined) {
      output.writeFieldBegin('status', Thrift.Type.I64, 10);
      output.writeI64(this.status);
      output.writeFieldEnd();
    }
    if (this.comment !== null && this.comment !== undefined) {
      output.writeFieldBegin('comment', Thrift.Type.STRING, 11);
      output.writeString(this.comment);
      output.writeFieldEnd();
    }
    if (this.plan_confirm !== null && this.plan_confirm !== undefined) {
      output.writeFieldBegin('plan_confirm', Thrift.Type.STRUCT, 12);
      this.plan_confirm.write(output);
      output.writeFieldEnd();
    }
    if (this.pay_confirm !== null && this.pay_confirm !== undefined) {
      output.writeFieldBegin('pay_confirm', Thrift.Type.STRUCT, 13);
      this.pay_confirm.write(output);
      output.writeFieldEnd();
    }
    if (this.pay_info !== null && this.pay_info !== undefined) {
      output.writeFieldBegin('pay_info', Thrift.Type.STRING, 14);
      output.writeString(this.pay_info);
      output.writeFieldEnd();
    }
    if (this.pay_timestamp !== null && this.pay_timestamp !== undefined) {
      output.writeFieldBegin('pay_timestamp', Thrift.Type.STRING, 15);
      output.writeString(this.pay_timestamp);
      output.writeFieldEnd();
    }
    if (this.close_timestamp !== null && this.close_timestamp !== undefined) {
      output.writeFieldBegin('close_timestamp', Thrift.Type.STRING, 16);
      output.writeString(this.close_timestamp);
      output.writeFieldEnd();
    }
    if (this.close_by !== null && this.close_by !== undefined) {
      output.writeFieldBegin('close_by', Thrift.Type.STRING, 17);
      output.writeString(this.close_by);
      output.writeFieldEnd();
    }
    if (this.except_close_by !== null && this.except_close_by !== undefined) {
      output.writeFieldBegin('except_close_by', Thrift.Type.STRING, 18);
      output.writeString(this.except_close_by);
      output.writeFieldEnd();
    }
    if (this.except_close_timestamp !== null && this.except_close_timestamp !== undefined) {
      output.writeFieldBegin('except_close_timestamp', Thrift.Type.STRING, 19);
      output.writeString(this.except_close_timestamp);
      output.writeFieldEnd();
    }
    if (this.except_close_reason !== null && this.except_close_reason !== undefined) {
      output.writeFieldBegin('except_close_reason', Thrift.Type.STRING, 20);
      output.writeString(this.except_close_reason);
      output.writeFieldEnd();
    }
    if (this.proxy_company !== null && this.proxy_company !== undefined) {
      output.writeFieldBegin('proxy_company', Thrift.Type.STRING, 21);
      output.writeString(this.proxy_company);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
plan_status = class {
  constructor(args) {
    this.plan_id = null;
    this.status = null;
    this.plan_time = null;
    if (args) {
      if (args.plan_id !== undefined && args.plan_id !== null) {
        this.plan_id = args.plan_id;
      }
      if (args.status !== undefined && args.status !== null) {
        this.status = args.status;
      }
      if (args.plan_time !== undefined && args.plan_time !== null) {
        this.plan_time = args.plan_time;
      }
    }
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      const fid = ret.fid;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      switch (fid) {
        case 1:
        if (ftype == Thrift.Type.I64) {
          this.plan_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.I64) {
          this.status = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.I64) {
          this.plan_time = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        default:
          input.skip(ftype);
      }
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('plan_status');
    if (this.plan_id !== null && this.plan_id !== undefined) {
      output.writeFieldBegin('plan_id', Thrift.Type.I64, 1);
      output.writeI64(this.plan_id);
      output.writeFieldEnd();
    }
    if (this.status !== null && this.status !== undefined) {
      output.writeFieldBegin('status', Thrift.Type.I64, 2);
      output.writeI64(this.status);
      output.writeFieldEnd();
    }
    if (this.plan_time !== null && this.plan_time !== undefined) {
      output.writeFieldBegin('plan_time', Thrift.Type.I64, 3);
      output.writeI64(this.plan_time);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
