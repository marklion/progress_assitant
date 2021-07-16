//
// Autogenerated by Thrift Compiler (0.14.1)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//
if (typeof Int64 === 'undefined' && typeof require === 'function') {
  const Int64 = require('node-int64');
}


//HELPER FUNCTIONS AND STRUCTURES

stuff_info_get_today_args = class {
  constructor(args) {
    this.ssid = null;
    if (args) {
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
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
          this.ssid = input.readString().value;
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
    output.writeStructBegin('stuff_info_get_today_args');
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 1);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_today_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = Thrift.copyList(args.success, [stuff_detail]);
      }
      if (args.e !== undefined && args.e !== null) {
        this.e = args.e;
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
        case 0:
        if (ftype == Thrift.Type.LIST) {
          this.success = [];
          const _rtmp351 = input.readListBegin();
          const _size50 = _rtmp351.size || 0;
          for (let _i52 = 0; _i52 < _size50; ++_i52) {
            let elem53 = null;
            elem53 = new stuff_detail();
            elem53.read(input);
            this.success.push(elem53);
          }
          input.readListEnd();
        } else {
          input.skip(ftype);
        }
        break;
        case 1:
        if (ftype == Thrift.Type.STRUCT) {
          this.e = new gen_exp();
          this.e.read(input);
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
    output.writeStructBegin('stuff_info_get_today_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.LIST, 0);
      output.writeListBegin(Thrift.Type.STRUCT, this.success.length);
      for (let iter54 in this.success) {
        if (this.success.hasOwnProperty(iter54)) {
          iter54 = this.success[iter54];
          iter54.write(output);
        }
      }
      output.writeListEnd();
      output.writeFieldEnd();
    }
    if (this.e !== null && this.e !== undefined) {
      output.writeFieldBegin('e', Thrift.Type.STRUCT, 1);
      this.e.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_today_unfollow_args = class {
  constructor(args) {
    this.ssid = null;
    if (args) {
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
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
          this.ssid = input.readString().value;
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
    output.writeStructBegin('stuff_info_get_today_unfollow_args');
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 1);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_today_unfollow_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = Thrift.copyList(args.success, [stuff_detail]);
      }
      if (args.e !== undefined && args.e !== null) {
        this.e = args.e;
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
        case 0:
        if (ftype == Thrift.Type.LIST) {
          this.success = [];
          const _rtmp356 = input.readListBegin();
          const _size55 = _rtmp356.size || 0;
          for (let _i57 = 0; _i57 < _size55; ++_i57) {
            let elem58 = null;
            elem58 = new stuff_detail();
            elem58.read(input);
            this.success.push(elem58);
          }
          input.readListEnd();
        } else {
          input.skip(ftype);
        }
        break;
        case 1:
        if (ftype == Thrift.Type.STRUCT) {
          this.e = new gen_exp();
          this.e.read(input);
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
    output.writeStructBegin('stuff_info_get_today_unfollow_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.LIST, 0);
      output.writeListBegin(Thrift.Type.STRUCT, this.success.length);
      for (let iter59 in this.success) {
        if (this.success.hasOwnProperty(iter59)) {
          iter59 = this.success[iter59];
          iter59.write(output);
        }
      }
      output.writeListEnd();
      output.writeFieldEnd();
    }
    if (this.e !== null && this.e !== undefined) {
      output.writeFieldBegin('e', Thrift.Type.STRUCT, 1);
      this.e.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_stuff_detail_args = class {
  constructor(args) {
    this.type_id = null;
    this.ssid = null;
    if (args) {
      if (args.type_id !== undefined && args.type_id !== null) {
        this.type_id = args.type_id;
      }
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
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
        if (ftype == Thrift.Type.STRING) {
          this.ssid = input.readString().value;
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
    output.writeStructBegin('stuff_info_get_stuff_detail_args');
    if (this.type_id !== null && this.type_id !== undefined) {
      output.writeFieldBegin('type_id', Thrift.Type.I64, 1);
      output.writeI64(this.type_id);
      output.writeFieldEnd();
    }
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 2);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_stuff_detail_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = new stuff_detail(args.success);
      }
      if (args.e !== undefined && args.e !== null) {
        this.e = args.e;
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
        case 0:
        if (ftype == Thrift.Type.STRUCT) {
          this.success = new stuff_detail();
          this.success.read(input);
        } else {
          input.skip(ftype);
        }
        break;
        case 1:
        if (ftype == Thrift.Type.STRUCT) {
          this.e = new gen_exp();
          this.e.read(input);
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
    output.writeStructBegin('stuff_info_get_stuff_detail_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.STRUCT, 0);
      this.success.write(output);
      output.writeFieldEnd();
    }
    if (this.e !== null && this.e !== undefined) {
      output.writeFieldBegin('e', Thrift.Type.STRUCT, 1);
      this.e.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_add_company_follow_stuff_args = class {
  constructor(args) {
    this.company_name = null;
    this.type_id = null;
    this.ssid = null;
    if (args) {
      if (args.company_name !== undefined && args.company_name !== null) {
        this.company_name = args.company_name;
      }
      if (args.type_id !== undefined && args.type_id !== null) {
        this.type_id = args.type_id;
      }
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
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
          this.company_name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.I64) {
          this.type_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.STRING) {
          this.ssid = input.readString().value;
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
    output.writeStructBegin('stuff_info_add_company_follow_stuff_args');
    if (this.company_name !== null && this.company_name !== undefined) {
      output.writeFieldBegin('company_name', Thrift.Type.STRING, 1);
      output.writeString(this.company_name);
      output.writeFieldEnd();
    }
    if (this.type_id !== null && this.type_id !== undefined) {
      output.writeFieldBegin('type_id', Thrift.Type.I64, 2);
      output.writeI64(this.type_id);
      output.writeFieldEnd();
    }
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 3);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_add_company_follow_stuff_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = args.success;
      }
      if (args.e !== undefined && args.e !== null) {
        this.e = args.e;
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
        case 0:
        if (ftype == Thrift.Type.BOOL) {
          this.success = input.readBool().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 1:
        if (ftype == Thrift.Type.STRUCT) {
          this.e = new gen_exp();
          this.e.read(input);
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
    output.writeStructBegin('stuff_info_add_company_follow_stuff_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.BOOL, 0);
      output.writeBool(this.success);
      output.writeFieldEnd();
    }
    if (this.e !== null && this.e !== undefined) {
      output.writeFieldBegin('e', Thrift.Type.STRUCT, 1);
      this.e.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_cancle_company_follow_stuff_args = class {
  constructor(args) {
    this.company_name = null;
    this.type_id = null;
    this.ssid = null;
    if (args) {
      if (args.company_name !== undefined && args.company_name !== null) {
        this.company_name = args.company_name;
      }
      if (args.type_id !== undefined && args.type_id !== null) {
        this.type_id = args.type_id;
      }
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
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
          this.company_name = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.I64) {
          this.type_id = input.readI64().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.STRING) {
          this.ssid = input.readString().value;
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
    output.writeStructBegin('stuff_info_cancle_company_follow_stuff_args');
    if (this.company_name !== null && this.company_name !== undefined) {
      output.writeFieldBegin('company_name', Thrift.Type.STRING, 1);
      output.writeString(this.company_name);
      output.writeFieldEnd();
    }
    if (this.type_id !== null && this.type_id !== undefined) {
      output.writeFieldBegin('type_id', Thrift.Type.I64, 2);
      output.writeI64(this.type_id);
      output.writeFieldEnd();
    }
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 3);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_cancle_company_follow_stuff_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = args.success;
      }
      if (args.e !== undefined && args.e !== null) {
        this.e = args.e;
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
        case 0:
        if (ftype == Thrift.Type.BOOL) {
          this.success = input.readBool().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 1:
        if (ftype == Thrift.Type.STRUCT) {
          this.e = new gen_exp();
          this.e.read(input);
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
    output.writeStructBegin('stuff_info_cancle_company_follow_stuff_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.BOOL, 0);
      output.writeBool(this.success);
      output.writeFieldEnd();
    }
    if (this.e !== null && this.e !== undefined) {
      output.writeFieldBegin('e', Thrift.Type.STRUCT, 1);
      this.e.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_follow_stuff_by_company_args = class {
  constructor(args) {
    this.company_name = null;
    if (args) {
      if (args.company_name !== undefined && args.company_name !== null) {
        this.company_name = args.company_name;
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
          this.company_name = input.readString().value;
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
    output.writeStructBegin('stuff_info_get_follow_stuff_by_company_args');
    if (this.company_name !== null && this.company_name !== undefined) {
      output.writeFieldBegin('company_name', Thrift.Type.STRING, 1);
      output.writeString(this.company_name);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_follow_stuff_by_company_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = Thrift.copyList(args.success, [stuff_detail]);
      }
      if (args.e !== undefined && args.e !== null) {
        this.e = args.e;
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
        case 0:
        if (ftype == Thrift.Type.LIST) {
          this.success = [];
          const _rtmp361 = input.readListBegin();
          const _size60 = _rtmp361.size || 0;
          for (let _i62 = 0; _i62 < _size60; ++_i62) {
            let elem63 = null;
            elem63 = new stuff_detail();
            elem63.read(input);
            this.success.push(elem63);
          }
          input.readListEnd();
        } else {
          input.skip(ftype);
        }
        break;
        case 1:
        if (ftype == Thrift.Type.STRUCT) {
          this.e = new gen_exp();
          this.e.read(input);
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
    output.writeStructBegin('stuff_info_get_follow_stuff_by_company_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.LIST, 0);
      output.writeListBegin(Thrift.Type.STRUCT, this.success.length);
      for (let iter64 in this.success) {
        if (this.success.hasOwnProperty(iter64)) {
          iter64 = this.success[iter64];
          iter64.write(output);
        }
      }
      output.writeListEnd();
      output.writeFieldEnd();
    }
    if (this.e !== null && this.e !== undefined) {
      output.writeFieldBegin('e', Thrift.Type.STRUCT, 1);
      this.e.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_follow_company_by_stuff_args = class {
  constructor(args) {
    this.type_id = null;
    this.ssid = null;
    if (args) {
      if (args.type_id !== undefined && args.type_id !== null) {
        this.type_id = args.type_id;
      }
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
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
        if (ftype == Thrift.Type.STRING) {
          this.ssid = input.readString().value;
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
    output.writeStructBegin('stuff_info_get_follow_company_by_stuff_args');
    if (this.type_id !== null && this.type_id !== undefined) {
      output.writeFieldBegin('type_id', Thrift.Type.I64, 1);
      output.writeI64(this.type_id);
      output.writeFieldEnd();
    }
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 2);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_info_get_follow_company_by_stuff_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = Thrift.copyList(args.success, [null]);
      }
      if (args.e !== undefined && args.e !== null) {
        this.e = args.e;
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
        case 0:
        if (ftype == Thrift.Type.LIST) {
          this.success = [];
          const _rtmp366 = input.readListBegin();
          const _size65 = _rtmp366.size || 0;
          for (let _i67 = 0; _i67 < _size65; ++_i67) {
            let elem68 = null;
            elem68 = input.readString().value;
            this.success.push(elem68);
          }
          input.readListEnd();
        } else {
          input.skip(ftype);
        }
        break;
        case 1:
        if (ftype == Thrift.Type.STRUCT) {
          this.e = new gen_exp();
          this.e.read(input);
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
    output.writeStructBegin('stuff_info_get_follow_company_by_stuff_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.LIST, 0);
      output.writeListBegin(Thrift.Type.STRING, this.success.length);
      for (let iter69 in this.success) {
        if (this.success.hasOwnProperty(iter69)) {
          iter69 = this.success[iter69];
          output.writeString(iter69);
        }
      }
      output.writeListEnd();
      output.writeFieldEnd();
    }
    if (this.e !== null && this.e !== undefined) {
      output.writeFieldBegin('e', Thrift.Type.STRUCT, 1);
      this.e.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_infoClient = class stuff_infoClient {
  constructor(input, output) {
    this.input = input;
    this.output = (!output) ? input : output;
    this.seqid = 0;
  }

  get_today (ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_today(ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_today (ssid, callback) {
    const params = {
      ssid: ssid
    };
    const args = new stuff_info_get_today_args(params);
    try {
      this.output.writeMessageBegin('get_today', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_today();
        } catch (e) {
          error = e;
        }
        callback(error, result);
      });
    }
    catch (e) {
      if (typeof this.output.getTransport().reset === 'function') {
        this.output.getTransport().reset();
      }
      throw e;
    }
  }

  recv_get_today () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_info_get_today_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'get_today failed: unknown result';
  }

  get_today_unfollow (ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_today_unfollow(ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_today_unfollow (ssid, callback) {
    const params = {
      ssid: ssid
    };
    const args = new stuff_info_get_today_unfollow_args(params);
    try {
      this.output.writeMessageBegin('get_today_unfollow', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_today_unfollow();
        } catch (e) {
          error = e;
        }
        callback(error, result);
      });
    }
    catch (e) {
      if (typeof this.output.getTransport().reset === 'function') {
        this.output.getTransport().reset();
      }
      throw e;
    }
  }

  recv_get_today_unfollow () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_info_get_today_unfollow_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'get_today_unfollow failed: unknown result';
  }

  get_stuff_detail (type_id, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_stuff_detail(type_id, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_stuff_detail (type_id, ssid, callback) {
    const params = {
      type_id: type_id,
      ssid: ssid
    };
    const args = new stuff_info_get_stuff_detail_args(params);
    try {
      this.output.writeMessageBegin('get_stuff_detail', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_stuff_detail();
        } catch (e) {
          error = e;
        }
        callback(error, result);
      });
    }
    catch (e) {
      if (typeof this.output.getTransport().reset === 'function') {
        this.output.getTransport().reset();
      }
      throw e;
    }
  }

  recv_get_stuff_detail () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_info_get_stuff_detail_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'get_stuff_detail failed: unknown result';
  }

  add_company_follow_stuff (company_name, type_id, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_add_company_follow_stuff(company_name, type_id, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_add_company_follow_stuff (company_name, type_id, ssid, callback) {
    const params = {
      company_name: company_name,
      type_id: type_id,
      ssid: ssid
    };
    const args = new stuff_info_add_company_follow_stuff_args(params);
    try {
      this.output.writeMessageBegin('add_company_follow_stuff', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_add_company_follow_stuff();
        } catch (e) {
          error = e;
        }
        callback(error, result);
      });
    }
    catch (e) {
      if (typeof this.output.getTransport().reset === 'function') {
        this.output.getTransport().reset();
      }
      throw e;
    }
  }

  recv_add_company_follow_stuff () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_info_add_company_follow_stuff_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'add_company_follow_stuff failed: unknown result';
  }

  cancle_company_follow_stuff (company_name, type_id, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_cancle_company_follow_stuff(company_name, type_id, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_cancle_company_follow_stuff (company_name, type_id, ssid, callback) {
    const params = {
      company_name: company_name,
      type_id: type_id,
      ssid: ssid
    };
    const args = new stuff_info_cancle_company_follow_stuff_args(params);
    try {
      this.output.writeMessageBegin('cancle_company_follow_stuff', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_cancle_company_follow_stuff();
        } catch (e) {
          error = e;
        }
        callback(error, result);
      });
    }
    catch (e) {
      if (typeof this.output.getTransport().reset === 'function') {
        this.output.getTransport().reset();
      }
      throw e;
    }
  }

  recv_cancle_company_follow_stuff () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_info_cancle_company_follow_stuff_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'cancle_company_follow_stuff failed: unknown result';
  }

  get_follow_stuff_by_company (company_name) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_follow_stuff_by_company(company_name, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_follow_stuff_by_company (company_name, callback) {
    const params = {
      company_name: company_name
    };
    const args = new stuff_info_get_follow_stuff_by_company_args(params);
    try {
      this.output.writeMessageBegin('get_follow_stuff_by_company', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_follow_stuff_by_company();
        } catch (e) {
          error = e;
        }
        callback(error, result);
      });
    }
    catch (e) {
      if (typeof this.output.getTransport().reset === 'function') {
        this.output.getTransport().reset();
      }
      throw e;
    }
  }

  recv_get_follow_stuff_by_company () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_info_get_follow_stuff_by_company_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'get_follow_stuff_by_company failed: unknown result';
  }

  get_follow_company_by_stuff (type_id, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_follow_company_by_stuff(type_id, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_follow_company_by_stuff (type_id, ssid, callback) {
    const params = {
      type_id: type_id,
      ssid: ssid
    };
    const args = new stuff_info_get_follow_company_by_stuff_args(params);
    try {
      this.output.writeMessageBegin('get_follow_company_by_stuff', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_follow_company_by_stuff();
        } catch (e) {
          error = e;
        }
        callback(error, result);
      });
    }
    catch (e) {
      if (typeof this.output.getTransport().reset === 'function') {
        this.output.getTransport().reset();
      }
      throw e;
    }
  }

  recv_get_follow_company_by_stuff () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_info_get_follow_company_by_stuff_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'get_follow_company_by_stuff failed: unknown result';
  }
};
