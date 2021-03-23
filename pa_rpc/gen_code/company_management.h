/**
 * Autogenerated by Thrift Compiler (0.14.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef company_management_H
#define company_management_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "idl_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class company_managementIf {
 public:
  virtual ~company_managementIf() {}
  virtual void get_all_type(std::vector<int64_t> & _return, const std::string& ssid) = 0;
  virtual int64_t add_type(const std::string& name, const int64_t price, const std::string& ssid) = 0;
  virtual bool edit_type(const stuff_detail& stuff, const std::string& ssid) = 0;
  virtual void remove_type(const stuff_detail& stuff, const std::string& ssid) = 0;
  virtual void get_all_apply(std::vector<user_apply> & _return, const std::string& ssid) = 0;
  virtual bool approve_apply(const int64_t apply_id, const std::string& ssid, const bool approve) = 0;
};

class company_managementIfFactory {
 public:
  typedef company_managementIf Handler;

  virtual ~company_managementIfFactory() {}

  virtual company_managementIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(company_managementIf* /* handler */) = 0;
};

class company_managementIfSingletonFactory : virtual public company_managementIfFactory {
 public:
  company_managementIfSingletonFactory(const ::std::shared_ptr<company_managementIf>& iface) : iface_(iface) {}
  virtual ~company_managementIfSingletonFactory() {}

  virtual company_managementIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(company_managementIf* /* handler */) {}

 protected:
  ::std::shared_ptr<company_managementIf> iface_;
};

class company_managementNull : virtual public company_managementIf {
 public:
  virtual ~company_managementNull() {}
  void get_all_type(std::vector<int64_t> & /* _return */, const std::string& /* ssid */) {
    return;
  }
  int64_t add_type(const std::string& /* name */, const int64_t /* price */, const std::string& /* ssid */) {
    int64_t _return = 0;
    return _return;
  }
  bool edit_type(const stuff_detail& /* stuff */, const std::string& /* ssid */) {
    bool _return = false;
    return _return;
  }
  void remove_type(const stuff_detail& /* stuff */, const std::string& /* ssid */) {
    return;
  }
  void get_all_apply(std::vector<user_apply> & /* _return */, const std::string& /* ssid */) {
    return;
  }
  bool approve_apply(const int64_t /* apply_id */, const std::string& /* ssid */, const bool /* approve */) {
    bool _return = false;
    return _return;
  }
};

typedef struct _company_management_get_all_type_args__isset {
  _company_management_get_all_type_args__isset() : ssid(false) {}
  bool ssid :1;
} _company_management_get_all_type_args__isset;

class company_management_get_all_type_args {
 public:

  company_management_get_all_type_args(const company_management_get_all_type_args&);
  company_management_get_all_type_args& operator=(const company_management_get_all_type_args&);
  company_management_get_all_type_args() : ssid() {
  }

  virtual ~company_management_get_all_type_args() noexcept;
  std::string ssid;

  _company_management_get_all_type_args__isset __isset;

  void __set_ssid(const std::string& val);

  bool operator == (const company_management_get_all_type_args & rhs) const
  {
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const company_management_get_all_type_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_get_all_type_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_get_all_type_pargs {
 public:


  virtual ~company_management_get_all_type_pargs() noexcept;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_get_all_type_result__isset {
  _company_management_get_all_type_result__isset() : success(false) {}
  bool success :1;
} _company_management_get_all_type_result__isset;

class company_management_get_all_type_result {
 public:

  company_management_get_all_type_result(const company_management_get_all_type_result&);
  company_management_get_all_type_result& operator=(const company_management_get_all_type_result&);
  company_management_get_all_type_result() {
  }

  virtual ~company_management_get_all_type_result() noexcept;
  std::vector<int64_t>  success;

  _company_management_get_all_type_result__isset __isset;

  void __set_success(const std::vector<int64_t> & val);

  bool operator == (const company_management_get_all_type_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const company_management_get_all_type_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_get_all_type_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_get_all_type_presult__isset {
  _company_management_get_all_type_presult__isset() : success(false) {}
  bool success :1;
} _company_management_get_all_type_presult__isset;

class company_management_get_all_type_presult {
 public:


  virtual ~company_management_get_all_type_presult() noexcept;
  std::vector<int64_t> * success;

  _company_management_get_all_type_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _company_management_add_type_args__isset {
  _company_management_add_type_args__isset() : name(false), price(false), ssid(false) {}
  bool name :1;
  bool price :1;
  bool ssid :1;
} _company_management_add_type_args__isset;

class company_management_add_type_args {
 public:

  company_management_add_type_args(const company_management_add_type_args&);
  company_management_add_type_args& operator=(const company_management_add_type_args&);
  company_management_add_type_args() : name(), price(0), ssid() {
  }

  virtual ~company_management_add_type_args() noexcept;
  std::string name;
  int64_t price;
  std::string ssid;

  _company_management_add_type_args__isset __isset;

  void __set_name(const std::string& val);

  void __set_price(const int64_t val);

  void __set_ssid(const std::string& val);

  bool operator == (const company_management_add_type_args & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(price == rhs.price))
      return false;
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const company_management_add_type_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_add_type_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_add_type_pargs {
 public:


  virtual ~company_management_add_type_pargs() noexcept;
  const std::string* name;
  const int64_t* price;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_add_type_result__isset {
  _company_management_add_type_result__isset() : success(false) {}
  bool success :1;
} _company_management_add_type_result__isset;

class company_management_add_type_result {
 public:

  company_management_add_type_result(const company_management_add_type_result&);
  company_management_add_type_result& operator=(const company_management_add_type_result&);
  company_management_add_type_result() : success(0) {
  }

  virtual ~company_management_add_type_result() noexcept;
  int64_t success;

  _company_management_add_type_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const company_management_add_type_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const company_management_add_type_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_add_type_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_add_type_presult__isset {
  _company_management_add_type_presult__isset() : success(false) {}
  bool success :1;
} _company_management_add_type_presult__isset;

class company_management_add_type_presult {
 public:


  virtual ~company_management_add_type_presult() noexcept;
  int64_t* success;

  _company_management_add_type_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _company_management_edit_type_args__isset {
  _company_management_edit_type_args__isset() : stuff(false), ssid(false) {}
  bool stuff :1;
  bool ssid :1;
} _company_management_edit_type_args__isset;

class company_management_edit_type_args {
 public:

  company_management_edit_type_args(const company_management_edit_type_args&);
  company_management_edit_type_args& operator=(const company_management_edit_type_args&);
  company_management_edit_type_args() : ssid() {
  }

  virtual ~company_management_edit_type_args() noexcept;
  stuff_detail stuff;
  std::string ssid;

  _company_management_edit_type_args__isset __isset;

  void __set_stuff(const stuff_detail& val);

  void __set_ssid(const std::string& val);

  bool operator == (const company_management_edit_type_args & rhs) const
  {
    if (!(stuff == rhs.stuff))
      return false;
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const company_management_edit_type_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_edit_type_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_edit_type_pargs {
 public:


  virtual ~company_management_edit_type_pargs() noexcept;
  const stuff_detail* stuff;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_edit_type_result__isset {
  _company_management_edit_type_result__isset() : success(false) {}
  bool success :1;
} _company_management_edit_type_result__isset;

class company_management_edit_type_result {
 public:

  company_management_edit_type_result(const company_management_edit_type_result&);
  company_management_edit_type_result& operator=(const company_management_edit_type_result&);
  company_management_edit_type_result() : success(0) {
  }

  virtual ~company_management_edit_type_result() noexcept;
  bool success;

  _company_management_edit_type_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const company_management_edit_type_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const company_management_edit_type_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_edit_type_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_edit_type_presult__isset {
  _company_management_edit_type_presult__isset() : success(false) {}
  bool success :1;
} _company_management_edit_type_presult__isset;

class company_management_edit_type_presult {
 public:


  virtual ~company_management_edit_type_presult() noexcept;
  bool* success;

  _company_management_edit_type_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _company_management_remove_type_args__isset {
  _company_management_remove_type_args__isset() : stuff(false), ssid(false) {}
  bool stuff :1;
  bool ssid :1;
} _company_management_remove_type_args__isset;

class company_management_remove_type_args {
 public:

  company_management_remove_type_args(const company_management_remove_type_args&);
  company_management_remove_type_args& operator=(const company_management_remove_type_args&);
  company_management_remove_type_args() : ssid() {
  }

  virtual ~company_management_remove_type_args() noexcept;
  stuff_detail stuff;
  std::string ssid;

  _company_management_remove_type_args__isset __isset;

  void __set_stuff(const stuff_detail& val);

  void __set_ssid(const std::string& val);

  bool operator == (const company_management_remove_type_args & rhs) const
  {
    if (!(stuff == rhs.stuff))
      return false;
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const company_management_remove_type_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_remove_type_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_remove_type_pargs {
 public:


  virtual ~company_management_remove_type_pargs() noexcept;
  const stuff_detail* stuff;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_remove_type_result {
 public:

  company_management_remove_type_result(const company_management_remove_type_result&);
  company_management_remove_type_result& operator=(const company_management_remove_type_result&);
  company_management_remove_type_result() {
  }

  virtual ~company_management_remove_type_result() noexcept;

  bool operator == (const company_management_remove_type_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const company_management_remove_type_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_remove_type_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_remove_type_presult {
 public:


  virtual ~company_management_remove_type_presult() noexcept;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _company_management_get_all_apply_args__isset {
  _company_management_get_all_apply_args__isset() : ssid(false) {}
  bool ssid :1;
} _company_management_get_all_apply_args__isset;

class company_management_get_all_apply_args {
 public:

  company_management_get_all_apply_args(const company_management_get_all_apply_args&);
  company_management_get_all_apply_args& operator=(const company_management_get_all_apply_args&);
  company_management_get_all_apply_args() : ssid() {
  }

  virtual ~company_management_get_all_apply_args() noexcept;
  std::string ssid;

  _company_management_get_all_apply_args__isset __isset;

  void __set_ssid(const std::string& val);

  bool operator == (const company_management_get_all_apply_args & rhs) const
  {
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const company_management_get_all_apply_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_get_all_apply_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_get_all_apply_pargs {
 public:


  virtual ~company_management_get_all_apply_pargs() noexcept;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_get_all_apply_result__isset {
  _company_management_get_all_apply_result__isset() : success(false) {}
  bool success :1;
} _company_management_get_all_apply_result__isset;

class company_management_get_all_apply_result {
 public:

  company_management_get_all_apply_result(const company_management_get_all_apply_result&);
  company_management_get_all_apply_result& operator=(const company_management_get_all_apply_result&);
  company_management_get_all_apply_result() {
  }

  virtual ~company_management_get_all_apply_result() noexcept;
  std::vector<user_apply>  success;

  _company_management_get_all_apply_result__isset __isset;

  void __set_success(const std::vector<user_apply> & val);

  bool operator == (const company_management_get_all_apply_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const company_management_get_all_apply_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_get_all_apply_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_get_all_apply_presult__isset {
  _company_management_get_all_apply_presult__isset() : success(false) {}
  bool success :1;
} _company_management_get_all_apply_presult__isset;

class company_management_get_all_apply_presult {
 public:


  virtual ~company_management_get_all_apply_presult() noexcept;
  std::vector<user_apply> * success;

  _company_management_get_all_apply_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _company_management_approve_apply_args__isset {
  _company_management_approve_apply_args__isset() : apply_id(false), ssid(false), approve(false) {}
  bool apply_id :1;
  bool ssid :1;
  bool approve :1;
} _company_management_approve_apply_args__isset;

class company_management_approve_apply_args {
 public:

  company_management_approve_apply_args(const company_management_approve_apply_args&);
  company_management_approve_apply_args& operator=(const company_management_approve_apply_args&);
  company_management_approve_apply_args() : apply_id(0), ssid(), approve(0) {
  }

  virtual ~company_management_approve_apply_args() noexcept;
  int64_t apply_id;
  std::string ssid;
  bool approve;

  _company_management_approve_apply_args__isset __isset;

  void __set_apply_id(const int64_t val);

  void __set_ssid(const std::string& val);

  void __set_approve(const bool val);

  bool operator == (const company_management_approve_apply_args & rhs) const
  {
    if (!(apply_id == rhs.apply_id))
      return false;
    if (!(ssid == rhs.ssid))
      return false;
    if (!(approve == rhs.approve))
      return false;
    return true;
  }
  bool operator != (const company_management_approve_apply_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_approve_apply_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class company_management_approve_apply_pargs {
 public:


  virtual ~company_management_approve_apply_pargs() noexcept;
  const int64_t* apply_id;
  const std::string* ssid;
  const bool* approve;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_approve_apply_result__isset {
  _company_management_approve_apply_result__isset() : success(false) {}
  bool success :1;
} _company_management_approve_apply_result__isset;

class company_management_approve_apply_result {
 public:

  company_management_approve_apply_result(const company_management_approve_apply_result&);
  company_management_approve_apply_result& operator=(const company_management_approve_apply_result&);
  company_management_approve_apply_result() : success(0) {
  }

  virtual ~company_management_approve_apply_result() noexcept;
  bool success;

  _company_management_approve_apply_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const company_management_approve_apply_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const company_management_approve_apply_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_management_approve_apply_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _company_management_approve_apply_presult__isset {
  _company_management_approve_apply_presult__isset() : success(false) {}
  bool success :1;
} _company_management_approve_apply_presult__isset;

class company_management_approve_apply_presult {
 public:


  virtual ~company_management_approve_apply_presult() noexcept;
  bool* success;

  _company_management_approve_apply_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class company_managementClient : virtual public company_managementIf {
 public:
  company_managementClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  company_managementClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void get_all_type(std::vector<int64_t> & _return, const std::string& ssid);
  void send_get_all_type(const std::string& ssid);
  void recv_get_all_type(std::vector<int64_t> & _return);
  int64_t add_type(const std::string& name, const int64_t price, const std::string& ssid);
  void send_add_type(const std::string& name, const int64_t price, const std::string& ssid);
  int64_t recv_add_type();
  bool edit_type(const stuff_detail& stuff, const std::string& ssid);
  void send_edit_type(const stuff_detail& stuff, const std::string& ssid);
  bool recv_edit_type();
  void remove_type(const stuff_detail& stuff, const std::string& ssid);
  void send_remove_type(const stuff_detail& stuff, const std::string& ssid);
  void recv_remove_type();
  void get_all_apply(std::vector<user_apply> & _return, const std::string& ssid);
  void send_get_all_apply(const std::string& ssid);
  void recv_get_all_apply(std::vector<user_apply> & _return);
  bool approve_apply(const int64_t apply_id, const std::string& ssid, const bool approve);
  void send_approve_apply(const int64_t apply_id, const std::string& ssid, const bool approve);
  bool recv_approve_apply();
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class company_managementProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<company_managementIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (company_managementProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_get_all_type(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_add_type(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_edit_type(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_remove_type(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_get_all_apply(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_approve_apply(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  company_managementProcessor(::std::shared_ptr<company_managementIf> iface) :
    iface_(iface) {
    processMap_["get_all_type"] = &company_managementProcessor::process_get_all_type;
    processMap_["add_type"] = &company_managementProcessor::process_add_type;
    processMap_["edit_type"] = &company_managementProcessor::process_edit_type;
    processMap_["remove_type"] = &company_managementProcessor::process_remove_type;
    processMap_["get_all_apply"] = &company_managementProcessor::process_get_all_apply;
    processMap_["approve_apply"] = &company_managementProcessor::process_approve_apply;
  }

  virtual ~company_managementProcessor() {}
};

class company_managementProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  company_managementProcessorFactory(const ::std::shared_ptr< company_managementIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< company_managementIfFactory > handlerFactory_;
};

class company_managementMultiface : virtual public company_managementIf {
 public:
  company_managementMultiface(std::vector<std::shared_ptr<company_managementIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~company_managementMultiface() {}
 protected:
  std::vector<std::shared_ptr<company_managementIf> > ifaces_;
  company_managementMultiface() {}
  void add(::std::shared_ptr<company_managementIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void get_all_type(std::vector<int64_t> & _return, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_all_type(_return, ssid);
    }
    ifaces_[i]->get_all_type(_return, ssid);
    return;
  }

  int64_t add_type(const std::string& name, const int64_t price, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->add_type(name, price, ssid);
    }
    return ifaces_[i]->add_type(name, price, ssid);
  }

  bool edit_type(const stuff_detail& stuff, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->edit_type(stuff, ssid);
    }
    return ifaces_[i]->edit_type(stuff, ssid);
  }

  void remove_type(const stuff_detail& stuff, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->remove_type(stuff, ssid);
    }
    ifaces_[i]->remove_type(stuff, ssid);
  }

  void get_all_apply(std::vector<user_apply> & _return, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_all_apply(_return, ssid);
    }
    ifaces_[i]->get_all_apply(_return, ssid);
    return;
  }

  bool approve_apply(const int64_t apply_id, const std::string& ssid, const bool approve) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->approve_apply(apply_id, ssid, approve);
    }
    return ifaces_[i]->approve_apply(apply_id, ssid, approve);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class company_managementConcurrentClient : virtual public company_managementIf {
 public:
  company_managementConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  company_managementConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void get_all_type(std::vector<int64_t> & _return, const std::string& ssid);
  int32_t send_get_all_type(const std::string& ssid);
  void recv_get_all_type(std::vector<int64_t> & _return, const int32_t seqid);
  int64_t add_type(const std::string& name, const int64_t price, const std::string& ssid);
  int32_t send_add_type(const std::string& name, const int64_t price, const std::string& ssid);
  int64_t recv_add_type(const int32_t seqid);
  bool edit_type(const stuff_detail& stuff, const std::string& ssid);
  int32_t send_edit_type(const stuff_detail& stuff, const std::string& ssid);
  bool recv_edit_type(const int32_t seqid);
  void remove_type(const stuff_detail& stuff, const std::string& ssid);
  int32_t send_remove_type(const stuff_detail& stuff, const std::string& ssid);
  void recv_remove_type(const int32_t seqid);
  void get_all_apply(std::vector<user_apply> & _return, const std::string& ssid);
  int32_t send_get_all_apply(const std::string& ssid);
  void recv_get_all_apply(std::vector<user_apply> & _return, const int32_t seqid);
  bool approve_apply(const int64_t apply_id, const std::string& ssid, const bool approve);
  int32_t send_approve_apply(const int64_t apply_id, const std::string& ssid, const bool approve);
  bool recv_approve_apply(const int32_t seqid);
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif



#endif
