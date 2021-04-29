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

class driver_info;

class vichele_info_t;

class stuff_detail;

class user_apply;

class company_address_contact_info;

class company_attachment;

class plan_confirm_info;

class pay_confirm_info;

class vichele_in_plan;

class stuff_plan;

class plan_status;

class plan_number_id;

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

typedef struct _driver_info__isset {
  _driver_info__isset() : name(false), phone(false), id(false) {}
  bool name :1;
  bool phone :1;
  bool id :1;
} _driver_info__isset;

class driver_info : public virtual ::apache::thrift::TBase {
 public:

  driver_info(const driver_info&);
  driver_info& operator=(const driver_info&);
  driver_info() : name(), phone(), id(0) {
  }

  virtual ~driver_info() noexcept;
  std::string name;
  std::string phone;
  int64_t id;

  _driver_info__isset __isset;

  void __set_name(const std::string& val);

  void __set_phone(const std::string& val);

  void __set_id(const int64_t val);

  bool operator == (const driver_info & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(phone == rhs.phone))
      return false;
    if (!(id == rhs.id))
      return false;
    return true;
  }
  bool operator != (const driver_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const driver_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(driver_info &a, driver_info &b);

std::ostream& operator<<(std::ostream& out, const driver_info& obj);

typedef struct _vichele_info_t__isset {
  _vichele_info_t__isset() : number(false), id(false) {}
  bool number :1;
  bool id :1;
} _vichele_info_t__isset;

class vichele_info_t : public virtual ::apache::thrift::TBase {
 public:

  vichele_info_t(const vichele_info_t&);
  vichele_info_t& operator=(const vichele_info_t&);
  vichele_info_t() : number(), id(0) {
  }

  virtual ~vichele_info_t() noexcept;
  std::string number;
  int64_t id;

  _vichele_info_t__isset __isset;

  void __set_number(const std::string& val);

  void __set_id(const int64_t val);

  bool operator == (const vichele_info_t & rhs) const
  {
    if (!(number == rhs.number))
      return false;
    if (!(id == rhs.id))
      return false;
    return true;
  }
  bool operator != (const vichele_info_t &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const vichele_info_t & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(vichele_info_t &a, vichele_info_t &b);

std::ostream& operator<<(std::ostream& out, const vichele_info_t& obj);

typedef struct _stuff_detail__isset {
  _stuff_detail__isset() : name(false), last(false), price(false), company(false), type_id(false), saling(false), company_address(false), company_contact(false) {}
  bool name :1;
  bool last :1;
  bool price :1;
  bool company :1;
  bool type_id :1;
  bool saling :1;
  bool company_address :1;
  bool company_contact :1;
} _stuff_detail__isset;

class stuff_detail : public virtual ::apache::thrift::TBase {
 public:

  stuff_detail(const stuff_detail&);
  stuff_detail& operator=(const stuff_detail&);
  stuff_detail() : name(), last(), price(0), company(), type_id(0), saling(0), company_address(), company_contact() {
  }

  virtual ~stuff_detail() noexcept;
  std::string name;
  std::string last;
  int64_t price;
  std::string company;
  int64_t type_id;
  bool saling;
  std::string company_address;
  std::string company_contact;

  _stuff_detail__isset __isset;

  void __set_name(const std::string& val);

  void __set_last(const std::string& val);

  void __set_price(const int64_t val);

  void __set_company(const std::string& val);

  void __set_type_id(const int64_t val);

  void __set_saling(const bool val);

  void __set_company_address(const std::string& val);

  void __set_company_contact(const std::string& val);

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
    if (!(company_address == rhs.company_address))
      return false;
    if (!(company_contact == rhs.company_contact))
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

typedef struct _company_address_contact_info__isset {
  _company_address_contact_info__isset() : address(false), contact(false) {}
  bool address :1;
  bool contact :1;
} _company_address_contact_info__isset;

class company_address_contact_info : public virtual ::apache::thrift::TBase {
 public:

  company_address_contact_info(const company_address_contact_info&);
  company_address_contact_info& operator=(const company_address_contact_info&);
  company_address_contact_info() : address(), contact() {
  }

  virtual ~company_address_contact_info() noexcept;
  std::string address;
  std::string contact;

  _company_address_contact_info__isset __isset;

  void __set_address(const std::string& val);

  void __set_contact(const std::string& val);

  bool operator == (const company_address_contact_info & rhs) const
  {
    if (!(address == rhs.address))
      return false;
    if (!(contact == rhs.contact))
      return false;
    return true;
  }
  bool operator != (const company_address_contact_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_address_contact_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(company_address_contact_info &a, company_address_contact_info &b);

std::ostream& operator<<(std::ostream& out, const company_address_contact_info& obj);

typedef struct _company_attachment__isset {
  _company_attachment__isset() : id(false), path(false), pic_path(false) {}
  bool id :1;
  bool path :1;
  bool pic_path :1;
} _company_attachment__isset;

class company_attachment : public virtual ::apache::thrift::TBase {
 public:

  company_attachment(const company_attachment&);
  company_attachment& operator=(const company_attachment&);
  company_attachment() : id(0), path(), pic_path() {
  }

  virtual ~company_attachment() noexcept;
  int64_t id;
  std::string path;
  std::string pic_path;

  _company_attachment__isset __isset;

  void __set_id(const int64_t val);

  void __set_path(const std::string& val);

  void __set_pic_path(const std::string& val);

  bool operator == (const company_attachment & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(path == rhs.path))
      return false;
    if (!(pic_path == rhs.pic_path))
      return false;
    return true;
  }
  bool operator != (const company_attachment &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const company_attachment & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(company_attachment &a, company_attachment &b);

std::ostream& operator<<(std::ostream& out, const company_attachment& obj);

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

typedef struct _vichele_in_plan__isset {
  _vichele_in_plan__isset() : main_vichele(false), behind_vichele(false), driver_name(false), driver_phone(false), count(false), drop_address(false), use_for(false) {}
  bool main_vichele :1;
  bool behind_vichele :1;
  bool driver_name :1;
  bool driver_phone :1;
  bool count :1;
  bool drop_address :1;
  bool use_for :1;
} _vichele_in_plan__isset;

class vichele_in_plan : public virtual ::apache::thrift::TBase {
 public:

  vichele_in_plan(const vichele_in_plan&);
  vichele_in_plan& operator=(const vichele_in_plan&);
  vichele_in_plan() : main_vichele(), behind_vichele(), driver_name(), driver_phone(), count(0), drop_address(), use_for() {
  }

  virtual ~vichele_in_plan() noexcept;
  std::string main_vichele;
  std::string behind_vichele;
  std::string driver_name;
  std::string driver_phone;
  double count;
  std::string drop_address;
  std::string use_for;

  _vichele_in_plan__isset __isset;

  void __set_main_vichele(const std::string& val);

  void __set_behind_vichele(const std::string& val);

  void __set_driver_name(const std::string& val);

  void __set_driver_phone(const std::string& val);

  void __set_count(const double val);

  void __set_drop_address(const std::string& val);

  void __set_use_for(const std::string& val);

  bool operator == (const vichele_in_plan & rhs) const
  {
    if (!(main_vichele == rhs.main_vichele))
      return false;
    if (!(behind_vichele == rhs.behind_vichele))
      return false;
    if (!(driver_name == rhs.driver_name))
      return false;
    if (!(driver_phone == rhs.driver_phone))
      return false;
    if (!(count == rhs.count))
      return false;
    if (!(drop_address == rhs.drop_address))
      return false;
    if (!(use_for == rhs.use_for))
      return false;
    return true;
  }
  bool operator != (const vichele_in_plan &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const vichele_in_plan & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(vichele_in_plan &a, vichele_in_plan &b);

std::ostream& operator<<(std::ostream& out, const vichele_in_plan& obj);

typedef struct _stuff_plan__isset {
  _stuff_plan__isset() : type_id(false), count(false), vichele_info(false), plan_id(false), plan_time(false), created_time(false), name(false), price(false), status(false), comment(false), plan_confirm(false), pay_confirm(false), pay_info(false), pay_timestamp(false), close_timestamp(false), close_by(false), except_close_by(false), except_close_timestamp(false), except_close_reason(false), proxy_company(false), created_user_name(false), buy_company(false), sale_company(false), reject_reason(false) {}
  bool type_id :1;
  bool count :1;
  bool vichele_info :1;
  bool plan_id :1;
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
  bool close_by :1;
  bool except_close_by :1;
  bool except_close_timestamp :1;
  bool except_close_reason :1;
  bool proxy_company :1;
  bool created_user_name :1;
  bool buy_company :1;
  bool sale_company :1;
  bool reject_reason :1;
} _stuff_plan__isset;

class stuff_plan : public virtual ::apache::thrift::TBase {
 public:

  stuff_plan(const stuff_plan&);
  stuff_plan& operator=(const stuff_plan&);
  stuff_plan() : type_id(0), count(0), plan_id(0), plan_time(), created_time(0), name(), price(0), status(0), comment(), pay_info(), pay_timestamp(), close_timestamp(), close_by(), except_close_by(), except_close_timestamp(), except_close_reason(), proxy_company(), created_user_name(), buy_company(), sale_company(), reject_reason() {
  }

  virtual ~stuff_plan() noexcept;
  int64_t type_id;
  double count;
  std::vector<vichele_in_plan>  vichele_info;
  int64_t plan_id;
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
  std::string close_by;
  std::string except_close_by;
  std::string except_close_timestamp;
  std::string except_close_reason;
  std::string proxy_company;
  std::string created_user_name;
  std::string buy_company;
  std::string sale_company;
  std::string reject_reason;

  _stuff_plan__isset __isset;

  void __set_type_id(const int64_t val);

  void __set_count(const double val);

  void __set_vichele_info(const std::vector<vichele_in_plan> & val);

  void __set_plan_id(const int64_t val);

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

  void __set_close_by(const std::string& val);

  void __set_except_close_by(const std::string& val);

  void __set_except_close_timestamp(const std::string& val);

  void __set_except_close_reason(const std::string& val);

  void __set_proxy_company(const std::string& val);

  void __set_created_user_name(const std::string& val);

  void __set_buy_company(const std::string& val);

  void __set_sale_company(const std::string& val);

  void __set_reject_reason(const std::string& val);

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
    if (!(close_by == rhs.close_by))
      return false;
    if (!(except_close_by == rhs.except_close_by))
      return false;
    if (!(except_close_timestamp == rhs.except_close_timestamp))
      return false;
    if (!(except_close_reason == rhs.except_close_reason))
      return false;
    if (!(proxy_company == rhs.proxy_company))
      return false;
    if (!(created_user_name == rhs.created_user_name))
      return false;
    if (!(buy_company == rhs.buy_company))
      return false;
    if (!(sale_company == rhs.sale_company))
      return false;
    if (!(reject_reason == rhs.reject_reason))
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

typedef struct _plan_status__isset {
  _plan_status__isset() : plan_id(false), status(false), plan_time(false), conflict_reason(false) {}
  bool plan_id :1;
  bool status :1;
  bool plan_time :1;
  bool conflict_reason :1;
} _plan_status__isset;

class plan_status : public virtual ::apache::thrift::TBase {
 public:

  plan_status(const plan_status&);
  plan_status& operator=(const plan_status&);
  plan_status() : plan_id(0), status(0), plan_time(0), conflict_reason() {
  }

  virtual ~plan_status() noexcept;
  int64_t plan_id;
  int64_t status;
  int64_t plan_time;
  std::string conflict_reason;

  _plan_status__isset __isset;

  void __set_plan_id(const int64_t val);

  void __set_status(const int64_t val);

  void __set_plan_time(const int64_t val);

  void __set_conflict_reason(const std::string& val);

  bool operator == (const plan_status & rhs) const
  {
    if (!(plan_id == rhs.plan_id))
      return false;
    if (!(status == rhs.status))
      return false;
    if (!(plan_time == rhs.plan_time))
      return false;
    if (!(conflict_reason == rhs.conflict_reason))
      return false;
    return true;
  }
  bool operator != (const plan_status &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const plan_status & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(plan_status &a, plan_status &b);

std::ostream& operator<<(std::ostream& out, const plan_status& obj);

typedef struct _plan_number_id__isset {
  _plan_number_id__isset() : id(false), number(false) {}
  bool id :1;
  bool number :1;
} _plan_number_id__isset;

class plan_number_id : public virtual ::apache::thrift::TBase {
 public:

  plan_number_id(const plan_number_id&);
  plan_number_id& operator=(const plan_number_id&);
  plan_number_id() : id(0), number() {
  }

  virtual ~plan_number_id() noexcept;
  int64_t id;
  std::string number;

  _plan_number_id__isset __isset;

  void __set_id(const int64_t val);

  void __set_number(const std::string& val);

  bool operator == (const plan_number_id & rhs) const
  {
    if (!(id == rhs.id))
      return false;
    if (!(number == rhs.number))
      return false;
    return true;
  }
  bool operator != (const plan_number_id &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const plan_number_id & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(plan_number_id &a, plan_number_id &b);

std::ostream& operator<<(std::ostream& out, const plan_number_id& obj);



#endif
