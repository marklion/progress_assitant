//
// Autogenerated by Thrift Compiler (0.14.1)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//
if (typeof Int64 === 'undefined' && typeof require === 'function') {
  const Int64 = require('node-int64');
}


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
    if (args) {
      if (args.type_id !== undefined && args.type_id !== null) {
        this.type_id = args.type_id;
      }
      if (args.count !== undefined && args.count !== null) {
        this.count = args.count;
      }
      if (args.vichele_info !== undefined && args.vichele_info !== null) {
        this.vichele_info = Thrift.copyList(args.vichele_info, [null]);
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
            elem3 = input.readString().value;
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
      output.writeListBegin(Thrift.Type.STRING, this.vichele_info.length);
      for (let iter4 in this.vichele_info) {
        if (this.vichele_info.hasOwnProperty(iter4)) {
          iter4 = this.vichele_info[iter4];
          output.writeString(iter4);
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
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
