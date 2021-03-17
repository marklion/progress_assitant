//
// Autogenerated by Thrift Compiler (0.14.1)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//
if (typeof Int64 === 'undefined' && typeof require === 'function') {
  const Int64 = require('node-int64');
}


//HELPER FUNCTIONS AND STRUCTURES

user_management_get_user_info_args = class {
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
    output.writeStructBegin('user_management_get_user_info_args');
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
user_management_get_user_info_result = class {
  constructor(args) {
    this.success = null;
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = new user_info(args.success);
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
          this.success = new user_info();
          this.success.read(input);
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
    output.writeStructBegin('user_management_get_user_info_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.STRUCT, 0);
      this.success.write(output);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_user_login_args = class {
  constructor(args) {
    this.code = null;
    if (args) {
      if (args.code !== undefined && args.code !== null) {
        this.code = args.code;
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
          this.code = input.readString().value;
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
    output.writeStructBegin('user_management_user_login_args');
    if (this.code !== null && this.code !== undefined) {
      output.writeFieldBegin('code', Thrift.Type.STRING, 1);
      output.writeString(this.code);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_user_login_result = class {
  constructor(args) {
    this.success = null;
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = args.success;
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
        if (ftype == Thrift.Type.STRING) {
          this.success = input.readString().value;
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
    output.writeStructBegin('user_management_user_login_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.STRING, 0);
      output.writeString(this.success);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_update_user_info_args = class {
  constructor(args) {
    this.info = null;
    this.ssid = null;
    if (args) {
      if (args.info !== undefined && args.info !== null) {
        this.info = new user_info(args.info);
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
        if (ftype == Thrift.Type.STRUCT) {
          this.info = new user_info();
          this.info.read(input);
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
    output.writeStructBegin('user_management_update_user_info_args');
    if (this.info !== null && this.info !== undefined) {
      output.writeFieldBegin('info', Thrift.Type.STRUCT, 1);
      this.info.write(output);
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
user_management_update_user_info_result = class {
  constructor(args) {
    this.success = null;
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = args.success;
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
    output.writeStructBegin('user_management_update_user_info_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.BOOL, 0);
      output.writeBool(this.success);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_remove_user_args = class {
  constructor(args) {
    this.user_id = null;
    this.ssid = null;
    if (args) {
      if (args.user_id !== undefined && args.user_id !== null) {
        this.user_id = args.user_id;
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
          this.user_id = input.readI64().value;
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
    output.writeStructBegin('user_management_remove_user_args');
    if (this.user_id !== null && this.user_id !== undefined) {
      output.writeFieldBegin('user_id', Thrift.Type.I64, 1);
      output.writeI64(this.user_id);
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
user_management_remove_user_result = class {
  constructor(args) {
    this.success = null;
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = args.success;
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
    output.writeStructBegin('user_management_remove_user_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.BOOL, 0);
      output.writeBool(this.success);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_get_bound_vichele_args = class {
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
    output.writeStructBegin('user_management_get_bound_vichele_args');
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
user_management_get_bound_vichele_result = class {
  constructor(args) {
    this.success = null;
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = Thrift.copyList(args.success, [null]);
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
          const _rtmp36 = input.readListBegin();
          const _size5 = _rtmp36.size || 0;
          for (let _i7 = 0; _i7 < _size5; ++_i7) {
            let elem8 = null;
            elem8 = input.readString().value;
            this.success.push(elem8);
          }
          input.readListEnd();
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
    output.writeStructBegin('user_management_get_bound_vichele_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.LIST, 0);
      output.writeListBegin(Thrift.Type.STRING, this.success.length);
      for (let iter9 in this.success) {
        if (this.success.hasOwnProperty(iter9)) {
          iter9 = this.success[iter9];
          output.writeString(iter9);
        }
      }
      output.writeListEnd();
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_bind_new_vichele_args = class {
  constructor(args) {
    this.ssid = null;
    this.vichele = null;
    if (args) {
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
      }
      if (args.vichele !== undefined && args.vichele !== null) {
        this.vichele = args.vichele;
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
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.vichele = input.readString().value;
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
    output.writeStructBegin('user_management_bind_new_vichele_args');
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 1);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    if (this.vichele !== null && this.vichele !== undefined) {
      output.writeFieldBegin('vichele', Thrift.Type.STRING, 2);
      output.writeString(this.vichele);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_bind_new_vichele_result = class {
  constructor(args) {
    this.success = null;
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = args.success;
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
    output.writeStructBegin('user_management_bind_new_vichele_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.BOOL, 0);
      output.writeBool(this.success);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_remove_vichele_args = class {
  constructor(args) {
    this.ssid = null;
    this.vichele = null;
    if (args) {
      if (args.ssid !== undefined && args.ssid !== null) {
        this.ssid = args.ssid;
      }
      if (args.vichele !== undefined && args.vichele !== null) {
        this.vichele = args.vichele;
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
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.vichele = input.readString().value;
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
    output.writeStructBegin('user_management_remove_vichele_args');
    if (this.ssid !== null && this.ssid !== undefined) {
      output.writeFieldBegin('ssid', Thrift.Type.STRING, 1);
      output.writeString(this.ssid);
      output.writeFieldEnd();
    }
    if (this.vichele !== null && this.vichele !== undefined) {
      output.writeFieldBegin('vichele', Thrift.Type.STRING, 2);
      output.writeString(this.vichele);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_management_remove_vichele_result = class {
  constructor(args) {
  }

  read (input) {
    input.readStructBegin();
    while (true) {
      const ret = input.readFieldBegin();
      const ftype = ret.ftype;
      if (ftype == Thrift.Type.STOP) {
        break;
      }
      input.skip(ftype);
      input.readFieldEnd();
    }
    input.readStructEnd();
    return;
  }

  write (output) {
    output.writeStructBegin('user_management_remove_vichele_result');
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
user_managementClient = class user_managementClient {
  constructor(input, output) {
    this.input = input;
    this.output = (!output) ? input : output;
    this.seqid = 0;
  }

  get_user_info (ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_user_info(ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_user_info (ssid, callback) {
    const params = {
      ssid: ssid
    };
    const args = new user_management_get_user_info_args(params);
    try {
      this.output.writeMessageBegin('get_user_info', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_user_info();
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

  recv_get_user_info () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new user_management_get_user_info_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'get_user_info failed: unknown result';
  }

  user_login (code) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_user_login(code, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_user_login (code, callback) {
    const params = {
      code: code
    };
    const args = new user_management_user_login_args(params);
    try {
      this.output.writeMessageBegin('user_login', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_user_login();
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

  recv_user_login () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new user_management_user_login_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'user_login failed: unknown result';
  }

  update_user_info (info, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_update_user_info(info, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_update_user_info (info, ssid, callback) {
    const params = {
      info: info,
      ssid: ssid
    };
    const args = new user_management_update_user_info_args(params);
    try {
      this.output.writeMessageBegin('update_user_info', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_update_user_info();
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

  recv_update_user_info () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new user_management_update_user_info_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'update_user_info failed: unknown result';
  }

  remove_user (user_id, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_remove_user(user_id, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_remove_user (user_id, ssid, callback) {
    const params = {
      user_id: user_id,
      ssid: ssid
    };
    const args = new user_management_remove_user_args(params);
    try {
      this.output.writeMessageBegin('remove_user', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_remove_user();
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

  recv_remove_user () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new user_management_remove_user_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'remove_user failed: unknown result';
  }

  get_bound_vichele (ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_bound_vichele(ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_bound_vichele (ssid, callback) {
    const params = {
      ssid: ssid
    };
    const args = new user_management_get_bound_vichele_args(params);
    try {
      this.output.writeMessageBegin('get_bound_vichele', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_bound_vichele();
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

  recv_get_bound_vichele () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new user_management_get_bound_vichele_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'get_bound_vichele failed: unknown result';
  }

  bind_new_vichele (ssid, vichele) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_bind_new_vichele(ssid, vichele, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_bind_new_vichele (ssid, vichele, callback) {
    const params = {
      ssid: ssid,
      vichele: vichele
    };
    const args = new user_management_bind_new_vichele_args(params);
    try {
      this.output.writeMessageBegin('bind_new_vichele', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_bind_new_vichele();
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

  recv_bind_new_vichele () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new user_management_bind_new_vichele_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'bind_new_vichele failed: unknown result';
  }

  remove_vichele (ssid, vichele) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_remove_vichele(ssid, vichele, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_remove_vichele (ssid, vichele, callback) {
    const params = {
      ssid: ssid,
      vichele: vichele
    };
    const args = new user_management_remove_vichele_args(params);
    try {
      this.output.writeMessageBegin('remove_vichele', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_remove_vichele();
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

  recv_remove_vichele () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new user_management_remove_vichele_result();
    result.read(this.input);
    this.input.readMessageEnd();

    return;
  }
};
