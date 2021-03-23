/**
 * Autogenerated by Thrift Compiler (0.14.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef stuff_plan_management_H
#define stuff_plan_management_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "idl_types.h"



#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class stuff_plan_managementIf {
 public:
  virtual ~stuff_plan_managementIf() {}
  virtual int64_t create_plan(const stuff_plan& plan, const std::string& ssid) = 0;
  virtual void get_created_plan(std::vector<int64_t> & _return, const std::string& ssid) = 0;
  virtual void get_company_plan(std::vector<int64_t> & _return, const std::string& ssid) = 0;
  virtual void get_plan(stuff_plan& _return, const int64_t plan_id) = 0;
  virtual bool update_plan(const stuff_plan& plan, const std::string& ssid) = 0;
  virtual bool confirm_plan(const int64_t plan_id, const std::string& ssid, const bool confirm) = 0;
};

class stuff_plan_managementIfFactory {
 public:
  typedef stuff_plan_managementIf Handler;

  virtual ~stuff_plan_managementIfFactory() {}

  virtual stuff_plan_managementIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(stuff_plan_managementIf* /* handler */) = 0;
};

class stuff_plan_managementIfSingletonFactory : virtual public stuff_plan_managementIfFactory {
 public:
  stuff_plan_managementIfSingletonFactory(const ::std::shared_ptr<stuff_plan_managementIf>& iface) : iface_(iface) {}
  virtual ~stuff_plan_managementIfSingletonFactory() {}

  virtual stuff_plan_managementIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(stuff_plan_managementIf* /* handler */) {}

 protected:
  ::std::shared_ptr<stuff_plan_managementIf> iface_;
};

class stuff_plan_managementNull : virtual public stuff_plan_managementIf {
 public:
  virtual ~stuff_plan_managementNull() {}
  int64_t create_plan(const stuff_plan& /* plan */, const std::string& /* ssid */) {
    int64_t _return = 0;
    return _return;
  }
  void get_created_plan(std::vector<int64_t> & /* _return */, const std::string& /* ssid */) {
    return;
  }
  void get_company_plan(std::vector<int64_t> & /* _return */, const std::string& /* ssid */) {
    return;
  }
  void get_plan(stuff_plan& /* _return */, const int64_t /* plan_id */) {
    return;
  }
  bool update_plan(const stuff_plan& /* plan */, const std::string& /* ssid */) {
    bool _return = false;
    return _return;
  }
  bool confirm_plan(const int64_t /* plan_id */, const std::string& /* ssid */, const bool /* confirm */) {
    bool _return = false;
    return _return;
  }
};

typedef struct _stuff_plan_management_create_plan_args__isset {
  _stuff_plan_management_create_plan_args__isset() : plan(false), ssid(false) {}
  bool plan :1;
  bool ssid :1;
} _stuff_plan_management_create_plan_args__isset;

class stuff_plan_management_create_plan_args {
 public:

  stuff_plan_management_create_plan_args(const stuff_plan_management_create_plan_args&);
  stuff_plan_management_create_plan_args& operator=(const stuff_plan_management_create_plan_args&);
  stuff_plan_management_create_plan_args() : ssid() {
  }

  virtual ~stuff_plan_management_create_plan_args() noexcept;
  stuff_plan plan;
  std::string ssid;

  _stuff_plan_management_create_plan_args__isset __isset;

  void __set_plan(const stuff_plan& val);

  void __set_ssid(const std::string& val);

  bool operator == (const stuff_plan_management_create_plan_args & rhs) const
  {
    if (!(plan == rhs.plan))
      return false;
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_create_plan_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_create_plan_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class stuff_plan_management_create_plan_pargs {
 public:


  virtual ~stuff_plan_management_create_plan_pargs() noexcept;
  const stuff_plan* plan;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_create_plan_result__isset {
  _stuff_plan_management_create_plan_result__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_create_plan_result__isset;

class stuff_plan_management_create_plan_result {
 public:

  stuff_plan_management_create_plan_result(const stuff_plan_management_create_plan_result&);
  stuff_plan_management_create_plan_result& operator=(const stuff_plan_management_create_plan_result&);
  stuff_plan_management_create_plan_result() : success(0) {
  }

  virtual ~stuff_plan_management_create_plan_result() noexcept;
  int64_t success;

  _stuff_plan_management_create_plan_result__isset __isset;

  void __set_success(const int64_t val);

  bool operator == (const stuff_plan_management_create_plan_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_create_plan_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_create_plan_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_create_plan_presult__isset {
  _stuff_plan_management_create_plan_presult__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_create_plan_presult__isset;

class stuff_plan_management_create_plan_presult {
 public:


  virtual ~stuff_plan_management_create_plan_presult() noexcept;
  int64_t* success;

  _stuff_plan_management_create_plan_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _stuff_plan_management_get_created_plan_args__isset {
  _stuff_plan_management_get_created_plan_args__isset() : ssid(false) {}
  bool ssid :1;
} _stuff_plan_management_get_created_plan_args__isset;

class stuff_plan_management_get_created_plan_args {
 public:

  stuff_plan_management_get_created_plan_args(const stuff_plan_management_get_created_plan_args&);
  stuff_plan_management_get_created_plan_args& operator=(const stuff_plan_management_get_created_plan_args&);
  stuff_plan_management_get_created_plan_args() : ssid() {
  }

  virtual ~stuff_plan_management_get_created_plan_args() noexcept;
  std::string ssid;

  _stuff_plan_management_get_created_plan_args__isset __isset;

  void __set_ssid(const std::string& val);

  bool operator == (const stuff_plan_management_get_created_plan_args & rhs) const
  {
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_get_created_plan_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_get_created_plan_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class stuff_plan_management_get_created_plan_pargs {
 public:


  virtual ~stuff_plan_management_get_created_plan_pargs() noexcept;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_get_created_plan_result__isset {
  _stuff_plan_management_get_created_plan_result__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_get_created_plan_result__isset;

class stuff_plan_management_get_created_plan_result {
 public:

  stuff_plan_management_get_created_plan_result(const stuff_plan_management_get_created_plan_result&);
  stuff_plan_management_get_created_plan_result& operator=(const stuff_plan_management_get_created_plan_result&);
  stuff_plan_management_get_created_plan_result() {
  }

  virtual ~stuff_plan_management_get_created_plan_result() noexcept;
  std::vector<int64_t>  success;

  _stuff_plan_management_get_created_plan_result__isset __isset;

  void __set_success(const std::vector<int64_t> & val);

  bool operator == (const stuff_plan_management_get_created_plan_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_get_created_plan_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_get_created_plan_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_get_created_plan_presult__isset {
  _stuff_plan_management_get_created_plan_presult__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_get_created_plan_presult__isset;

class stuff_plan_management_get_created_plan_presult {
 public:


  virtual ~stuff_plan_management_get_created_plan_presult() noexcept;
  std::vector<int64_t> * success;

  _stuff_plan_management_get_created_plan_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _stuff_plan_management_get_company_plan_args__isset {
  _stuff_plan_management_get_company_plan_args__isset() : ssid(false) {}
  bool ssid :1;
} _stuff_plan_management_get_company_plan_args__isset;

class stuff_plan_management_get_company_plan_args {
 public:

  stuff_plan_management_get_company_plan_args(const stuff_plan_management_get_company_plan_args&);
  stuff_plan_management_get_company_plan_args& operator=(const stuff_plan_management_get_company_plan_args&);
  stuff_plan_management_get_company_plan_args() : ssid() {
  }

  virtual ~stuff_plan_management_get_company_plan_args() noexcept;
  std::string ssid;

  _stuff_plan_management_get_company_plan_args__isset __isset;

  void __set_ssid(const std::string& val);

  bool operator == (const stuff_plan_management_get_company_plan_args & rhs) const
  {
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_get_company_plan_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_get_company_plan_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class stuff_plan_management_get_company_plan_pargs {
 public:


  virtual ~stuff_plan_management_get_company_plan_pargs() noexcept;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_get_company_plan_result__isset {
  _stuff_plan_management_get_company_plan_result__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_get_company_plan_result__isset;

class stuff_plan_management_get_company_plan_result {
 public:

  stuff_plan_management_get_company_plan_result(const stuff_plan_management_get_company_plan_result&);
  stuff_plan_management_get_company_plan_result& operator=(const stuff_plan_management_get_company_plan_result&);
  stuff_plan_management_get_company_plan_result() {
  }

  virtual ~stuff_plan_management_get_company_plan_result() noexcept;
  std::vector<int64_t>  success;

  _stuff_plan_management_get_company_plan_result__isset __isset;

  void __set_success(const std::vector<int64_t> & val);

  bool operator == (const stuff_plan_management_get_company_plan_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_get_company_plan_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_get_company_plan_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_get_company_plan_presult__isset {
  _stuff_plan_management_get_company_plan_presult__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_get_company_plan_presult__isset;

class stuff_plan_management_get_company_plan_presult {
 public:


  virtual ~stuff_plan_management_get_company_plan_presult() noexcept;
  std::vector<int64_t> * success;

  _stuff_plan_management_get_company_plan_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _stuff_plan_management_get_plan_args__isset {
  _stuff_plan_management_get_plan_args__isset() : plan_id(false) {}
  bool plan_id :1;
} _stuff_plan_management_get_plan_args__isset;

class stuff_plan_management_get_plan_args {
 public:

  stuff_plan_management_get_plan_args(const stuff_plan_management_get_plan_args&);
  stuff_plan_management_get_plan_args& operator=(const stuff_plan_management_get_plan_args&);
  stuff_plan_management_get_plan_args() : plan_id(0) {
  }

  virtual ~stuff_plan_management_get_plan_args() noexcept;
  int64_t plan_id;

  _stuff_plan_management_get_plan_args__isset __isset;

  void __set_plan_id(const int64_t val);

  bool operator == (const stuff_plan_management_get_plan_args & rhs) const
  {
    if (!(plan_id == rhs.plan_id))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_get_plan_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_get_plan_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class stuff_plan_management_get_plan_pargs {
 public:


  virtual ~stuff_plan_management_get_plan_pargs() noexcept;
  const int64_t* plan_id;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_get_plan_result__isset {
  _stuff_plan_management_get_plan_result__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_get_plan_result__isset;

class stuff_plan_management_get_plan_result {
 public:

  stuff_plan_management_get_plan_result(const stuff_plan_management_get_plan_result&);
  stuff_plan_management_get_plan_result& operator=(const stuff_plan_management_get_plan_result&);
  stuff_plan_management_get_plan_result() {
  }

  virtual ~stuff_plan_management_get_plan_result() noexcept;
  stuff_plan success;

  _stuff_plan_management_get_plan_result__isset __isset;

  void __set_success(const stuff_plan& val);

  bool operator == (const stuff_plan_management_get_plan_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_get_plan_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_get_plan_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_get_plan_presult__isset {
  _stuff_plan_management_get_plan_presult__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_get_plan_presult__isset;

class stuff_plan_management_get_plan_presult {
 public:


  virtual ~stuff_plan_management_get_plan_presult() noexcept;
  stuff_plan* success;

  _stuff_plan_management_get_plan_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _stuff_plan_management_update_plan_args__isset {
  _stuff_plan_management_update_plan_args__isset() : plan(false), ssid(false) {}
  bool plan :1;
  bool ssid :1;
} _stuff_plan_management_update_plan_args__isset;

class stuff_plan_management_update_plan_args {
 public:

  stuff_plan_management_update_plan_args(const stuff_plan_management_update_plan_args&);
  stuff_plan_management_update_plan_args& operator=(const stuff_plan_management_update_plan_args&);
  stuff_plan_management_update_plan_args() : ssid() {
  }

  virtual ~stuff_plan_management_update_plan_args() noexcept;
  stuff_plan plan;
  std::string ssid;

  _stuff_plan_management_update_plan_args__isset __isset;

  void __set_plan(const stuff_plan& val);

  void __set_ssid(const std::string& val);

  bool operator == (const stuff_plan_management_update_plan_args & rhs) const
  {
    if (!(plan == rhs.plan))
      return false;
    if (!(ssid == rhs.ssid))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_update_plan_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_update_plan_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class stuff_plan_management_update_plan_pargs {
 public:


  virtual ~stuff_plan_management_update_plan_pargs() noexcept;
  const stuff_plan* plan;
  const std::string* ssid;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_update_plan_result__isset {
  _stuff_plan_management_update_plan_result__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_update_plan_result__isset;

class stuff_plan_management_update_plan_result {
 public:

  stuff_plan_management_update_plan_result(const stuff_plan_management_update_plan_result&);
  stuff_plan_management_update_plan_result& operator=(const stuff_plan_management_update_plan_result&);
  stuff_plan_management_update_plan_result() : success(0) {
  }

  virtual ~stuff_plan_management_update_plan_result() noexcept;
  bool success;

  _stuff_plan_management_update_plan_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const stuff_plan_management_update_plan_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_update_plan_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_update_plan_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_update_plan_presult__isset {
  _stuff_plan_management_update_plan_presult__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_update_plan_presult__isset;

class stuff_plan_management_update_plan_presult {
 public:


  virtual ~stuff_plan_management_update_plan_presult() noexcept;
  bool* success;

  _stuff_plan_management_update_plan_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _stuff_plan_management_confirm_plan_args__isset {
  _stuff_plan_management_confirm_plan_args__isset() : plan_id(false), ssid(false), confirm(false) {}
  bool plan_id :1;
  bool ssid :1;
  bool confirm :1;
} _stuff_plan_management_confirm_plan_args__isset;

class stuff_plan_management_confirm_plan_args {
 public:

  stuff_plan_management_confirm_plan_args(const stuff_plan_management_confirm_plan_args&);
  stuff_plan_management_confirm_plan_args& operator=(const stuff_plan_management_confirm_plan_args&);
  stuff_plan_management_confirm_plan_args() : plan_id(0), ssid(), confirm(0) {
  }

  virtual ~stuff_plan_management_confirm_plan_args() noexcept;
  int64_t plan_id;
  std::string ssid;
  bool confirm;

  _stuff_plan_management_confirm_plan_args__isset __isset;

  void __set_plan_id(const int64_t val);

  void __set_ssid(const std::string& val);

  void __set_confirm(const bool val);

  bool operator == (const stuff_plan_management_confirm_plan_args & rhs) const
  {
    if (!(plan_id == rhs.plan_id))
      return false;
    if (!(ssid == rhs.ssid))
      return false;
    if (!(confirm == rhs.confirm))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_confirm_plan_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_confirm_plan_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class stuff_plan_management_confirm_plan_pargs {
 public:


  virtual ~stuff_plan_management_confirm_plan_pargs() noexcept;
  const int64_t* plan_id;
  const std::string* ssid;
  const bool* confirm;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_confirm_plan_result__isset {
  _stuff_plan_management_confirm_plan_result__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_confirm_plan_result__isset;

class stuff_plan_management_confirm_plan_result {
 public:

  stuff_plan_management_confirm_plan_result(const stuff_plan_management_confirm_plan_result&);
  stuff_plan_management_confirm_plan_result& operator=(const stuff_plan_management_confirm_plan_result&);
  stuff_plan_management_confirm_plan_result() : success(0) {
  }

  virtual ~stuff_plan_management_confirm_plan_result() noexcept;
  bool success;

  _stuff_plan_management_confirm_plan_result__isset __isset;

  void __set_success(const bool val);

  bool operator == (const stuff_plan_management_confirm_plan_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const stuff_plan_management_confirm_plan_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan_management_confirm_plan_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _stuff_plan_management_confirm_plan_presult__isset {
  _stuff_plan_management_confirm_plan_presult__isset() : success(false) {}
  bool success :1;
} _stuff_plan_management_confirm_plan_presult__isset;

class stuff_plan_management_confirm_plan_presult {
 public:


  virtual ~stuff_plan_management_confirm_plan_presult() noexcept;
  bool* success;

  _stuff_plan_management_confirm_plan_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class stuff_plan_managementClient : virtual public stuff_plan_managementIf {
 public:
  stuff_plan_managementClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  stuff_plan_managementClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
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
  int64_t create_plan(const stuff_plan& plan, const std::string& ssid);
  void send_create_plan(const stuff_plan& plan, const std::string& ssid);
  int64_t recv_create_plan();
  void get_created_plan(std::vector<int64_t> & _return, const std::string& ssid);
  void send_get_created_plan(const std::string& ssid);
  void recv_get_created_plan(std::vector<int64_t> & _return);
  void get_company_plan(std::vector<int64_t> & _return, const std::string& ssid);
  void send_get_company_plan(const std::string& ssid);
  void recv_get_company_plan(std::vector<int64_t> & _return);
  void get_plan(stuff_plan& _return, const int64_t plan_id);
  void send_get_plan(const int64_t plan_id);
  void recv_get_plan(stuff_plan& _return);
  bool update_plan(const stuff_plan& plan, const std::string& ssid);
  void send_update_plan(const stuff_plan& plan, const std::string& ssid);
  bool recv_update_plan();
  bool confirm_plan(const int64_t plan_id, const std::string& ssid, const bool confirm);
  void send_confirm_plan(const int64_t plan_id, const std::string& ssid, const bool confirm);
  bool recv_confirm_plan();
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class stuff_plan_managementProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<stuff_plan_managementIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (stuff_plan_managementProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_create_plan(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_get_created_plan(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_get_company_plan(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_get_plan(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_update_plan(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_confirm_plan(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  stuff_plan_managementProcessor(::std::shared_ptr<stuff_plan_managementIf> iface) :
    iface_(iface) {
    processMap_["create_plan"] = &stuff_plan_managementProcessor::process_create_plan;
    processMap_["get_created_plan"] = &stuff_plan_managementProcessor::process_get_created_plan;
    processMap_["get_company_plan"] = &stuff_plan_managementProcessor::process_get_company_plan;
    processMap_["get_plan"] = &stuff_plan_managementProcessor::process_get_plan;
    processMap_["update_plan"] = &stuff_plan_managementProcessor::process_update_plan;
    processMap_["confirm_plan"] = &stuff_plan_managementProcessor::process_confirm_plan;
  }

  virtual ~stuff_plan_managementProcessor() {}
};

class stuff_plan_managementProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  stuff_plan_managementProcessorFactory(const ::std::shared_ptr< stuff_plan_managementIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< stuff_plan_managementIfFactory > handlerFactory_;
};

class stuff_plan_managementMultiface : virtual public stuff_plan_managementIf {
 public:
  stuff_plan_managementMultiface(std::vector<std::shared_ptr<stuff_plan_managementIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~stuff_plan_managementMultiface() {}
 protected:
  std::vector<std::shared_ptr<stuff_plan_managementIf> > ifaces_;
  stuff_plan_managementMultiface() {}
  void add(::std::shared_ptr<stuff_plan_managementIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  int64_t create_plan(const stuff_plan& plan, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->create_plan(plan, ssid);
    }
    return ifaces_[i]->create_plan(plan, ssid);
  }

  void get_created_plan(std::vector<int64_t> & _return, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_created_plan(_return, ssid);
    }
    ifaces_[i]->get_created_plan(_return, ssid);
    return;
  }

  void get_company_plan(std::vector<int64_t> & _return, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_company_plan(_return, ssid);
    }
    ifaces_[i]->get_company_plan(_return, ssid);
    return;
  }

  void get_plan(stuff_plan& _return, const int64_t plan_id) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->get_plan(_return, plan_id);
    }
    ifaces_[i]->get_plan(_return, plan_id);
    return;
  }

  bool update_plan(const stuff_plan& plan, const std::string& ssid) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->update_plan(plan, ssid);
    }
    return ifaces_[i]->update_plan(plan, ssid);
  }

  bool confirm_plan(const int64_t plan_id, const std::string& ssid, const bool confirm) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->confirm_plan(plan_id, ssid, confirm);
    }
    return ifaces_[i]->confirm_plan(plan_id, ssid, confirm);
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class stuff_plan_managementConcurrentClient : virtual public stuff_plan_managementIf {
 public:
  stuff_plan_managementConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  stuff_plan_managementConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
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
  int64_t create_plan(const stuff_plan& plan, const std::string& ssid);
  int32_t send_create_plan(const stuff_plan& plan, const std::string& ssid);
  int64_t recv_create_plan(const int32_t seqid);
  void get_created_plan(std::vector<int64_t> & _return, const std::string& ssid);
  int32_t send_get_created_plan(const std::string& ssid);
  void recv_get_created_plan(std::vector<int64_t> & _return, const int32_t seqid);
  void get_company_plan(std::vector<int64_t> & _return, const std::string& ssid);
  int32_t send_get_company_plan(const std::string& ssid);
  void recv_get_company_plan(std::vector<int64_t> & _return, const int32_t seqid);
  void get_plan(stuff_plan& _return, const int64_t plan_id);
  int32_t send_get_plan(const int64_t plan_id);
  void recv_get_plan(stuff_plan& _return, const int32_t seqid);
  bool update_plan(const stuff_plan& plan, const std::string& ssid);
  int32_t send_update_plan(const stuff_plan& plan, const std::string& ssid);
  bool recv_update_plan(const int32_t seqid);
  bool confirm_plan(const int64_t plan_id, const std::string& ssid, const bool confirm);
  int32_t send_confirm_plan(const int64_t plan_id, const std::string& ssid, const bool confirm);
  bool recv_confirm_plan(const int32_t seqid);
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
