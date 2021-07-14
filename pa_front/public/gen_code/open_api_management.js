//
// Autogenerated by Thrift Compiler (0.14.1)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//
if (typeof Int64 === 'undefined' && typeof require === 'function') {
  const Int64 = require('node-int64');
}


//HELPER FUNCTIONS AND STRUCTURES

open_api_management_register_api_user_args = class {
  constructor(args) {
    this.company_name = null;
    this.email = null;
    this.password = null;
    if (args) {
      if (args.company_name !== undefined && args.company_name !== null) {
        this.company_name = args.company_name;
      }
      if (args.email !== undefined && args.email !== null) {
        this.email = args.email;
      }
      if (args.password !== undefined && args.password !== null) {
        this.password = args.password;
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
        if (ftype == Thrift.Type.STRING) {
          this.email = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 3:
        if (ftype == Thrift.Type.STRING) {
          this.password = input.readString().value;
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
    output.writeStructBegin('open_api_management_register_api_user_args');
    if (this.company_name !== null && this.company_name !== undefined) {
      output.writeFieldBegin('company_name', Thrift.Type.STRING, 1);
      output.writeString(this.company_name);
      output.writeFieldEnd();
    }
    if (this.email !== null && this.email !== undefined) {
      output.writeFieldBegin('email', Thrift.Type.STRING, 2);
      output.writeString(this.email);
      output.writeFieldEnd();
    }
    if (this.password !== null && this.password !== undefined) {
      output.writeFieldBegin('password', Thrift.Type.STRING, 3);
      output.writeString(this.password);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
open_api_management_register_api_user_result = class {
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
    output.writeStructBegin('open_api_management_register_api_user_result');
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
open_api_management_verify_email_code_args = class {
  constructor(args) {
    this.email = null;
    this.code = null;
    if (args) {
      if (args.email !== undefined && args.email !== null) {
        this.email = args.email;
      }
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
          this.email = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.code = input.readString().value;
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
    output.writeStructBegin('open_api_management_verify_email_code_args');
    if (this.email !== null && this.email !== undefined) {
      output.writeFieldBegin('email', Thrift.Type.STRING, 1);
      output.writeString(this.email);
      output.writeFieldEnd();
    }
    if (this.code !== null && this.code !== undefined) {
      output.writeFieldBegin('code', Thrift.Type.STRING, 2);
      output.writeString(this.code);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
open_api_management_verify_email_code_result = class {
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
    output.writeStructBegin('open_api_management_verify_email_code_result');
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
open_api_management_unregister_api_user_args = class {
  constructor(args) {
    this.email = null;
    this.password = null;
    if (args) {
      if (args.email !== undefined && args.email !== null) {
        this.email = args.email;
      }
      if (args.password !== undefined && args.password !== null) {
        this.password = args.password;
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
          this.email = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.password = input.readString().value;
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
    output.writeStructBegin('open_api_management_unregister_api_user_args');
    if (this.email !== null && this.email !== undefined) {
      output.writeFieldBegin('email', Thrift.Type.STRING, 1);
      output.writeString(this.email);
      output.writeFieldEnd();
    }
    if (this.password !== null && this.password !== undefined) {
      output.writeFieldBegin('password', Thrift.Type.STRING, 2);
      output.writeString(this.password);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
open_api_management_unregister_api_user_result = class {
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
    output.writeStructBegin('open_api_management_unregister_api_user_result');
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
open_api_management_get_token_args = class {
  constructor(args) {
    this.email = null;
    this.password = null;
    if (args) {
      if (args.email !== undefined && args.email !== null) {
        this.email = args.email;
      }
      if (args.password !== undefined && args.password !== null) {
        this.password = args.password;
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
          this.email = input.readString().value;
        } else {
          input.skip(ftype);
        }
        break;
        case 2:
        if (ftype == Thrift.Type.STRING) {
          this.password = input.readString().value;
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
    output.writeStructBegin('open_api_management_get_token_args');
    if (this.email !== null && this.email !== undefined) {
      output.writeFieldBegin('email', Thrift.Type.STRING, 1);
      output.writeString(this.email);
      output.writeFieldEnd();
    }
    if (this.password !== null && this.password !== undefined) {
      output.writeFieldBegin('password', Thrift.Type.STRING, 2);
      output.writeString(this.password);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
open_api_management_get_token_result = class {
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
        if (ftype == Thrift.Type.STRING) {
          this.success = input.readString().value;
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
    output.writeStructBegin('open_api_management_get_token_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.STRING, 0);
      output.writeString(this.success);
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
open_api_management_get_today_transformation_args = class {
  constructor(args) {
    this.token = null;
    if (args) {
      if (args.token !== undefined && args.token !== null) {
        this.token = args.token;
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
          this.token = input.readString().value;
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
    output.writeStructBegin('open_api_management_get_today_transformation_args');
    if (this.token !== null && this.token !== undefined) {
      output.writeFieldBegin('token', Thrift.Type.STRING, 1);
      output.writeString(this.token);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
open_api_management_get_today_transformation_result = class {
  constructor(args) {
    this.success = null;
    this.e = null;
    if (args instanceof gen_exp) {
        this.e = args;
        return;
    }
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = Thrift.copyList(args.success, [api_transformation_info]);
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
          const _rtmp3111 = input.readListBegin();
          const _size110 = _rtmp3111.size || 0;
          for (let _i112 = 0; _i112 < _size110; ++_i112) {
            let elem113 = null;
            elem113 = new api_transformation_info();
            elem113.read(input);
            this.success.push(elem113);
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
    output.writeStructBegin('open_api_management_get_today_transformation_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.LIST, 0);
      output.writeListBegin(Thrift.Type.STRUCT, this.success.length);
      for (let iter114 in this.success) {
        if (this.success.hasOwnProperty(iter114)) {
          iter114 = this.success[iter114];
          iter114.write(output);
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
open_api_managementClient = class open_api_managementClient {
  constructor(input, output) {
    this.input = input;
    this.output = (!output) ? input : output;
    this.seqid = 0;
  }

  register_api_user (company_name, email, password) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_register_api_user(company_name, email, password, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_register_api_user (company_name, email, password, callback) {
    const params = {
      company_name: company_name,
      email: email,
      password: password
    };
    const args = new open_api_management_register_api_user_args(params);
    try {
      this.output.writeMessageBegin('register_api_user', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_register_api_user();
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

  recv_register_api_user () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new open_api_management_register_api_user_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'register_api_user failed: unknown result';
  }

  verify_email_code (email, code) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_verify_email_code(email, code, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_verify_email_code (email, code, callback) {
    const params = {
      email: email,
      code: code
    };
    const args = new open_api_management_verify_email_code_args(params);
    try {
      this.output.writeMessageBegin('verify_email_code', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_verify_email_code();
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

  recv_verify_email_code () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new open_api_management_verify_email_code_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'verify_email_code failed: unknown result';
  }

  unregister_api_user (email, password) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_unregister_api_user(email, password, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_unregister_api_user (email, password, callback) {
    const params = {
      email: email,
      password: password
    };
    const args = new open_api_management_unregister_api_user_args(params);
    try {
      this.output.writeMessageBegin('unregister_api_user', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_unregister_api_user();
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

  recv_unregister_api_user () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new open_api_management_unregister_api_user_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'unregister_api_user failed: unknown result';
  }

  get_token (email, password) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_token(email, password, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_token (email, password, callback) {
    const params = {
      email: email,
      password: password
    };
    const args = new open_api_management_get_token_args(params);
    try {
      this.output.writeMessageBegin('get_token', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_token();
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

  recv_get_token () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new open_api_management_get_token_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'get_token failed: unknown result';
  }

  get_today_transformation (token) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_today_transformation(token, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_today_transformation (token, callback) {
    const params = {
      token: token
    };
    const args = new open_api_management_get_today_transformation_args(params);
    try {
      this.output.writeMessageBegin('get_today_transformation', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_today_transformation();
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

  recv_get_today_transformation () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new open_api_management_get_today_transformation_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.e) {
      throw result.e;
    }
    if (null !== result.success) {
      return result.success;
    }
    throw 'get_today_transformation failed: unknown result';
  }
};
