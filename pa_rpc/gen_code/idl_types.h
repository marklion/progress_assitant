/**
 * Autogenerated by Thrift Compiler (0.14.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef idl_TYPES_H
#define idl_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>




class gen_exp;

class user_info;

class stuff_detail;

class user_apply;

class plan_confirm_info;

class pay_confirm_info;

class stuff_plan;

typedef struct _gen_exp__isset {
  _gen_exp__isset() : msg(false) {}
  bool msg :1;
} _gen_exp__isset;

class gen_exp : public ::apache::thrift::TException {
 public:

  gen_exp(const gen_exp&);
  gen_exp& operator=(const gen_exp&);
  gen_exp() : msg() {
  }

  virtual ~gen_exp() noexcept;
  std::string msg;

  _gen_exp__isset __isset;

  void __set_msg(const std::string& val);

  bool operator == (const gen_exp & rhs) const
  {
    if (!(msg == rhs.msg))
      return false;
    return true;
  }
  bool operator != (const gen_exp &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const gen_exp & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
  mutable std::string thriftTExceptionMessageHolder_;
  const char* what() const noexcept;
};

void swap(gen_exp &a, gen_exp &b);

std::ostream& operator<<(std::ostream& out, const gen_exp& obj);

typedef struct _user_info__isset {
  _user_info__isset() : user_id(false), name(false), logo(false), company(false), buyer(false), phone(false) {}
  bool user_id :1;
  bool name :1;
  bool logo :1;
  bool company :1;
  bool buyer :1;
  bool phone :1;
} _user_info__isset;

class user_info : public virtual ::apache::thrift::TBase {
 public:

  user_info(const user_info&);
  user_info& operator=(const user_info&);
  user_info() : user_id(0), name(), logo(), company(), buyer(0), phone() {
  }

  virtual ~user_info() noexcept;
  int64_t user_id;
  std::string name;
  std::string logo;
  std::string company;
  bool buyer;
  std::string phone;

  _user_info__isset __isset;

  void __set_user_id(const int64_t val);

  void __set_name(const std::string& val);

  void __set_logo(const std::string& val);

  void __set_company(const std::string& val);

  void __set_buyer(const bool val);

  void __set_phone(const std::string& val);

  bool operator == (const user_info & rhs) const
  {
    if (!(user_id == rhs.user_id))
      return false;
    if (!(name == rhs.name))
      return false;
    if (!(logo == rhs.logo))
      return false;
    if (!(company == rhs.company))
      return false;
    if (!(buyer == rhs.buyer))
      return false;
    if (!(phone == rhs.phone))
      return false;
    return true;
  }
  bool operator != (const user_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const user_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(user_info &a, user_info &b);

std::ostream& operator<<(std::ostream& out, const user_info& obj);

typedef struct _stuff_detail__isset {
  _stuff_detail__isset() : name(false), last(false), price(false), company(false), type_id(false), saling(false) {}
  bool name :1;
  bool last :1;
  bool price :1;
  bool company :1;
  bool type_id :1;
  bool saling :1;
} _stuff_detail__isset;

class stuff_detail : public virtual ::apache::thrift::TBase {
 public:

  stuff_detail(const stuff_detail&);
  stuff_detail& operator=(const stuff_detail&);
  stuff_detail() : name(), last(), price(0), company(), type_id(0), saling(0) {
  }

  virtual ~stuff_detail() noexcept;
  std::string name;
  std::string last;
  int64_t price;
  std::string company;
  int64_t type_id;
  bool saling;

  _stuff_detail__isset __isset;

  void __set_name(const std::string& val);

  void __set_last(const std::string& val);

  void __set_price(const int64_t val);

  void __set_company(const std::string& val);

  void __set_type_id(const int64_t val);

  void __set_saling(const bool val);

  bool operator == (const stuff_detail & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(last == rhs.last))
      return false;
    if (!(price == rhs.price))
      return false;
    if (!(company == rhs.company))
      return false;
    if (!(type_id == rhs.type_id))
      return false;
    if (!(saling == rhs.saling))
      return false;
    return true;
  }
  bool operator != (const stuff_detail &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_detail & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(stuff_detail &a, stuff_detail &b);

std::ostream& operator<<(std::ostream& out, const stuff_detail& obj);

typedef struct _user_apply__isset {
  _user_apply__isset() : name(false), phone(false), logo(false), apply_id(false), status(false) {}
  bool name :1;
  bool phone :1;
  bool logo :1;
  bool apply_id :1;
  bool status :1;
} _user_apply__isset;

class user_apply : public virtual ::apache::thrift::TBase {
 public:

  user_apply(const user_apply&);
  user_apply& operator=(const user_apply&);
  user_apply() : name(), phone(), logo(), apply_id(0), status(0) {
  }

  virtual ~user_apply() noexcept;
  std::string name;
  std::string phone;
  std::string logo;
  int64_t apply_id;
  int64_t status;

  _user_apply__isset __isset;

  void __set_name(const std::string& val);

  void __set_phone(const std::string& val);

  void __set_logo(const std::string& val);

  void __set_apply_id(const int64_t val);

  void __set_status(const int64_t val);

  bool operator == (const user_apply & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(phone == rhs.phone))
      return false;
    if (!(logo == rhs.logo))
      return false;
    if (!(apply_id == rhs.apply_id))
      return false;
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const user_apply &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const user_apply & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(user_apply &a, user_apply &b);

std::ostream& operator<<(std::ostream& out, const user_apply& obj);

typedef struct _plan_confirm_info__isset {
  _plan_confirm_info__isset() : timestamp(false), name(false) {}
  bool timestamp :1;
  bool name :1;
} _plan_confirm_info__isset;

class plan_confirm_info : public virtual ::apache::thrift::TBase {
 public:

  plan_confirm_info(const plan_confirm_info&);
  plan_confirm_info& operator=(const plan_confirm_info&);
  plan_confirm_info() : timestamp(), name() {
  }

  virtual ~plan_confirm_info() noexcept;
  std::string timestamp;
  std::string name;

  _plan_confirm_info__isset __isset;

  void __set_timestamp(const std::string& val);

  void __set_name(const std::string& val);

  bool operator == (const plan_confirm_info & rhs) const
  {
    if (!(timestamp == rhs.timestamp))
      return false;
    if (!(name == rhs.name))
      return false;
    return true;
  }
  bool operator != (const plan_confirm_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const plan_confirm_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(plan_confirm_info &a, plan_confirm_info &b);

std::ostream& operator<<(std::ostream& out, const plan_confirm_info& obj);

typedef struct _pay_confirm_info__isset {
  _pay_confirm_info__isset() : timestamp(false), name(false) {}
  bool timestamp :1;
  bool name :1;
} _pay_confirm_info__isset;

class pay_confirm_info : public virtual ::apache::thrift::TBase {
 public:

  pay_confirm_info(const pay_confirm_info&);
  pay_confirm_info& operator=(const pay_confirm_info&);
  pay_confirm_info() : timestamp(), name() {
  }

  virtual ~pay_confirm_info() noexcept;
  std::string timestamp;
  std::string name;

  _pay_confirm_info__isset __isset;

  void __set_timestamp(const std::string& val);

  void __set_name(const std::string& val);

  bool operator == (const pay_confirm_info & rhs) const
  {
    if (!(timestamp == rhs.timestamp))
      return false;
    if (!(name == rhs.name))
      return false;
    return true;
  }
  bool operator != (const pay_confirm_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const pay_confirm_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(pay_confirm_info &a, pay_confirm_info &b);

std::ostream& operator<<(std::ostream& out, const pay_confirm_info& obj);

typedef struct _stuff_plan__isset {
  _stuff_plan__isset() : type_id(false), count(false), vichele_info(false), plan_id(false), created_by(false), plan_time(false), created_time(false), name(false), price(false), status(false), comment(false), plan_confirm(false), pay_confirm(false), pay_info(false), pay_timestamp(false), close_timestamp(false) {}
  bool type_id :1;
  bool count :1;
  bool vichele_info :1;
  bool plan_id :1;
  bool created_by :1;
  bool plan_time :1;
  bool created_time :1;
  bool name :1;
  bool price :1;
  bool status :1;
  bool comment :1;
  bool plan_confirm :1;
  bool pay_confirm :1;
  bool pay_info :1;
  bool pay_timestamp :1;
  bool close_timestamp :1;
} _stuff_plan__isset;

class stuff_plan : public virtual ::apache::thrift::TBase {
 public:

  stuff_plan(const stuff_plan&);
  stuff_plan& operator=(const stuff_plan&);
  stuff_plan() : type_id(0), count(0), plan_id(0), created_by(0), plan_time(), created_time(0), name(), price(0), status(0), comment(), pay_info(), pay_timestamp(), close_timestamp() {
  }

  virtual ~stuff_plan() noexcept;
  int64_t type_id;
  double count;
  std::vector<std::string>  vichele_info;
  int64_t plan_id;
  int64_t created_by;
  std::string plan_time;
  int64_t created_time;
  std::string name;
  double price;
  int64_t status;
  std::string comment;
  plan_confirm_info plan_confirm;
  pay_confirm_info pay_confirm;
  std::string pay_info;
  std::string pay_timestamp;
  std::string close_timestamp;

  _stuff_plan__isset __isset;

  void __set_type_id(const int64_t val);

  void __set_count(const double val);

  void __set_vichele_info(const std::vector<std::string> & val);

  void __set_plan_id(const int64_t val);

  void __set_created_by(const int64_t val);

  void __set_plan_time(const std::string& val);

  void __set_created_time(const int64_t val);

  void __set_name(const std::string& val);

  void __set_price(const double val);

  void __set_status(const int64_t val);

  void __set_comment(const std::string& val);

  void __set_plan_confirm(const plan_confirm_info& val);

  void __set_pay_confirm(const pay_confirm_info& val);

  void __set_pay_info(const std::string& val);

  void __set_pay_timestamp(const std::string& val);

  void __set_close_timestamp(const std::string& val);

  bool operator == (const stuff_plan & rhs) const
  {
    if (!(type_id == rhs.type_id))
      return false;
    if (!(count == rhs.count))
      return false;
    if (!(vichele_info == rhs.vichele_info))
      return false;
    if (!(plan_id == rhs.plan_id))
      return false;
    if (!(created_by == rhs.created_by))
      return false;
    if (!(plan_time == rhs.plan_time))
      return false;
    if (!(created_time == rhs.created_time))
      return false;
    if (!(name == rhs.name))
      return false;
    if (!(price == rhs.price))
      return false;
    if (!(status == rhs.status))
      return false;
    if (!(comment == rhs.comment))
      return false;
    if (!(plan_confirm == rhs.plan_confirm))
      return false;
    if (!(pay_confirm == rhs.pay_confirm))
      return false;
    if (!(pay_info == rhs.pay_info))
      return false;
    if (!(pay_timestamp == rhs.pay_timestamp))
      return false;
    if (!(close_timestamp == rhs.close_timestamp))
      return false;
    return true;
  }
  bool operator != (const stuff_plan &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(stuff_plan &a, stuff_plan &b);

std::ostream& operator<<(std::ostream& out, const stuff_plan& obj);



#endif
