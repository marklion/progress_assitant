//
// Autogenerated by Thrift Compiler (0.14.1)
//
// DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
//
if (typeof Int64 === 'undefined' && typeof require === 'function') {
  const Int64 = require('node-int64');
}


//HELPER FUNCTIONS AND STRUCTURES

stuff_plan_management_create_plan_args = class {
  constructor(args) {
    this.plan = null;
    this.ssid = null;
    if (args) {
      if (args.plan !== undefined && args.plan !== null) {
        this.plan = new stuff_plan(args.plan);
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
          this.plan = new stuff_plan();
          this.plan.read(input);
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
    output.writeStructBegin('stuff_plan_management_create_plan_args');
    if (this.plan !== null && this.plan !== undefined) {
      output.writeFieldBegin('plan', Thrift.Type.STRUCT, 1);
      this.plan.write(output);
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
stuff_plan_management_create_plan_result = class {
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
        if (ftype == Thrift.Type.I64) {
          this.success = input.readI64().value;
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
    output.writeStructBegin('stuff_plan_management_create_plan_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.I64, 0);
      output.writeI64(this.success);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_plan_management_get_created_plan_args = class {
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
    output.writeStructBegin('stuff_plan_management_get_created_plan_args');
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
stuff_plan_management_get_created_plan_result = class {
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
          const _rtmp321 = input.readListBegin();
          const _size20 = _rtmp321.size || 0;
          for (let _i22 = 0; _i22 < _size20; ++_i22) {
            let elem23 = null;
            elem23 = input.readI64().value;
            this.success.push(elem23);
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
    output.writeStructBegin('stuff_plan_management_get_created_plan_result');
    if (this.success !== null && this.success !== undefined) {
      output.writeFieldBegin('success', Thrift.Type.LIST, 0);
      output.writeListBegin(Thrift.Type.I64, this.success.length);
      for (let iter24 in this.success) {
        if (this.success.hasOwnProperty(iter24)) {
          iter24 = this.success[iter24];
          output.writeI64(iter24);
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
stuff_plan_management_get_plan_args = class {
  constructor(args) {
    this.plan_id = null;
    if (args) {
      if (args.plan_id !== undefined && args.plan_id !== null) {
        this.plan_id = args.plan_id;
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
    output.writeStructBegin('stuff_plan_management_get_plan_args');
    if (this.plan_id !== null && this.plan_id !== undefined) {
      output.writeFieldBegin('plan_id', Thrift.Type.I64, 1);
      output.writeI64(this.plan_id);
      output.writeFieldEnd();
    }
    output.writeFieldStop();
    output.writeStructEnd();
    return;
  }

};
stuff_plan_management_get_plan_result = class {
  constructor(args) {
    this.success = null;
    if (args) {
      if (args.success !== undefined && args.success !== null) {
        this.success = new stuff_plan(args.success);
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
          this.success = new stuff_plan();
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
    output.writeStructBegin('stuff_plan_management_get_plan_result');
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
stuff_plan_management_update_plan_args = class {
  constructor(args) {
    this.plan = null;
    this.ssid = null;
    if (args) {
      if (args.plan !== undefined && args.plan !== null) {
        this.plan = new stuff_plan(args.plan);
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
          this.plan = new stuff_plan();
          this.plan.read(input);
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
    output.writeStructBegin('stuff_plan_management_update_plan_args');
    if (this.plan !== null && this.plan !== undefined) {
      output.writeFieldBegin('plan', Thrift.Type.STRUCT, 1);
      this.plan.write(output);
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
stuff_plan_management_update_plan_result = class {
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
    output.writeStructBegin('stuff_plan_management_update_plan_result');
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
stuff_plan_managementClient = class stuff_plan_managementClient {
  constructor(input, output) {
    this.input = input;
    this.output = (!output) ? input : output;
    this.seqid = 0;
  }

  create_plan (plan, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_create_plan(plan, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_create_plan (plan, ssid, callback) {
    const params = {
      plan: plan,
      ssid: ssid
    };
    const args = new stuff_plan_management_create_plan_args(params);
    try {
      this.output.writeMessageBegin('create_plan', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_create_plan();
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

  recv_create_plan () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_plan_management_create_plan_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'create_plan failed: unknown result';
  }

  get_created_plan (ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_created_plan(ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_created_plan (ssid, callback) {
    const params = {
      ssid: ssid
    };
    const args = new stuff_plan_management_get_created_plan_args(params);
    try {
      this.output.writeMessageBegin('get_created_plan', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_created_plan();
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

  recv_get_created_plan () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_plan_management_get_created_plan_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'get_created_plan failed: unknown result';
  }

  get_plan (plan_id) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_get_plan(plan_id, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_get_plan (plan_id, callback) {
    const params = {
      plan_id: plan_id
    };
    const args = new stuff_plan_management_get_plan_args(params);
    try {
      this.output.writeMessageBegin('get_plan', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_get_plan();
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

  recv_get_plan () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_plan_management_get_plan_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'get_plan failed: unknown result';
  }

  update_plan (plan, ssid) {
    const self = this;
    return new Promise((resolve, reject) => {
      self.send_update_plan(plan, ssid, (error, result) => {
        return error ? reject(error) : resolve(result);
      });
    });
  }

  send_update_plan (plan, ssid, callback) {
    const params = {
      plan: plan,
      ssid: ssid
    };
    const args = new stuff_plan_management_update_plan_args(params);
    try {
      this.output.writeMessageBegin('update_plan', Thrift.MessageType.CALL, this.seqid);
      args.write(this.output);
      this.output.writeMessageEnd();
      const self = this;
      this.output.getTransport().flush(true, () => {
        let error = null, result = null;
        try {
          result = self.recv_update_plan();
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

  recv_update_plan () {
    const ret = this.input.readMessageBegin();
    const mtype = ret.mtype;
    if (mtype == Thrift.MessageType.EXCEPTION) {
      const x = new Thrift.TApplicationException();
      x.read(this.input);
      this.input.readMessageEnd();
      throw x;
    }
    const result = new stuff_plan_management_update_plan_result();
    result.read(this.input);
    this.input.readMessageEnd();

    if (null !== result.success) {
      return result.success;
    }
    throw 'update_plan failed: unknown result';
  }
};
